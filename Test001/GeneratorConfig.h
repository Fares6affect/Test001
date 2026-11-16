#pragma once
#include <array>
#include "IslandStrategy.h"
using namespace std;

struct GeneratorConfig {
	double termProb;
	double unarProb;
	double binProb;

	array<double, 6> unarOperProb;

	array<double, 2> binOperProb;
};

GeneratorConfig getGeneratorConfig(IslandStrategy strategy);