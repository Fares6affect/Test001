#include "GeneratorConfig.h"

GeneratorConfig getGeneratorConfig(IslandStrategy strategy) {
	switch (strategy) {
	case IslandStrategy::Local:
		return {
			5, 50,
			6, 4,
			{30,15,15,10,15,15},
			{50,50}
		};
	case IslandStrategy::Unite:
		return {
			5, 45,
			4, 6,
			{25,15,15,15,15,15},
			{50,50}
		};
	case IslandStrategy::Explore:
		return {
			3, 40,
			7, 3,
			{50,10,10,10,10,10},
			{50,50}
		};
	case IslandStrategy::Struct:
		return {
			2, 55,
			3, 7,
			{20,10,20,20,20,10},
			{50,50}
		};
	}
}