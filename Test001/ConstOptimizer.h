
#pragma once
#include "Meter.h"
#include "Element.h"

#include <random>

class ConstOptimizer {
private:
    Meter& meter;

    size_t maxIterations;
    size_t maxPasses;
    size_t maxJointIterations;
    size_t maxGlobalAttempts;

    double initialStep;
    double target;

    mt19937 rand;

    void collectConstants(
        Node& node,
        vector<Node*>& constants
    );

    void optimizeConstant(
        Node& root,
        Node& constant
    );

    void optimizeTogether(
        Node& root,
        vector<Node*>& constants
    );

    void mutateGlobal(
        vector<Node*>& constants
    );

public:
    ConstOptimizer(
        Meter& meter,
        size_t maxIterations = 30,
        size_t maxPasses = 3,
        double initialStep = 0.1,
        double target = 1e-3,
        size_t maxJointIterations = 10,
        size_t maxGlobalAttempts = 5
    );

    void optimize(Node& root);
};
