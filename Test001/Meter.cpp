#include "Meter.h"
#include <iostream>

Meter::Meter(
	const vector<Data>& data,
	double fine
) :data(data), fine(fine){}

double Meter::measure(Node& node) const{
	/*double mse = 0.0;
	for (const Data& d: data) {
		CalcResult res = calculator.tryCalculate(node, d.parameters);
		if (!res.isValid()) return numeric_limits<double>::max();
		double difference = res.value - d.answer;
		mse += difference * difference;
		if (!isfinite(mse))
			return numeric_limits<double>::max();
	}
	return mse / data.size() + fine * (node.size());*/

	double mae = 0.0;
	for (const Data& d : data) {
		CalcResult res = calculator.tryCalculate(node, d.parameters);
		if (!res.isValid()) return numeric_limits<double>::max();
		mae += fabs(res.value - d.answer);
		if (!isfinite(mae))
			return numeric_limits<double>::max();
	}
	return mae / data.size() + fine * (node.size());
}