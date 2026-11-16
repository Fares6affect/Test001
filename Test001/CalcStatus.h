#pragma once

enum class CalcStatus {
	Valid,

	LogError,
	ConvError,
	SqrtError,

	NonFinite
};