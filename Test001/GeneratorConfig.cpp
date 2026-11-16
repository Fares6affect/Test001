#include "GeneratorConfig.h"

GeneratorConfig getGeneratorConfig(IslandStrategy strategy) {
	switch (strategy) {
	case IslandStrategy::Local:
		return {
			30,40,30,
			{35,15,20,15,15},
			{50,50}
		};
	case IslandStrategy::Unite:
		return {
			25,45,30,
			{40,15,15,15,15},
			{50,50}
		};
	case IslandStrategy::Explore:
		return {
			25,55,20,
			{65,10,10,10,5},
			{50,50}
		};
	case IslandStrategy::Struct:
		return {
			25,45,30,
			{45,10,15,15,15},
			{50,50}
		};
	}
}