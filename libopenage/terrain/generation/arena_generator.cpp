
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

std::vector<std::pair<const coord::tile, PlaceableUnit>> generate_arena_map(std::shared_ptr<GameSpec> spec, std::shared_ptr<Terrain> terrain, int size, climate c) {

    // gather available tiles due to climate
    terrain_tiles_for_climate tt = get_climate_tiles(c);

    // apply hill algorithm and generate heightmap
    heightmap_t generated_map = hill_generation(size, 15);
    to_file(generated_map, size);

    // calculate rectangle place for player regions
    //todo

    // create forrest region
    Region forrest = Region::create_from(generated_map, size, 0.1f, terrain_type::FORREST);
    forrest.populate(349, 1.0f);

    // create base region
    Region base = Region::create_from(generated_map, size, 0.1f, 1.0f, terrain_type::GRAS);
    base.populate(349, 0.05f);

    Region final = Region::merge(base, forrest);
    std::cout << "final region: " << final.size() << std::endl;
    
    // store placeable units
    std::vector<std::pair<const coord::tile, PlaceableUnit>> vec_pu;
    std::copy(final.units.begin(), 
        final.units.end(),
        std::back_inserter(vec_pu)); 

    //auto merged = Region::merge(forrest, base);
    final.convert_to_terrain(terrain);

    // cleanup generated map array
    for(auto i = 0; i != size; ++i) {
        delete [] generated_map[i];
    }
    delete [] generated_map;

    return vec_pu;
}


}
