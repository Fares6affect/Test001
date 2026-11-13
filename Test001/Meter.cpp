#include "Meter.h"

Meter::Meter(
	const vector<Data>& data,
	double fine
) :data(data), fine(fine){}

double Meter::measure(const Node& node) const{
	double mse = 0.0;
	try {
		for (const Data& d: data) {
			double difference = calculator.calculate(node, d.parameters) - d.answer;
			if (!isfinite(difference))
				return INFINITY;
			mse += difference * difference;
			if (!isfinite(mse))
				return INFINITY;
		}
	}
	catch (const domain_error&) {
		return INFINITY;
	}
	return mse / data.size() + fine * (node.size());
}