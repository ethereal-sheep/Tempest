/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "Scene.h"

namespace Tempest
{
	id_t Map::find(int x, int y)
	{
		if (collision_map.count(x) && collision_map[x].count(y) && collision_map[x][y])
			return collision_map[x][y];
		if (deco_map.count(x) && deco_map[x].count(y) && deco_map[x][y])
			return deco_map[x][y];
		if (tile_map.count(x) && tile_map[x].count(y) && tile_map[x][y])
			return tile_map[x][y];
		return INVALID;
	}

	id_t Map::find_edge(int a_x, int a_y, int b_x, int b_y)
	{
		if (wall_map.count(a_x) && wall_map[a_x].count(a_y) && wall_map[a_x][a_y].count(b_x) && wall_map[a_x][a_y][b_x].count(b_y) && wall_map[a_x][a_y][b_x][b_y])
			return wall_map[a_x][a_y][b_x][b_y];
		if (deco_edge_map.count(a_x) && deco_edge_map[a_x].count(a_y) && deco_edge_map[a_x][a_y].count(b_x) && deco_edge_map[a_x][a_y][b_x].count(b_y) && deco_edge_map[a_x][a_y][b_x][b_y])
			return deco_edge_map[a_x][a_y][b_x][b_y];
		if (door_map.count(a_x) && door_map[a_x].count(a_y) && door_map[a_x][a_y].count(b_x) && door_map[a_x][a_y][b_x].count(b_y) && door_map[a_x][a_y][b_x][b_y])
			return door_map[a_x][a_y][b_x][b_y];
		return INVALID;
	}

	void Map::update()
	{
		// just to clean up
		// might be sufficient
		collision_map.clear();
		deco_map.clear();
		tile_map.clear();
		wall_map.clear();
		door_map.clear();
		deco_edge_map.clear();
		for (auto& [id, pf] : prefabs)
		{
			if (pf.has<tc::Transform>() && pf.has<tc::Shape>())
			{
				auto shape = pf.get_if<tc::Shape>();
				auto transform = pf.get_if<tc::Transform>();
				const int& x = shape->x;
				const int& y = shape->y;
				if (x + y >= 2)
				{
					int a_x = x, a_y = y, e_x = 0, e_y = 0;

					if (a_x % 2 != a_y % 2)
					{
						a_x = a_y = std::min(x, y);
						e_x = x - a_x;
						e_y = y - a_y;
					}

					vec3 min, max;

					min.x = -.5f - (a_x - 1) / 2.f;
					min.z = -.5f - (a_y - 1) / 2.f;
					min.y = 0;

					max.x = .5f + (a_x - 1) / 2.f + e_x;
					max.z = .5f + (a_y - 1) / 2.f + e_y;
					max.y = 0;

					auto rot = transform->rotation;
					min = rot * min;
					max = rot * max;


					min += transform->position;
					max += transform->position;

					if (max.x < min.x) std::swap(min.x, max.x);
					if (max.z < min.z) std::swap(min.z, max.z);

					for (int i = (int)std::round(min.x); i < (int)std::round(max.x); ++i)
						for (int j = (int)std::round(min.z); j < (int)std::round(max.z); ++j) {
							if (pf.has<tc::Tile>())
								tile_map[i][j] = id;
							else if (pf.has<tc::Collision>())
								collision_map[i][j] = id;
							else
								deco_map[i][j] = id;
						}
				}
				else if (x + y == 1)
				{
					vec3 s, e;

					e.x = .5f;
					e.z = .5f;

					s.x = y ? -.5f : .5f;
					s.z = x ? -.5f : .5f;

					auto rot = transform->rotation;
					s = rot * s;
					e = rot * e;

					int a_x = (int)std::floor(transform->position.x + s.x);
					int a_y = (int)std::floor(transform->position.z + s.z);
					int b_x = (int)std::floor(transform->position.x + e.x);
					int b_y = (int)std::floor(transform->position.z + e.z);

					if (pf.has<tc::Wall>())
					{
						wall_map[a_x][a_y][b_x][b_y] = id;
						wall_map[b_x][b_y][a_x][a_y] = id;
					}
					else if (pf.has<tc::Door>())
					{
						door_map[a_x][a_y][b_x][b_y] = id;
						door_map[b_x][b_y][a_x][a_y] = id;
					}
					else
					{
						deco_edge_map[a_x][a_y][b_x][b_y] = id;
						deco_edge_map[b_x][b_y][a_x][a_y] = id;
					}


				}
			}
		}
	}

	void Map::verify()
	{
		for (auto& [id, pf] : prefabs)
		{
			if (pf.has<tc::Transform>() && pf.has<tc::Shape>())
			{

			}
		}
	}
}