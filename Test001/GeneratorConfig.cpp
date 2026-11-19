#include "GeneratorConfig.h"

GeneratorConfig getGeneratorConfig(IslandStrategy strategy) {
	switch (strategy) {
	case IslandStrategy::Local:
		return {
			30,40,30,
			{30,15,15,10,15,15},
			{50,50}
		};
	case IslandStrategy::Unite:
		return {
			25,35,40,
			{25,15,15,15,15,15},
			{50,50}
		};
	case IslandStrategy::Explore:
		return {
			20,50,30,
			{50,10,10,10,10,10},
			{50,50}
		};
	case IslandStrategy::Struct:
		return {
			20,30,50,
			{20,10,20,20,20,10},
			{50,50}
		};
	}
}