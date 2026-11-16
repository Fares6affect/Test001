#pragma once
#include "CalcStatus.h"

struct CalcResult {
	double value;
	CalcStatus status;

	bool isValid() const {
		return status == CalcStatus::Valid;
	}
};