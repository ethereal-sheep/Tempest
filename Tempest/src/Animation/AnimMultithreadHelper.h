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

				for(auto& [id, anim] : force)
				{
					buffer.ForceChange(id, anim);
				}

				for (auto& [id, loop] : play)
					buffer.PlayAnimation(id, loop);

				for (auto& i : pause)
					buffer.PauseAnimation(i);

				for (auto& i : stop)
					buffer.StopAnimation(i);

				for (auto& [id, spd] : speed)
					buffer.SetSpeed(id, spd);

				store.clear();
				chg_animation.clear();
				chg_duration.clear();
				play.clear();
				pause.clear();
				stop.clear();
				speed.clear();

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
			if(!current.CheckAnimator(id) && !store.count(id))
				store.insert(std::make_pair(id, anim));
		}

		void ChangeAnimation(uint32_t id, std::string anim)
		{
			chg_animation.insert(std::make_pair(id, anim));
		}

		bool CheckAnimator(uint32_t id)
		{
			return current.CheckAnimator(id); // || chg_animation.contains(id) || chg_duration.contains(id);
		}

		void ChangeDuration(uint32_t id, float duration)
		{
			chg_duration.insert(std::make_pair(id, duration));
		}

		void PlayAnimation(uint32_t id, bool loop)
		{
			play.insert(std::make_pair(id, loop));
		}

		void PauseAnimation(uint32_t id)
		{	
			pause.push_back(id);
		}

		void StopAnimation(uint32_t id)
		{
			stop.push_back(id);
		}
		
		void SetSpeed(uint32_t id, float spd)
		{
			speed.insert(std::make_pair(id, spd));
		}

		void ForceChange(uint32_t id, std::string anim)
		{
			force.insert(std::make_pair(id, anim));
		}

		AnimationManager buffer;
		AnimationManager current;

		tmap<uint32_t, Animator> store;
		tmap<uint32_t, std::string> chg_animation;
		tmap<uint32_t, float> chg_duration;
		tmap<uint32_t, float> speed;
		tmap<uint32_t, bool> play;
		tvector<uint32_t> pause;
		tvector<uint32_t> stop;
		tmap<uint32_t, std::string> force;

		std::atomic_bool run_once;
		std::atomic_bool dead;
		float acc_dt = 0.f;
		std::atomic<float> member_dt;
	};
}