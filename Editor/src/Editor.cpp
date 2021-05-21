
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

int gameLoop()
{
	return 1;
}

int main()
{
	tmem::MemoryManager mm;
	m_resource* mr = mm.request({true});

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
			for (int i = 0; i < entity_num; ++i)
			{
				auto id = ecs.create();
				auto meta = ecs.emplace<tc::Meta>(id);
				meta->name = std::to_string(i) + " indexed";
				ecs.emplace<tc::Transform>(id);
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

			auto change_pos = [&](auto id) {
				if (auto transform = ecs.get<tc::Transform>(id))
					transform->position += els::vec3f::i + els::vec3f::k;
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

			int count = 0;
			auto check_and_print = [&](auto id) {
				if (auto transform = ecs.get<tc::Transform>(id))
				{
					if (transform->position != els::vec3(2, 0, 2))
					{
						++count;
						cout << id << ": ";
						cout << "{";
						cout << transform->position.x << ",";
						cout << transform->position.y << ",";
						cout << transform->position.z;
						cout << "}" << endl;
					}
				}
			};

			auto meta_view = ecs.view<tc::Meta>();
			auto transform_view = ecs.view<tc::Transform>();

			std::thread t1(ts::test_system{}, meta_view, change_name1);
			std::thread t2(ts::test_system{}, meta_view, change_name2);

			t1.join();
			t2.join();

			ts::test_system{}(meta_view, print_name);
			cout << "Total Bad: " << count << endl;
		}

		{
			// exit
		}
	}

	if (auto debug_mr = mm.get_debug_resource())
		output_debug_mr(debug_mr);

	mm.reclaim();
}