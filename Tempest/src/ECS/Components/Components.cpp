/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "Components.h"

namespace Tempest::Components
{
	void Door::update(float dt)
	{
		if (next != prev)
		{
			if (current_time < interpolation_time)
			{
				current_time += dt;
				auto t = glm::backEaseOut(glm::clamp(current_time / interpolation_time, 0.f, 1.f));

				auto& t1 = states[(int)prev];
				auto& t2 = states[(int)next];

				// set rotation
				auto slerped = glm::slerp(t1.local_rotation, t2.local_rotation, t);
				current_local.local_rotation = slerped;

				auto lerped = glm::mix(t1.local_position, t2.local_position, t);
				current_local.local_position = lerped;

				auto scaled = glm::mix(t1.local_scale, t2.local_scale, t);
				current_local.local_scale = scaled;
			}
			else
			{
				prev = next;
			}
		}
		else
		{
			current_local = states[(int)prev];
		}
	}
	bool Door::change_state(State new_state)
	{
		if (next == prev)
		{
			current_time = 0.f;
			next = new_state;
			return true;
		}
		return false;
	}

	void Unit::update(float dt)
	{
		if (moving)
		{
			if (current_time < interpolation_time)
			{
				end_frame = false;

				current_time += dt;
				auto a = glm::clamp(current_time / interpolation_time, 0.f, 1.f);
				auto t = glm::quarticEaseOut(a);

				auto& t1 = prev_xform;
				auto& t2 = next_xform;

				// set rotation
				auto slerped = glm::slerp(t1.rotation, t2.rotation, t);
				curr_xform.rotation = slerped;

				auto lerped = glm::mix(t1.position, t2.position, t);
				curr_xform.position = lerped;
				curr_xform.position.y = abs(sin(t*glm::pi<float>()));
			}
			else if(path.size())
			{
				current_time = 0.f;

				prev_xform = next_xform;

				auto& v1 = path[path.size() - 1];

				next_xform.position.x = v1.x + .5f;
				next_xform.position.z = v1.y + .5f;

				path.pop_back();
				end_frame = true;
			}
			else
			{
				path.clear(); 
				moving = false;
				end_frame = false;
			}
		}
		else
		{
			end_frame = false;
		}
	}

	bool Unit::set_path(const tvector<glm::ivec2>& p, const Transform& curr)
	{
		if (!moving)
		{
			path = p;
			moving = true;
			interpolation_time = 0.2f;
			current_time = interpolation_time;
			next_xform = curr;
			return true;
		}
		return false;
	}
}