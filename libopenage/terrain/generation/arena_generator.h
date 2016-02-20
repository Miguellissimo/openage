
#pragma once

#include "../terrain.h"
#include "terrain_generation.h"
#include "../../game_spec.h"

namespace openage {

std::vector<std::pair<const coord::tile, PlaceableUnit>> generate_arena_map(std::shared_ptr<GameSpec> spec,
	std::shared_ptr<Terrain> terrain, 
	int size, 
	climate c);


}
