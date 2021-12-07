/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#include "Core.h"

namespace Tempest
{

	inline std::tuple<int, int, int, int, float, float, float, float> shape_data_from_position(int x, int y, float x_pos, float y_pos)
	{
		int a_x = x, a_y = y, e_x = 0, e_y = 0;

		if (a_x + a_y <= 1)
		{
			float p_x = a_x % 2 ? std::floor(x_pos) + .5f : std::round(x_pos);
			float p_y = a_y % 2 ? std::floor(y_pos) + .5f : std::round(y_pos);

			x_pos = a_x % 2 ? std::floor(x_pos) : std::round(x_pos);
			y_pos = a_y % 2 ? std::floor(y_pos) : std::round(y_pos);

			return std::make_tuple(a_x, a_y, e_x, e_y, x_pos, y_pos, p_x, p_y);
		}
		else
		{
			if (a_x % 2 != a_y % 2)
			{
				a_x = a_y = std::min(x, y);
				e_x = x - a_x;
				e_y = y - a_y;
			}
			x_pos = a_x % 2 ? std::floor(x_pos) + .5f : std::round(x_pos);
			y_pos = a_y % 2 ? std::floor(y_pos) + .5f : std::round(y_pos);

			return std::make_tuple(a_x, a_y, e_x, e_y, x_pos, y_pos, x_pos, y_pos);
		}
	}

	inline std::tuple<float, float, float, float> shape_bounding_with_position(int x, int y, float x_pos, float y_pos, float offset = .5f, float min_offset = .1f)
	{
		auto [a_x, a_y, e_x, e_y, o_x, o_y, p_x, p_y] = shape_data_from_position(x, y, x_pos, y_pos);

		float a, b, c, d;

		a = p_x + -offset - (a_x - 1) / 2.f;
		b = p_y + -offset - (a_y - 1) / 2.f;

		c = p_x + offset + (a_x - 1) / 2.f + e_x;
		d = p_y + offset + (a_y - 1) / 2.f + e_y;

		if (abs(a - c) < 0.001f)
		{
			a -= min_offset;
			c += min_offset;
		}
		if (abs(b - d) < 0.001f)
		{
			b -= min_offset;
			d += min_offset;
		}
		return std::make_tuple(a, b, c, d);
	}

	inline std::tuple<float, float, float, float> shape_bounding_for_rotation(int x, int y, float offset = .5f, float min_offset = .1f)
	{
		int a_x = x, a_y = y, e_x = 0, e_y = 0;
		if (a_x % 2 != a_y % 2)
		{
			a_x = a_y = std::min(x, y);
			e_x = x - a_x;
			e_y = y - a_y;
		}
		float a, b, c, d;

		a = -offset - (a_x - 1) / 2.f;
		b = -offset - (a_y - 1) / 2.f;

		c = offset + (a_x - 1) / 2.f + e_x;
		d = offset + (a_y - 1) / 2.f + e_y;

		if (abs(a - c) < 0.001f)
		{
			a -= min_offset;
			c += min_offset;
		}
		if (abs(b - d) < 0.001f)
		{
			b -= min_offset;
			d += min_offset;
		}
		return std::make_tuple(a, b, c, d);
	}

	inline std::tuple<float, float, float, float> shape_bounding(int x, int y, float offset = .5f, float min_offset = .1f)
	{
		return shape_bounding_with_position(x, y, 0, 0, offset, min_offset);
	}




	
}