
#include "ConstOptimizer.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>

using namespace std;


ConstOptimizer::ConstOptimizer(
    Meter& meter,
    size_t maxIterations,
    size_t maxPasses,
    double initialStep,
    double target,
    size_t maxJointIterations,
    size_t maxGlobalAttempts
) :
    meter(meter),
    maxIterations(maxIterations),
    maxPasses(maxPasses),
    maxJointIterations(maxJointIterations),
    maxGlobalAttempts(maxGlobalAttempts),
    initialStep(initialStep),
    target(target),
    rand(random_device{}())
{}

void ConstOptimizer::collectConstants(
    Node& node,
    vector<Node*>& constants
) {
    if (node.el == Element::Const) {
        constants.push_back(&node);
        return;
    }

    if (node.left != nullptr)
        collectConstants(*node.left, constants);

    if (node.right != nullptr)
        collectConstants(*node.right, constants);
}

void ConstOptimizer::optimizeConstant(
    Node& root,
    Node& constant
) {
    double currentValue = constant.value;

    if (!isfinite(currentValue))
        return;

    double currentMeasure = meter.measure(root);

    double step = max(
        target,
        abs(currentValue) * initialStep
    );

    for (size_t i = 0;
        i < maxIterations && step >= target;
        i++)
    {
        double bestValue = currentValue;
        double bestMeasure = currentMeasure;

        double plusValue = currentValue + step;

        if (isfinite(plusValue)) {
            constant.value = plusValue;

            double plusMeasure = meter.measure(root);

            if (plusMeasure < bestMeasure) {
                bestMeasure = plusMeasure;
                bestValue = plusValue;
            }
        }

        double minusValue = currentValue - step;

        if (isfinite(minusValue)) {
            constant.value = minusValue;

            double minusMeasure = meter.measure(root);

            if (minusMeasure < bestMeasure) {
                bestMeasure = minusMeasure;
                bestValue = minusValue;
            }
        }

        if (bestMeasure < currentMeasure) {
            currentValue = bestValue;
            currentMeasure = bestMeasure;

            constant.value = currentValue;

            step *= 1.5;
        }
        else {
            step *= 0.5;
        }
    }

    constant.value = currentValue;
}

void ConstOptimizer::optimizeTogether(
    Node& root,
    vector<Node*>& constants
) {
    if (constants.size() < 2)
        return;

    size_t count = constants.size();

    vector<double> currentValues(count);
    vector<double> delta(count);

    for (size_t i = 0; i < count; i++)
        currentValues[i] = constants[i]->value;

    double currentMeasure = meter.measure(root);

    double scale = 1.0;

    vector<size_t> indexes(count);

    for (size_t i = 0; i < count; i++)
        indexes[i] = i;

    for (size_t iteration = 0;
        iteration < maxJointIterations && scale >= target;
        iteration++)
    {
        shuffle(indexes.begin(), indexes.end(), rand);

        uniform_int_distribution<size_t> countDist(2, count);
        size_t selectedCount = countDist(rand);

        fill(delta.begin(), delta.end(), 0.0);

        for (size_t i = 0; i < selectedCount; i++) {
            size_t id = indexes[i];

            double sigma = max(
                target,
                abs(currentValues[id]) * initialStep
            );

            normal_distribution<double> noise(
                0.0,
                sigma * scale
            );

            delta[id] = noise(rand);
        }

        for (size_t i = 0; i < count; i++)
            constants[i]->value = currentValues[i];

        double plusMeasure = INFINITY;
        double minusMeasure = INFINITY;

        vector<double> plusValues(count);
        vector<double> minusValues(count);

        bool plusValid = true;
        bool minusValid = true;

        for (size_t i = 0; i < count; i++) {
            plusValues[i] = currentValues[i] + delta[i];
            minusValues[i] = currentValues[i] - delta[i];

            if (!isfinite(plusValues[i]))
                plusValid = false;

            if (!isfinite(minusValues[i]))
                minusValid = false;
        }

        if (plusValid) {
            for (size_t i = 0; i < count; i++)
                constants[i]->value = plusValues[i];

            plusMeasure = meter.measure(root);
        }

        if (minusValid) {
            for (size_t i = 0; i < count; i++)
                constants[i]->value = minusValues[i];

            minusMeasure = meter.measure(root);
        }

        for (size_t i = 0; i < count; i++)
            constants[i]->value = currentValues[i];

        if (plusMeasure < currentMeasure &&
            plusMeasure <= minusMeasure)
        {
            currentMeasure = plusMeasure;
            currentValues = plusValues;

            scale = min(2.0, scale * 1.2);
        }
        else if (minusMeasure < currentMeasure) {
            currentMeasure = minusMeasure;
            currentValues = minusValues;

            scale = min(2.0, scale * 1.2);
        }
        else {
            scale *= 0.5;
        }

        for (size_t i = 0; i < count; i++)
            constants[i]->value = currentValues[i];
    }
}

void ConstOptimizer::mutateGlobal(
    vector<Node*>& constants
) {
    if (constants.empty())
        return;

    size_t count = constants.size();

    vector<size_t> indexes(count);

    for (size_t i = 0; i < count; i++)
        indexes[i] = i;

    shuffle(indexes.begin(), indexes.end(), rand);

    uniform_int_distribution<size_t> countDist(1, count);
    size_t selectedCount = countDist(rand);

    uniform_real_distribution<double> exponentDist(-3.0, 3.0);
    uniform_int_distribution<int> signDist(0, 1);

    for (size_t i = 0; i < selectedCount; i++) {
        Node* constant = constants[indexes[i]];

        double oldValue = constant->value;

        double factor = pow(10.0, exponentDist(rand));

        double magnitude;

        if (oldValue == 0.0)
            magnitude = factor;
        else
            magnitude = abs(oldValue) * factor;

        if (!isfinite(magnitude))
            continue;

        double newValue = signDist(rand) ? magnitude : -magnitude;

        constant->value = newValue;
    }
}


void ConstOptimizer::optimize(Node& root) {
    vector<Node*> constants;

    collectConstants(root, constants);

    if (constants.empty())
        return;

    for (size_t pass = 0; pass < maxPasses; pass++) {

        double oldMeasure = meter.measure(root);

        for (Node* constant : constants)
            optimizeConstant(root, *constant);

        optimizeTogether(root, constants);

        double bestMeasure = meter.measure(root);

        vector<double> bestValues(constants.size());

        for (size_t i = 0; i < constants.size(); i++)
            bestValues[i] = constants[i]->value;

        for (size_t attempt = 0;
            attempt < maxGlobalAttempts;
            attempt++)
        {
            for (size_t i = 0; i < constants.size(); i++)
                constants[i]->value = bestValues[i];

            mutateGlobal(constants);

            for (Node* constant : constants)
                optimizeConstant(root, *constant);

            optimizeTogether(root, constants);

            double newMeasure = meter.measure(root);

            if (newMeasure < bestMeasure) {
                bestMeasure = newMeasure;

                for (size_t i = 0; i < constants.size(); i++)
                    bestValues[i] = constants[i]->value;
            }
        }

        for (size_t i = 0; i < constants.size(); i++)
            constants[i]->value = bestValues[i];

        double newMeasure = meter.measure(root);

        if (!(newMeasure < oldMeasure))
            break;
    }
}
