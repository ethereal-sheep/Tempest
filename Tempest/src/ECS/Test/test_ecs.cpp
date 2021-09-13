#include "test_entity.h"
#include "ECS/ECS.h"
#include "Util/thread_pool.h"
#include "Util/timer.h"
#include "Util/Service.h"

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
		ECS ecs;
		int t = 10;
		while (t--)
		{
			auto id = ecs.create();
			auto trf = ecs.emplace<tc::Transform>(id);
			trf->position = {};
			trf->rotation = quat{ glm::vec3{1.f} };
		}

	}

	void testing_ecs_3()
	{

		//const char* s = R"(S:\Development\Projects)";
		//string newStat = "Hello";
		//int newStatVal = 10;
		//Entity global = 0, i = 0;
		//{
		//	ECS ecs;
		//	global = ecs.create();
		//	i = ecs.create();
		//	auto statline = ecs.emplace<tc::Statline>(global);
		//	auto character = ecs.emplace<tc::Character>(i);

		//	statline->add_stat(newStat);
		//	character->set_stat(statline->index_of_stat(newStat), newStatVal);
		//	ecs.save(s);
		//}
		//{
		//	ECS ecs;
		//	ecs.load(s);
		//	LOG_ASSERT(ecs.has<tc::Statline>(global));
		//	LOG_ASSERT(ecs.has<tc::Character>(i));


		//	auto statline = ecs.get_if<tc::Statline>(global);
		//	//auto character = ecs.get_if<tc::Character>(i);

		//	LOG_ASSERT(statline->exist(newStat));
		//	LOG_ASSERT(statline->exist(newStat));



		//}
	}
	void testing_ecs_4()
	{
		/*const char* s = R"(S:\Development\Projects)";
		string newStat = "four";
		string newStat2 = "five";
		int newStatVal = 10;
		int newStatVal2 = 10;
		Entity global = 0, i = 0;

		{
			ECS ecs;
			global = ecs.create();
			i = ecs.create();
			auto statline = ecs.emplace<tc::Statline>(global);
			auto character = ecs.emplace<tc::Character>(i);

			statline->add_stat(newStat);
			statline->add_stat(newStat2);

			character->set_stat(statline->index_of_stat(newStat), newStatVal);
			character->set_stat(statline->index_of_stat(newStat2), newStatVal2);

			ecs.save(s);
		}
		{
			ECS ecs;
			ecs.load(s);
			LOG_ASSERT(ecs.has<tc::Statline>(global));
			LOG_ASSERT(ecs.has<tc::Character>(i));


			auto statline = ecs.get_if<tc::Statline>(global);
			auto character = ecs.get_if<tc::Character>(i);

			LOG_ASSERT(statline->exist(newStat));
			LOG_ASSERT(statline->exist(newStat2));
			LOG_ASSERT(character->get_stat(statline->index_of_stat(newStat)) == newStatVal);
			LOG_ASSERT(character->get_stat(statline->index_of_stat(newStat2)) == newStatVal2);

		}
*/

	}

	void testing_ecs_5()
	{
		//const char* s = R"(S:\Development\Projects)";
		/*string newStat = "four";
		string newStat2 = "five";
		int newStatVal = 10;
		int newStatVal2 = 2321;
		Entity global = 0, i = 0;

		{
			ECS ecs;
			global = ecs.create();
			i = ecs.create();
			auto statline = ecs.emplace<tc::Statline>(global);
			auto character = ecs.emplace<tc::Character>(i);

			statline->add_stat(newStat);
			statline->add_stat(newStat2);

			character->set_stat(statline->index_of_stat(newStat), newStatVal);
			character->set_stat(statline->index_of_stat(newStat2), newStatVal2);

			character->remove_stat(statline->index_of_stat(newStat));
			statline->remove_stat(newStat);

			LOG_ASSERT(statline->size() == 4);
			LOG_ASSERT(character->get_stats()[3] == newStatVal2);
			LOG_ASSERT(statline->get_stats()[3] == newStat2);

		}*/
	}
	void testing_ecs_6()
	{
		/*const char* s = R"(S:\Development\Projects)";
		string newStat = "four";
		string newStat2 = "five";
		int newStatVal = 10;
		int newStatVal2 = 2321;
		Entity global = 0, i = 0, k = 0;

		{
			ECS ecs;
			global = ecs.create();
			i = ecs.create();
			k = ecs.create();
			auto statline = ecs.emplace<tc::Statline>(global);
			auto character = ecs.emplace<tc::Character>(i);
			auto weapon = ecs.emplace<tc::Weapon>(k);

			statline->add_stat(newStat);
			statline->add_stat(newStat2);

			character->set_stat(statline->index_of_stat(newStat), newStatVal);
			character->set_stat(statline->index_of_stat(newStat2), newStatVal2);
			weapon->set_stat(statline->index_of_stat("ATK"), newStatVal);

			character->remove_stat(statline->index_of_stat(newStat));
			character->weapon = k;

			auto calc_char_main_stat = [&]() {
				auto& weapon = ecs.get<tc::Weapon>(character->weapon);
				auto main = weapon.get_main_stat();

				return character->get_stat(main) + weapon.get_stat(main);
			};

			auto calc_char_some_stat = [&](const string& stat) {

				auto index = statline->index_of_stat(stat);
				auto& weapon = ecs.get<tc::Weapon>(character->weapon);

				return character->get_stat(index) + weapon.get_stat(index);
			};

			LOG_ASSERT(calc_char_main_stat() == newStatVal);
			LOG_ASSERT(calc_char_some_stat("ATK") == newStatVal);

			ecs.save(s);
		}*/
	}

	void testing_ecs_7()
	{
		//const char* s = R"(S:\Development\Projects)";
		//Entity global = 0;

		//{
		//	ECS ecs;
		//	global = ecs.create();
		//	auto sys = ecs.emplace<tc::ActionGraph>(global);
		//	sys->g.add_var("Test", pin_type::Bool);
		//	ecs.save(s);
		//}

		//{
		//	ECS ecs;
		//	ecs.load(s);
		//	auto sys = ecs.get_if<tc::ActionGraph>(global);
		//	LOG_ASSERT(sys);

		//	//LOG_ASSERT(sys->g.get_variables().size());
		//	//LOG_ASSERT(sys->g.get_links().size());
		//	//LOG_ASSERT(sys->g.get_nodes().size());

		//}
	}

	void testing_ecs()
	{
		testing_ecs_1();
		testing_ecs_3();
		testing_ecs_4();
		testing_ecs_5();
		testing_ecs_6();
		testing_ecs_7();
	}
}