#pragma once

#include "Core.h"
#include "Util/Service.h"
#include "Util/thread_pool.h"

namespace Tempest
{
	class Animator {};

	struct AnimationManager {
	
		void update(float dt)
		{

		}
	
	};

	class AnimMultithreadHelper
	{
		void run_update()
		{
			while (run_once)
			{
				buffer.update(member_dt);
				run_once = false;
			}
		}

		void init()
		{
			auto func = [&]()
			{
				run_update();
			};

			Service<thread_pool>::Get().push_task(func);
		}


		// let current update the next one
		void step(float dt)
		{
			if (run_once == false)
			{
				current = buffer;
				member_dt = dt;
				run_once = true;
			}
		}

		// get previous
		Animator& get_animator(auto handler)
		{

		}

		auto create_animator(string animation, bool loop)
		{
			if (run_once == false)
			{

			}
		
		}

		AnimationManager buffer;
		AnimationManager current;

		std::atomic_bool run_once;
		std::atomic<float> member_dt;
	};
}