
#define _CRT_SECURE_NO_WARNINGS

#include "ECS/Components/Components.h"
#include "ECS/ECS.h"
#include "Util.h"

#include "Memory/Resource/debug_resource.h"
#include "Memory/Resource/malloc_resource.h"

#include <iostream>
#include <thread>

using namespace std;
using namespace Tempest;

int main()
{
	std::pmr::monotonic_buffer_resource a{ 1000000 };
	debug_resource mr{ "testing", &a};

	Tempest::ECS test(&mr);

	test.register_component<tc::Transform>();
	test.register_component<tc::Meta>();
	test.register_component<tc::Relationship>();
	test.register_component<int>();
	test.register_component<double>();
	test.register_component<unsigned>();

	auto lambdaA = [&](auto id) {
		test.get<tc::Transform>(id)->position.x += 1.f;
	};

	auto lambdaB = [&](auto id) {
		cout << test.get<tc::Meta>(id)->name << endl;
	};

	auto lambdaC = [&](auto id) {
		cout << test.get<tc::Transform>(id)->position.x << endl;
	};

	std::pmr::set_default_resource(nullptr);


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


	//auto a = test.view<tc::Transform>();
	//auto b = test.view<tc::Meta>();

	//auto test1 = [&]()
	//{
	//	int t = 10000;
	//	while (t--)
	//	{
	//		a.each(lambdaC);
	//	}
	//};

	//auto test2 = [&]()
	//{
	//	int t = 10000;
	//	while (t--)
	//	{
	//		b.each(lambdaB);
	//	}
	//};

	//std::thread t1{ test1 };

	//test2();

	//t1.join();


	std::cout << "" << std::endl;
	std::cout << "-------------------------------------------" << std::endl;
	std::cout << "                " << mr.get_name() << std::endl;
	std::cout << "-------------------------------------------" << std::endl;
	std::cout << " Total bytes allocated:               " << mr.bytes_allocated() << std::endl;
	std::cout << " Total bytes deallocated:             " << mr.bytes_deallocated() << std::endl;
	std::cout << " Highest memory usage:                " << mr.bytes_highwater() << std::endl;
	std::cout << " Outstanding Bytes:                   " << mr.bytes_outstanding() << std::endl;
	std::cout << " Outstanding Blocks:                  " << mr.blocks_outstanding() << std::endl;
	std::cout << "-------------------------------------------" << std::endl;
	std::cout << " Last allocated address:              0x" << mr.last_allocated_address() << std::endl;
	std::cout << " Last allocated bytes:                " << mr.last_allocated_num_bytes() << std::endl;
	std::cout << " Last allocated alignment:            " << mr.last_allocated_alignment() << std::endl;
	std::cout << " Last deallocated address:            0x" << mr.last_deallocated_address() << std::endl;
	std::cout << " Last deallocated bytes:              " << mr.last_deallocated_num_bytes() << std::endl;
	std::cout << " Last deallocated alignment:          " << mr.last_deallocated_alignment() << std::endl;
	std::cout << "-------------------------------------------" << std::endl;
	std::cout << " Misc Errors:                         " << mr.misc_errors() << std::endl;
	std::cout << " Bad Deallocations:                   " << mr.bad_deallocations() << std::endl;
	std::cout << "-------------------------------------------" << std::endl;

}





/*Tempest::ECS test;
auto lambdaA = [&](auto id) {
	test.get<tc::Transform>(id)->position.x += 1.f;
};

auto lambdaB = [&](auto id) {
	cout << test.get<tc::Meta>(id)->name << endl;
};

auto lambdaC = [&](auto id) {
	cout << test.get<tc::Transform>(id)->position.x << endl;
};

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

lambdaA(id3);
auto id4 = test.clone(id3);
lambdaA(id3);

lambdaC(id3);
lambdaC(id4);

cout << test.has<tc::Meta>(id4) << endl;*/

/*auto test1 = [&]()
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

Tempest::Service<Tempest::ECS>::Release();*/