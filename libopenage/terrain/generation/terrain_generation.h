#pragma once

#include <unordered_set>
#include <vector>
#include "../../coord/decl.h"

namespace openage {

/**
* the type to store a set of tiles
*/
using tileset_t = std::unordered_set<coord::tile>;

enum class climate { SNOW, TROPICAL, DESERT, NORMAL };

enum terrain_type {
	GRAS,
	WATER,
	BEACH,
	DRYED_GRAS,
	REED, 
	SOIL,
	FARM,
	FARM_EXHAUSTED,
	DIRTY_GRAS,
	FORREST,
	DRYED_GRAS2
};

struct terrain_tiles_for_climate {
    std::vector<terrain_type> base;
    std::vector<terrain_type> beach;
    std::vector<terrain_type> ground_variance;
    std::vector<terrain_type> trees;
    std::vector<terrain_type> animals;
};

terrain_tiles_for_climate get_climate_tiles(climate c);


}
