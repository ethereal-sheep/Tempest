
#define _CRT_SECURE_NO_WARNINGS

#include "ECS/Components/Components.h"
#include "ECS/Systems/Systems.h"
#include "ECS/ECS.h"
#include "Util.h"

#include "Memory/Test/mem_test.h"
#include "Memory/MemoryManager.h"

#include <iostream>
#include <thread>

#include "Log/Log.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/async.h"
#include "spdlog/sinks/msvc_sink.h"
#include "spdlog/async.h"
#include "spdlog/sinks/dist_sink.h"
#include "spdlog/sinks/basic_file_sink.h"

using namespace std;
using namespace Tempest;





void test1()
{
	tmem::MemoryManager mm;
	m_resource* mr = mm.request({ true, tmem::ResourceStrategy::MONOTONIC });

	{
		Tempest::ECS ecs(mr);
		// init
		{
			// create 10 entities
			int entity_num = 10000;

			cout << ecs.size() << endl;

			for (int i = 0; i < entity_num; ++i)
			{
				auto id = ecs.create();
				ecs.emplace<tc::Example>(id);
			}
		}

		// gameloop

		{
			auto change_member1 = [&](auto id) {
				if (auto eg = ecs.get<tc::Example>(id))
					++eg->member1;
			};
			auto change_member2 = [&](auto id) {
				if (auto eg = ecs.get<tc::Example>(id))
					++eg->member2;
			};
			auto change_member3 = [&](auto id) {
				if (auto eg = ecs.get<tc::Example>(id))
					++eg->member3;
			};
			auto change_member4 = [&](auto id) {
				if (auto eg = ecs.get<tc::Example>(id))
					++eg->member4;
			};

			auto change_name1 = [&](auto id) {
				if (auto meta = ecs.get<tc::Meta>(id))
					meta->name = "Hello";
			};
			auto change_name2 = [&](auto id) {
				if (auto meta = ecs.get<tc::Meta>(id))
					meta->name = "Oops";
			};

			auto change_pos1 = [&](auto id) {
				if (auto transform = ecs.get<tc::Transform>(id))
					transform->position += els::random::ball_rand(1);
			};
			auto change_pos2 = [&](auto id) {
				if (auto transform = ecs.get<tc::Transform>(id))
					transform->position *= 2;
			};
			auto print_name = [&](auto id) {
				cout << id << ": ";
				if (auto meta = ecs.get<tc::Meta>(id))
				{
					cout << meta->name << endl;
				}
				else
					cout << "INVALID" << endl;
			};
			auto print_eg = [&](auto id) {
				cout << id << ": ";
				if (auto eg = ecs.get<tc::Example>(id))
				{
					cout << eg->member1 << "," << eg->member2 << endl;
				}
				else
					cout << "INVALID" << endl;
			};

			auto print_pos = [&](auto id) {
				cout << id << ": ";
				if (auto transform = ecs.get<tc::Transform>(id))
				{
					cout << "{";
					cout << transform->position.x << ",";
					cout << transform->position.y << ",";
					cout << transform->position.z;
					cout << "}" << endl;
				}
				else
					cout << "INVALID" << endl;
			};

			auto print = [&](auto id) {
				auto meta = ecs.get<tc::Meta>(id);
				auto transform = ecs.get<tc::Transform>(id);
				if (meta && transform)
				{
					printf_s(
						"%s %lu: { %.2f, %.2f, %.2f }\n",
						meta->name.c_str(),
						id,
						transform->position.x,
						transform->position.y,
						transform->position.z);

					/*cout << meta->name << " " << id << ": ";
					cout << "{";
					cout << transform->position.x << ",";
					cout << transform->position.y << ",";
					cout << transform->position.z;
					cout << "}" << endl;*/
				}
				else
					cout << "INVALID" << endl;
			};


			std::pmr::vector<int> c(1, 0, mr);
			auto count = [&](auto) {
				++c[0];
			};

			auto example_view = ecs.view<tc::Example>();
			auto meta_view = ecs.view<tc::Meta>();
			auto transform_view = ecs.view<tc::Transform>();
			auto transform_meta_view = ecs.view<tc::Transform, tc::Meta>();

			auto data_race = [&]()
			{
				// data race
				std::thread t1;
				std::thread t2;

				t1 = std::thread(ts::dynamic_system{}, transform_view, count);
				t2 = std::thread(ts::dynamic_system{}, meta_view, count);

				t1.join();
				t2.join();

				cout << "Count: " << c[0] << endl;
			};

			auto data_race2 = [&]()
			{
				// check data race

				std::thread t1;
				std::thread t2;
				auto duration = std::chrono::high_resolution_clock().now().time_since_epoch();

				int t = 10;
				while(t--)
				{
					auto test_1 = [&]() {
						for (auto entity : example_view)
							change_member1(entity);
					};
					auto test_2 = [&]() {
						for (auto entity : example_view)
							change_member2(entity);
					};

					//t1 = std::thread(test_1);
					test_1();
					test_2();
					//t1.join();
				}

				cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock().now().time_since_epoch() - duration).count() << endl;

				//ts::dynamic_system{}(example_view, print_eg);

			};

			auto test_thread_1 = [&]()
			{
				// no data race
				std::thread t1;
				std::thread t2;

				t1 = std::thread(ts::dynamic_system{}, transform_view, change_pos1);
				t2 = std::thread(ts::dynamic_system{}, meta_view, change_name1);

				t1.join();
				t2.join();

				t1 = std::thread(ts::dynamic_system{}, transform_view, change_pos2);
				t2 = std::thread(ts::dynamic_system{}, meta_view, change_name2);

				t1.join();
				t2.join();

				ts::dynamic_system{}(transform_meta_view, print);
			};

			auto test_thread_2 = [&]()
			{
				// no data race
				std::thread t1;
				std::thread t2;

				t1 = std::thread(ts::dynamic_system{}, transform_view, count);
				t2 = std::thread(ts::dynamic_system{}, meta_view, change_name1);

				t1.join();
				t2.join();

				t1 = std::thread(ts::dynamic_system{}, meta_view, count);
				t2 = std::thread(ts::dynamic_system{}, transform_view, change_pos2);

				t1.join();
				t2.join();

				//ts::dynamic_system{}(transform_meta_view, print);

				cout << "Count: " << c[0] << endl;
			};


			//data_race();
			data_race2();
			//test_thread_1();
			//test_thread_2();

		}

		{
			// exit
		}
	}

	if (auto debug_mr = mm.get_debug_resource())
		output_debug_mr(debug_mr);

	mm.reclaim();
}

