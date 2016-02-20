
#include "terrain_generation.h"
#include <algorithm>
#include <iterator>
#include <utility>
#include <cmath>
#include <random>
#include <stdexcept>
#include <sstream>
#include <iostream>

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

bool TileComparator::operator() (const coord::tile &lhs, const coord::tile &rhs) const {
    if (lhs.ne < rhs.ne) {
        return true;
    } else if (lhs.ne == rhs.ne and lhs.se < rhs.se) {
        return true;
    } else {
        return false;
    }
}


Region Region::create_from(heightmap_t terrain, int size, float threshold, terrain_type tt) {
    tileset_t tiles;
    std::map<coord::tile, terrain_type, TileComparator> tile_types;

    for (auto row = 0; row != size; ++row) {
        for (auto col = 0; col != size; ++col) {
            if (terrain[row][col] < threshold) {
                auto current_tile = coord::tile{row, col};
                tiles.insert(current_tile);
            }
        }
    }

    Region new_region(tiles);
    for (auto tile : tiles) {
        new_region.tile_types[tile] = tt;
    }
    
    return new_region;
}

Region Region::create_from(heightmap_t terrain, int size, float start, float end, terrain_type tt) {
    tileset_t tiles;
    std::map<coord::tile, terrain_type, TileComparator> tile_types;

    for (auto row = 0; row != size; ++row) {
        for (auto col = 0; col != size; ++col) {
            if (terrain[row][col] >= start and terrain[row][col] <= end) {
                auto current_tile = coord::tile{row, col};
                tiles.insert(current_tile);
            }
        }
    }

    Region new_region(tiles);
    for (auto tile : tiles) {
        new_region.tile_types[tile] = tt;
    }

    return new_region;
}

Region Region::merge(Region foreground, Region background) {
    tileset_t merged_tiles;

    std::map<coord::tile, PlaceableUnit, TileComparator> units;
    std::map<coord::tile, terrain_type, TileComparator> tile_types;

    for (auto tile : foreground) {
        merged_tiles.insert(tile);
        tile_types[tile] = foreground.tile_types[tile];

        if (foreground.units.find(tile) != foreground.units.end()) {
            units[tile] = foreground.units[tile];
        }
    }

    for (auto tile : background) {
        merged_tiles.insert(tile);
        tile_types[tile] = background.tile_types[tile];
        
        if (background.units.find(tile) != background.units.end()) {
            units[tile] = background.units[tile];
        }
    }

    Region new_region(merged_tiles);
    new_region.tile_types.insert(tile_types.begin(), tile_types.end());
    new_region.units.insert(units.begin(), units.end());

    return new_region;
}

int Region::size() const {
    return m_tiles.size();
}

coord::tile Region::take_random() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, m_tiles.size());

    auto it = m_tiles.begin();
    std::advance(it, dis(gen));

    return *it;
}

coord::tile Region::access(int ne, int se) {
    for (auto tile : m_tiles) {
        if (tile.ne == ne and tile.se == se) {
            return tile;
        }
    }
    std::stringstream s_stream;
    s_stream << "The tile (" << ne << ", " << se << ") does not exist" << std::endl; 
    throw std::out_of_range(s_stream.str());
}

bool Region::has_unit(coord::tile t) {
    return units.find(t) != units.end();
}

PlaceableUnit Region::get_unit(coord::tile t) {
    return units.at(t);
}

void Region::populate(int object_id, float percentage, int owner_id) {
    unsigned number_of_units = std::round(this->size() * percentage);

    tileset_t tiles_to_populate;

    std::random_device rd;
    std::mt19937 gen(rd());

    while (tiles_to_populate.size() != number_of_units) {
        std::uniform_int_distribution<> dis(0, m_tiles.size() -1);

        auto it = m_tiles.begin();
        unsigned random_number = dis(gen);

        std::advance(it, random_number);
        tiles_to_populate.insert(*it);
        m_tiles.erase(*it);
    }
    
    for (coord::tile t : tiles_to_populate) {
        PlaceableUnit pu;
        pu.owner_id = owner_id;
        pu.unit_id = object_id;

        this->units.insert(std::make_pair(t, pu));
        m_tiles.insert(t);
    }
}

void Region::convert_to_terrain(std::shared_ptr<Terrain> terrain) {
    for (auto tile : m_tiles) { 
        TerrainChunk *chunk = terrain->get_create_chunk(tile);
        chunk->get_data(tile)->terrain_id = tile_types[tile];
    }
}

}
