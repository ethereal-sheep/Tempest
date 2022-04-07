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

				//auto current_front = next_xform.position - prev_xform.position;
				prev_xform = next_xform;

				auto v1 = path[path.size() - 1];

				next_xform.position.x = v1.x + .5f;
				next_xform.position.z = v1.y + .5f;

				path.pop_back();

				if (path.size())
				{
					auto& next = path[path.size() - 1];


					auto next_front = next - v1;
					auto next_front3 = vec3(next_front.x, 0, next_front.y);

					auto a = glm::normalize(vec3{0, 0, -1});
					auto b = glm::normalize(next_front3);
					auto dot_ab = glm::dot(a, b);
					if (dot_ab * dot_ab < 0.1f)
					{
						glm::quat q;
						auto c = glm::cross(a, b);
						q.x = c.x;
						q.y = c.y;
						q.z = c.z;
						q.w = glm::sqrt(glm::dot(a,a) * glm::dot(b, b)) + dot_ab;

						next_xform.rotation = glm::normalize(q);
					}
					else if(dot_ab < -0.999f)
					{
						next_xform.rotation = glm::rotate(glm::quat_identity<float, glm::highp>(), glm::pi<float>(), vec3{ 0,1,0 });
					}
					else
					{
						next_xform.rotation = glm::quat_identity<float, glm::highp>();
					}


				}


				end_frame = true;
			}
			else
			{
				path.clear(); 
				moving = false;
				end_frame = false;
			}
		}
		else if (attacking)
		{
			if (current_time < interpolation_time)
			{
				current_time += dt;

				end_frame = false;


				Local zero{}, one{}, two{};
				one.local_position = { 0, 0.1, 0.1 };
				one.local_rotation = glm::quat(glm::radians(vec3{ 20, 0, 0 }));
				two.local_position = { 0, 0.1, -0.5 };
				two.local_rotation = glm::quat(glm::radians(vec3{ -17, 0, 0 }));


				auto interpolate = [](Local t1, Local t2, float t)
				{
					Local new_local{};
					// set rotation
					auto slerped = glm::slerp(t1.local_rotation, t2.local_rotation, t);
					new_local.local_rotation = slerped;

					auto lerped = glm::mix(t1.local_position, t2.local_position, t);
					new_local.local_position = lerped;

					return new_local;
				};


				auto a = glm::clamp(current_time / interpolation_time, 0.f, 1.f);

				if (attack_state == 1)
				{
					float t = glm::quarticEaseOut(a);
					curr_local = interpolate(zero, one, t);
				}
				else if (attack_state == 2)
				{
					float t = glm::quarticEaseIn(a);
					curr_local = interpolate(one, two, t);
				}
				else if (attack_state == 3)
				{
					float t = glm::sineEaseOut(a);
					curr_local = interpolate(two, zero, t);
				}
				else if (attack_state == 4)
				{
					float t = glm::sineEaseOut(a);
					curr_local = interpolate(zero, zero, t);
				}
			}
			else if (attack_state < 4)
			{
				if (attack_state == 0)
				{
					interpolation_time = .7f;
				}
				else if (attack_state == 1)
				{
					interpolation_time = .25f;
				}
				else if (attack_state == 2)
				{
					interpolation_time = .2f;
				}
				else if (attack_state == 3)
				{
					interpolation_time = 1.f;
				}

				current_time = 0.f;
				++attack_state;
			}
			else
			{
				curr_local = Local();
				attacking = false;
				end_frame = true;
			}

		}
		else if (getting_hit)
		{
			if (current_time < interpolation_time)
			{
				current_time += dt;
				auto t = current_time / interpolation_time;

				Local zero{}, one{}, two{};
				one.local_position = { 0.0, 0.1, 0.1 };
				one.local_rotation = glm::quat(glm::radians(vec3{ 20, 0, 0 }));
				two.local_position = { -0.0, 0.1, -0.1 };
				two.local_rotation = glm::quat(glm::radians(vec3{ -20, 0, 0 }));


				auto interpolate = [](Local t1, Local t2, float t)
				{
					Local new_local{};
					// set rotation
					auto slerped = glm::slerp(t1.local_rotation, t2.local_rotation, t);
					new_local.local_rotation = slerped;

					auto lerped = glm::mix(t1.local_position, t2.local_position, t);
					new_local.local_position = lerped;

					return new_local;
				};


				auto wobble = [&](float t) // between 0, 1
				{
					auto cos = glm::cos(t * glm::pi<float>() * hit_str); // between -1, 1
					cos *= (1 - t);

					if (cos > 0)
					{
						auto abs = glm::abs(cos);
						curr_local = interpolate(zero, one, abs);
					}
					else
					{
						auto abs = glm::abs(cos);
						curr_local = interpolate(zero, two, abs);
					}
				};

				wobble(t);

				//glm::sine(t);
			}
			else
			{
				curr_local = Local();
				getting_hit = false;
				end_frame = true;
			}
		}
		else
		{
			end_frame = false;
		}
	}

	bool Unit::set_path(const tvector<glm::ivec2>& p, const Transform& curr)
	{
		if (!moving && !attacking && !getting_hit)
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

	bool Unit::attack()
	{
		if (!moving && !attacking && !getting_hit)
		{
			attack_state = 0;
			current_time = 10000.f;
			interpolation_time = 10000.f;
			attacking = true;

			return true;
		}

		return false;
	}
	bool Unit::get_hit(float str, float time)
	{
		if (!moving && !attacking && !getting_hit)
		{
			current_time = 0.f;
			interpolation_time = time;
			getting_hit = true;
			hit_str = str;

			return true;
		}
		return false;
	}
	bool Unit::die()
	{
		dead = true;
		return dead;
	}
	bool Unit::revive()
	{
		dead = false;
		return dead;
	}
}