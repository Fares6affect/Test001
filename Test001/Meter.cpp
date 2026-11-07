#include "Meter.h"

Meter::Meter(
	const vector<Data>& data,
	double fine
) :data(data), fine(fine){}

double Meter::measure(const Node& node) const{
	double mse = 0.0;
	try {
		for (size_t i = 0; i < data.size(); i++) {
			double difference = calculator.calculate(node, data[i].parameters) - data[i].answer;
			if (!isfinite(difference))
				return INFINITY;
			mse += difference * difference;
			if (!isfinite(mse))
				return INFINITY;
		}
	}
	catch (domain_error) {
		return INFINITY;
	}
	return mse / data.size() + fine * (node.size());
}