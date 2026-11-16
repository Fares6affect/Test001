#include "ImproverConfig.h"

ImproverConfig getImproverConfig(IslandStrategy strategy) {
	switch (strategy) {
	case IslandStrategy::Local:
		return {
			0.35, 0.55,
			
			0.9,

			0.35, 0.15, 0.1, 0.1, 0.05, 0.1, 0.15
		};

	case IslandStrategy::Unite:
		return {
			0.55, 0.35, 
			
			0.85,

			0.15, 0.05, 0.1, 0.3, 0.1, 0.15, 0.15
		};

	case IslandStrategy::Explore:
		return {
			0.15, 0.25,
			
			0.55,

			0.1, 0.1, 0.15, 0.25, 0.15, 0.15, 0.1
		};

	case IslandStrategy::Struct:
		return {
			0.25, 0.60, 
			
			0.7,

			0.1, 0.05, 0.2, 0.2, 0.15, 0.2, 0.1
		};
	}
}