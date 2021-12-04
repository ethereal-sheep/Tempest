#include "Scene.h"

namespace Tempest
{
	void Map::update()
	{
		// just to clean up
		// might be sufficient
		collision_map.clear();
		tile_map.clear();
		wall_map.clear();
		for (auto& [id, pf] : prefabs)
		{
			if (pf.has<tc::Transform>() && pf.has<tc::Shape>())
			{
				auto shape = pf.get_if<tc::Shape>();
				auto transform = pf.get_if<tc::Transform>();
				const int& x = shape->x;
				const int& y = shape->y;
				if (pf.has<tc::Tile>() || pf.has<tc::Collision>())
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
						}
				}
				else if (pf.has<tc::Wall>())
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

					wall_map[a_x][a_y][b_x][b_y] = id;
					wall_map[b_x][b_y][a_x][a_y] = id;
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