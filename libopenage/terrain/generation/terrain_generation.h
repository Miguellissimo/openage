#pragma once

#include <iterator>
#include <unordered_set>
#include <vector>
#include <map>
#include "../../coord/tile.h"
#include "../terrain.h"

namespace openage {

/**
* the type to store a set of tiles
*/
using tileset_t = std::unordered_set<coord::tile>;
using heightmap_t = float **;

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


struct PlaceableUnit {
	int owner_id;
	int unit_id;
};

class TileComparator
{
public:
	bool operator() (const coord::tile &lhs, const coord::tile &rhs) const;
};


class Region {
private:
	tileset_t m_tiles;

public:
	std::map<coord::tile, PlaceableUnit, TileComparator> units;
	std::map<coord::tile, terrain_type, TileComparator> tile_types;

public:
	using iterator = tileset_t::iterator;
    using const_iterator = tileset_t::const_iterator;

	static Region create_from(heightmap_t terrain, int size, float threshold, terrain_type tt = terrain_type::GRAS);
	static Region create_from(heightmap_t terrain, int size, float start, float end, terrain_type tt = terrain_type::GRAS);
	static Region merge(Region foreground, Region background);

	Region() = default;
	Region(tileset_t tiles) : m_tiles(tiles) { };

	Region subset_by_erosion();
	Region subset_by_dilation();

	int size() const;

	coord::tile take_random();
	coord::tile access(int ne, int sw);

	bool has_unit(coord::tile t);
	PlaceableUnit get_unit(coord::tile);
	void assign(coord::tile, PlaceableUnit unit);

	Region join(Region other);
	Region intersection(Region other);
	Region subtract(Region other);

	void populate(int object_id, float percentage, int owner_id = 0);

	void convert_to_terrain(std::shared_ptr<Terrain> terrain);

	iterator begin() { return this->m_tiles.begin(); }
    iterator end() { return this->m_tiles.end(); }
    const_iterator cbegin() const { return this->m_tiles.cbegin(); }
    const_iterator cend() const { return this->m_tiles.cend(); }
};

}
