#include "ECS/Util/sparse_set.h"
#include "ECS/Components/Components.h"
#include "ECS/ECS.h"

#include <iostream>

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

	auto lambda = [&](auto id) {
		cout << test.get<tc::Meta>(id)->name << endl;
	};

	a.each(lambda);

}