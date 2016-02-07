
#include "terrain_generation.h"

namespace openage {

terrain_tiles_for_climate get_climate_tiles(climate c) {
    terrain_tiles_for_climate tt;

    switch(c) {
        case climate::SNOW:
        case climate::DESERT:
        case climate::NORMAL:
            tt.base.push_back(terrain_type::GRAS);
            tt.beach.push_back(terrain_type::BEACH);
            tt.ground_variance.push_back(terrain_type::DRYED_GRAS);
            tt.ground_variance.push_back(terrain_type::DRYED_GRAS2);
            tt.ground_variance.push_back(terrain_type::DIRTY_GRAS);
            break;
        case climate::TROPICAL:
        default:
            break;
    }

    return tt;
}


}
