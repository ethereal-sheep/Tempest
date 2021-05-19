#include "ECS/Util/sparse_set.h"
#include "ECS/Components/Components.h"
#include "ECS/ECS.h"

#include <iostream>
#include <thread>

using namespace std;

int main()
{
	Tempest::ECS test;
	// stress

	

	test.register_component<tc::Transform>();
	test.register_component<tc::Meta>();
	test.register_component<tc::Relationship>();

	auto id1 = test.create();
	auto meta = test.emplace<tc::Meta>(id1);
	test.emplace<tc::Transform>(id1);
	meta->name = "id1";

	auto id2 = test.create();
	meta = test.emplace<tc::Meta>(id2);
	meta->name = "id2";
	test.emplace<tc::Transform>(id2);
	test.emplace<tc::Relationship>(id2);

	auto id3 = test.create();
	meta = test.emplace<tc::Meta>(id3);
	test.emplace<tc::Transform>(id3);
	test.emplace<tc::Relationship>(id3);
	meta = test.replace<tc::Meta>(id3, "123");

	auto a = test.view<tc::Transform>();
	auto b = test.view<tc::Meta>();


	auto lambdaA = [&](auto id) {
		test.get<tc::Transform>(id)->position.x += 1.f;
	};

	auto lambdaB = [&](auto id) {
		cout << test.get<tc::Meta>(id)->name << endl;
	};

	auto lambdaC = [&](auto id) {
		cout << test.get<tc::Transform>(id)->position.x << endl;
	};

	int t = 1000;

	auto test1 = [&]()
	{
		int t = 10000;
		while (t--)
		{
			a.each(lambdaC);
		}
	};

	auto test2 = [&]()
	{
		int t = 10000;
		while (t--)
		{
			b.each(lambdaB);
		}
	};

	std::thread t1{ test1 };

	test2();

	t1.join();

	/*while (t--)
	{
		
		a.each(lambdaA);

	}*/


}