void test2()
{
	tmem::MemoryManager mm;
	m_resource* mr = mm.request({ true, tmem::ResourceStrategy::MONOTONIC });

	{
		Tempest::ECS ecs(mr);
		// init
		{
			// create 10 entities
			int entity_num = 10;
			for (int i = 0; i < entity_num; ++i)
			{
				auto id = ecs.create();
				ecs.emplace<tc::Meta>(id);
				if(i % 2 == 0)
					ecs.emplace<tc::Example>(id);
				if (i % 3 == 0)
					ecs.emplace<tc::Transform>(id);
			}
		}

		// gameloop

		{
			auto change_pos = [&](auto id) {
				if (auto transform = ecs.get<tc::Transform>(id))
					transform->position += els::random::ball_rand(1);
			};


			auto transform_view = ecs.view<tc::Transform>();

			ts::dynamic_system{}(transform_view, change_pos);


			const char* s = R"(S:\Development\Projects)";
			tpath path(s);
			if (std::filesystem::exists(path))
			{
				ecs.save(path);
			}
		}

		{
			// exit
		}
	}

	if (auto debug_mr = mm.get_debug_resource())
		output_debug_mr(debug_mr);

	mm.reclaim();
}

void test3()
{
	tmem::MemoryManager mm;
	m_resource* mr = mm.request({ true, tmem::ResourceStrategy::MONOTONIC });

	{
		Tempest::ECS ecs(mr);
		// init
		{
			const char* s = R"(S:\Development\Projects)";
			tpath path(s);
			ecs.load(path);
		}

		// gameloop

		{
			auto all_view = ecs.view<tc::Meta, tc::Example, tc::Transform>();
			auto meta_view = ecs.view<tc::Meta>();
			auto example_view = ecs.view<tc::Example>();
			auto transform_view = ecs.view<tc::Transform>();


			auto print = [](auto entity) {
				printf_s("%u, ", entity);
			};

			cout << "All Component" << endl;
			all_view.each(print);
			cout << endl;

			cout << "Meta" << endl;
			meta_view.each(print);
			cout << endl;

			cout << "Example" << endl;
			example_view.each(print);
			cout << endl;

			cout << "Transform" << endl;
			transform_view.each(print);
			cout << endl;

		}

		{
			// exit
		}
	}

	if (auto debug_mr = mm.get_debug_resource())
		output_debug_mr(debug_mr);

	mm.reclaim();
}

