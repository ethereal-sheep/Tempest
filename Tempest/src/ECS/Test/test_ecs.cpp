#include "test_entity.h"
#include "ECS/ECS.h"
#include "Util/thread_pool.h"
#include "Util/timer.h"
#include "Util/Service.h"
#include 

namespace Tempest
{
	void testing_ecs_1()
	{
		debug_mr dg;
		ECS ecs(&dg);

		auto& tp = Service<thread_pool>::Get();
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
		auto future = tp.submit_task(single_task, 4);

		while (future.wait_for(time::mcs(100)) == std::future_status::timeout);

		tp.wait_for_tasks();

		LOG("Time elapsed: {0}", time.elapsed<float, time::mcs>());
	}

	void testing_ecs_2()
    {
        struct rigidbody
        {
            // properly defined based on component
            int member = 0;
        };
        {
            debug_mr dg("testing_ecs_2.1");
            dg.set_strict(true);

            ECS ecs(&dg);


            auto t = 100;
            for (auto i = 0; i < t; ++i)
            {
                auto entity = ecs.create();
                auto* rb = ecs.emplace<rigidbody>(entity);
                rb->member = i;
            }

            ecs.save("empty path");
        }

        {
            debug_mr dg("testing_ecs_2.2");
            dg.set_strict(true);

            ECS ecs(&dg);

            ecs.load("empty path");

            auto view = ecs.view<rigidbody>();
            LOG_ASSERT(view.size_hint() == 100);

            for (auto id : view)
            {
                LOG_ASSERT(id == ecs.get<rigidbody>(id).member - 1);
            }

        }



    }

	void testing_ecs()
	{
		testing_ecs_2();
	}
}