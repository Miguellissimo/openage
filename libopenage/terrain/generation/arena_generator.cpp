
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

Region player_area(int player_owner) {
    tileset_t rect;

    for (auto ne = 20; ne != 31; ++ne) {
        for (auto se = 5; se != 16; ++se) {
            rect.insert({ne, se});
        }
    }

    Region area(rect);
    for (auto tile : area) {
        area.tile_types[tile] = terrain_type::DRYED_GRAS;
    }

    // place stone
    area.units[{22, 6}] = {player_owner, 102};
    area.units[{23, 6}] = {player_owner, 102};
    area.units[{23, 7}] = {player_owner, 102};

    // place gold
    area.units[{29, 12}] = {player_owner, 66};
    area.units[{28, 12}] = {player_owner, 66};
    area.units[{30, 11}] = {player_owner, 66};
    area.units[{27, 13}] = {player_owner, 66};
    area.units[{28, 13}] = {player_owner, 66};

    // place sheeps
    area.units[{23, 13}] = {player_owner, 594};
    area.units[{24, 12}] = {player_owner, 594};

    // place TC and villagers
    area.units[{25, 7}] = {player_owner, 109, true};
    area.units[{22, 9}] = {player_owner, 83};
    area.units[{29, 11}] = {player_owner, 293};

    return area;
}

std::vector<std::pair<const coord::tile, PlaceableUnit>> generate_arena_map(std::shared_ptr<GameSpec> spec, std::shared_ptr<Terrain> terrain, int size, climate c) {

    // gather available tiles due to climate
    terrain_tiles_for_climate tt = get_climate_tiles(c);

    // apply hill algorithm and generate heightmap
    heightmap_t generated_map = hill_generation(size, 15);
    to_file(generated_map, size);

    // calculate rectangle place for player regions
    //todo
    Region p1 = player_area(1);

    // create forrest region
    Region forrest = Region::create_from(generated_map, size, 0.1f, terrain_type::FORREST);
    forrest.populate(349, 1.0f);

    // create base region
    Region base = Region::create_from(generated_map, size, 0.1f, 1.0f, terrain_type::GRAS);
    base.populate(349, 0.05f);

    Region final = Region::merge(forrest, p1);
    std::cout << "final region: " << final.size() << std::endl;
    
    // store placeable units
    std::vector<std::pair<const coord::tile, PlaceableUnit>> vec_pu;
    std::copy(p1.units.begin(), 
        p1.units.end(),
        std::back_inserter(vec_pu)); 

    final.convert_to_terrain(terrain);

    // cleanup generated map array
    for(auto i = 0; i != size; ++i) {
        delete [] generated_map[i];
    }
    delete [] generated_map;

    return vec_pu;
}


}
