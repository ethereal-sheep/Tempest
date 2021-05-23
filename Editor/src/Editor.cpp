
#define _CRT_SECURE_NO_WARNINGS

#include "ECS/Components/Components.h"
#include "ECS/Systems/Systems.h"
#include "ECS/ECS.h"
#include "Util.h"

#include "Memory/Test/mem_test.h"
#include "Memory/MemoryManager.h"

#include <iostream>
#include <thread>

using namespace std;
using namespace Tempest;


int main()
{
	tmem::MemoryManager mm;
	m_resource* mr = mm.request({true, tmem::ResourceStrategy::MONOTONIC});

	{
		Tempest::ECS ecs(mr);
		// init
		{
			ecs.register_component<tc::Transform>();
			ecs.register_component<tc::Meta>();
			ecs.register_component<tc::Relationship>();
			ecs.register_component<int>();
			ecs.register_component<double>();
			ecs.register_component<unsigned>();

			// create 10 entities
			int entity_num = 10000;

			cout << ecs.size() << endl;

			for (int i = 0; i < entity_num; ++i)
			{
				auto id = ecs.create();
				auto meta = ecs.emplace<tc::Meta>(id);
				meta->name = std::to_string(i) + " indexed";
				auto transform = ecs.emplace<tc::Transform>(id);
				transform->position = els::vec3f(1.f, 0.f, 1.f);
			}
		}

		// gameloop
		{
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
					transform->position += els::vec3f::i + els::vec3f::k;
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
			std::pmr::vector<int> c(1,0, mr);
			auto count = [&](auto id) {
				++c[0];
			};

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
			//test_thread_1();
			test_thread_2();

		}

		{
			// exit
		}
	}

	if (auto debug_mr = mm.get_debug_resource())
		output_debug_mr(debug_mr);

	mm.reclaim();
}