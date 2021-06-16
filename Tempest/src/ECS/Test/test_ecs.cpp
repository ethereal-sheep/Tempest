#include "test_entity.h"
#include "ECS/ECS.h"
#include "Util/thread_pool.h"
#include "Util/timer.h"

namespace Tempest
{
	void testing_ecs_1()
	{
		debug_mr dg;
		ECS ecs(&dg);
		thread_pool tp(std::thread::hardware_concurrency());
		tp.sleep_duration = 0;

		auto a = 0.;
		auto print = [&](Entity entity)
		{
			// data race but whatever
			a = pow(entity, entity);

		};

		auto t = 50000;
		for (auto i = 0; i < t; ++i)
		{
			auto entity = ecs.create();
			auto meta = ecs.emplace<tc::Meta>(entity);
			meta->name = "Entity ";
			meta->name += std::to_string(i);
		}

		auto view = ecs.view<tc::Meta>();

		auto single_task = [&](auto i)
		{
			for (auto& e : view)
				print(e);

			LOG("Task {0} complete.", i);
		};
		auto multi_task = [&]()
		{
			for (auto& e : view)
				tp.push_task(print, e);
		};

		timer time;

		LOG("Please wait: ");

		tp.push_task(single_task, 1);
		tp.push_task(single_task, 2);
		tp.push_task(single_task, 3);
		single_task(4);

		tp.wait_for_tasks();

		LOG("Time elapsed: {0}", time.elapsed<float, time::mcs>());
	}

	void testing_ecs()
	{
		testing_ecs_1();
	}
}