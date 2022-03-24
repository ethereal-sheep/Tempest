#pragma once

#include "Core.h"
#include "Util/Service.h"
#include "Util/thread_pool.h"
#include "Animation/AnimationManager.h"
#include "Animation/Animator.h"

namespace Tempest
{
	class AnimMultithreadHelper
	{
		void run_update()
		{
			while (!dead) 
			{
				if (run_once)
				{
					auto v =  buffer.AsyncUpdateAnimations(member_dt);
					
					bool still_running = true;
					while (still_running)
					{
						still_running = false;
						for (auto& f : v)
						{
							if (f.wait_for(std::chrono::seconds(0)) != std::future_status::ready)
							{
								still_running = true;
								break;
							}

						}
					}

					run_once = false;
				}
			}
		}

	public:
		void init()
		{
			auto func = [&]()
			{
				run_update();
			};

			dead = false;
			Service<thread_pool>::Get().push_task(func);
		}

		void shutdown()
		{
			dead = true;
		}

		// let current update the next one
		void step(float dt)
		{
			// if current not done
			if (!run_once)
			{
				current = buffer;

				member_dt = acc_dt + dt;
				acc_dt = 0.f;

				// update buffer
				// check if queue or vector got stuff
				// then add
				for (auto& i : store)
				{
					if(!buffer.CheckAnimator(i.first))
						buffer.AddAnimator(i.first, std::move(i.second));
				}

				for (auto& [id, anim] : chg_animation)
				{
					buffer.ChangeAnimation(id, anim);
				}

				for(auto& [id, time] : chg_duration)
				{
					buffer.ChangeDuration(id, time);
				}

				store.clear();
				chg_animation.clear();
				chg_duration.clear();

				run_once = true;
			}
			else
			{
				acc_dt += dt;
			}
		}

		// get previous
		AnimationManager& get()
		{
			return current;
		}

		void AddAnimator(uint32_t id, Animator anim)
		{
			if(!current.CheckAnimator(id) && !store.contains(id))
				store.insert(std::make_pair(id, anim));
		}

		void ChangeAnimation(uint32_t id, Animation* animation)
		{
			chg_animation.insert(std::make_pair(id, animation));
		}

		bool CheckAnimator(uint32_t id)
		{
			return current.CheckAnimator(id); // || chg_animation.contains(id) || chg_duration.contains(id);
		}

		void ChangeDuration(uint32_t id, float duration)
		{
			chg_duration.insert(std::make_pair(id, duration));
		}
		

		AnimationManager buffer;
		AnimationManager current;
		tmap<uint32_t, Animator> store;
		tmap<uint32_t, Animation*> chg_animation;
		tmap<uint32_t, float> chg_duration;

		std::atomic_bool run_once;
		std::atomic_bool dead;
		float acc_dt = 0.f;
		std::atomic<float> member_dt;
	};
}