void test4()
{
	tmem::MemoryManager mm;
	m_resource* mr = mm.request({ false, tmem::ResourceStrategy::MONOTONIC, 1048576 });

	{
		Tempest::ECS ecs(mr);
		// init
		{
			// create 10 entities
			int entity_num = MAX_ENTITY-1;
			for (int i = 0; i < entity_num; ++i)
			{
				auto id = ecs.create();
				ecs.emplace<tc::Example>(id);
			}
		}

		// gameloop

		{

			auto change_member1 = [&](auto id) {
				if (auto eg = ecs.get<tc::Example>(id))
					++eg->member1;
			};
			auto change_member2 = [&](auto id) {
				if (auto eg = ecs.get<tc::Example>(id))
					++eg->member2;
			};
			auto change_member3 = [&](auto id) {
				if (auto eg = ecs.get<tc::Example>(id))
					++eg->member3;
			};
			auto change_member4 = [&](auto id) {
				if (auto eg = ecs.get<tc::Example>(id))
					++eg->member4;
			};

			auto example_view = ecs.view<tc::Example>();

			auto change1 = [&]() {
				for (auto entity : example_view)
					change_member1(entity);
			};
			auto change2 = [&]() {
				for (auto entity : example_view)
					change_member2(entity);
			};
			auto change3 = [&]() {
				for (auto entity : example_view)
					change_member3(entity);
			};
			auto change4 = [&]() {
				for (auto entity : example_view)
					change_member4(entity);
			};

			const int thread_count = 4;
			std::thread threads[thread_count];

			int t = 1;
			auto duration = std::chrono::high_resolution_clock().now().time_since_epoch();
			while (t--)
			{
				// for test aggregation
				/*for (auto entity : example_view)
				{
					change_member1(entity);
					change_member2(entity);
					change_member3(entity);
					change_member4(entity);
				}*/

				// for test exclusive
				/*for (auto entity : example_view)
					change_member1(entity);
				for (auto entity : example_view)
					change_member2(entity);
				for (auto entity : example_view)
					change_member3(entity);
				for (auto entity : example_view)
					change_member4(entity);*/

				// dynamic system
				/*ts::dynamic_system{}(example_view, change_member1);
				ts::dynamic_system{}(example_view, change_member2);
				ts::dynamic_system{}(example_view, change_member3);
				ts::dynamic_system{}(example_view, change_member4);*/

				// lambda
				/*change1();
				change2();
				change3();
				change4();*/

				// multithreading dynamic system
				/*threads[0] = std::thread(ts::dynamic_system{}, example_view, change_member1);
				threads[1] = std::thread(ts::dynamic_system{}, example_view, change_member2);
				threads[2] = std::thread(ts::dynamic_system{}, example_view, change_member3);
				threads[3] = std::thread(ts::dynamic_system{}, example_view, change_member4);
				for (int i = 0; i < thread_count; ++i)
					threads[i].join();*/


				// multithreading lambda
				threads[0] = std::thread(change1);
				threads[1] = std::thread(change2);
				threads[2] = std::thread(change3);
				threads[3] = std::thread(change4);
				for (int i = 0; i < thread_count; ++i)
					threads[i].join();


			}
			cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock().now().time_since_epoch() - duration).count() << endl;
			cout << thread::hardware_concurrency() << endl;
		}

		{
			// exit
		}
	}

	if (auto debug_mr = mm.get_debug_resource())
		output_debug_mr(debug_mr);

	mm.reclaim();
}

int main()
{
	spdlog::init_thread_pool(1024, 2);

	auto dist_sink = make_shared<spdlog::sinks::dist_sink_st>();
	auto sink1 = make_shared<spdlog::sinks::stdout_color_sink_mt>();
	auto sink2 = make_shared<spdlog::sinks::basic_file_sink_mt>("mylog.log");

	std::vector<spdlog::sink_ptr> sinks;
	sinks.push_back(sink1);
	sinks.push_back(sink2);

	auto async_file = spdlog::create_async<spdlog::sinks::dist_sink_mt>("Instance 1", sinks);
	spdlog::set_pattern("[%Y-%m-%d %T][%n][%^%l%$] %v");
	spdlog::set_level(spdlog::level::trace);
	async_file->info("123");
	async_file->info("123"); 
	async_file->flush();
}