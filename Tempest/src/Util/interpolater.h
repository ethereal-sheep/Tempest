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
#include "type_traits.h"
#include "Logger/Log.h"
#include "TMath.h"

namespace Tempest
{
	template<typename T>
	class interpolater
	{
		float curr_time = 0;
		float end_time = -1;
		T start_v = T{};
		T end_v = T{};
		std::function<float(float)> fn = [](float x) { return x; };

	public:
		void start(T start, T end, float duration_in_sec = 1, float delay_in_sec = 0, std::function<float(float)> interpolation = [](float x) { return x; })
		{
			curr_time = -delay_in_sec;
			end_time = duration_in_sec;

			start_v = start;
			end_v = end;

			fn = interpolation;
		}

		bool is_in_progress() const	{ return !is_finished(); }
		bool is_finished() const { return curr_time >= end_time; }

		T get() const
		{
			float clamped = els::clamp(curr_time, 0.f, end_time);
			if (end_time > 0.001)
			{
				clamped /= end_time;
				clamped = fn(clamped);
			}
			else
				clamped = 1;


			return els::lerp(start_v, end_v, clamped);
		}

		T update(float dt)
		{
			if (!is_finished())
			{
				curr_time += dt;
				if (curr_time >= end_time)
					curr_time = end_time;
			}

			return get();
		}
	};
}