
#include "arena_generator.h"
#include "hill_algorithm.h"
#include <vector>
#include <array>
#include <cmath>
#include <random>
#include <memory>
#include <fstream>
#include <iostream>

namespace openage {

void to_file(float **terrain, int size) {

    std::ofstream out_file;
    out_file.open("generated_map.txt", std::ios::out | std::ios::trunc);
    for (auto row = 0; row != size; ++row) {
        for (auto col = 0; col != size; ++col) {
            out_file << terrain[row][col] << " ";
        }
        out_file << std::endl;
    }
}

std::vector<PlaceableUnit> generate_arena_map(std::shared_ptr<GameSpec> spec, std::shared_ptr<Terrain> terrain, int size, climate c) {

    // gather available tiles due to climate
    terrain_tiles_for_climate tt = get_climate_tiles(c);

    // apply hill algorithm
    float ** generated_map = hill_generation(size, 15);

    // store placeable units
    std::vector<PlaceableUnit> vec_pu;

    // convert matrix to tiles
    for (auto row = 0; row != size; ++row) {
        for (auto col = 0; col != size; ++col) {
            coord::tile current_tile{row, col};
            TerrainChunk *chunk = terrain->get_create_chunk(current_tile);

            if (generated_map[row][col] <= 0.3) {
                chunk->get_data(current_tile)->terrain_id = terrain_type::FORREST;
                vec_pu.push_back({current_tile, 0, 349});
            } else {
                chunk->get_data(current_tile)->terrain_id = tt.base[0];
            }
        }
    }

    // cleanup generated map array
    for(auto i = 0; i != size; ++i) {
        delete [] generated_map[i];
    }
    delete [] generated_map;

    return vec_pu;
}


}
