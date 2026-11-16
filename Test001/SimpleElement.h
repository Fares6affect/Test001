#pragma once
#include <string>
#include <vector>
using namespace std;

struct SimpleElement {
	double k = 1.0;
	vector<string> mns;

	SimpleElement(
		double k,
		vector<string> mns
	) :
		k(k), mns(mns){}
};