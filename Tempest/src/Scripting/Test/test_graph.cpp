
#include "../GMS.h"
#include "test_scripting.h"
#include <numeric>

namespace Tempest
{
	void testing_graph_1()
	{
		debug_resource dr;
		GMS gms(&dr);
		auto s1 = gms.new_graph();
		auto s2 = gms.new_graph();
	}

	void testing_graph_2()
	{
		debug_resource dr;
		GMS gms(&dr);
		tset<string> ss;
		// stress test
		auto t = 5;
		while (t--)
		{
			auto s = gms.new_graph();
			LOG_ASSERT(!ss.count(s));
			ss.insert(s);
		}
	}
	void testing_graph_3()
	{
		const char* s = R"(S:\Development\Projects)";
		tpath path(s);

		debug_resource dr;
		GMS gms(&dr);

		gms.new_graph();
		gms.new_graph();

		gms.save_all(path);
	}

	void testing_graph_4()
	{
		const char* s = R"(S:\Development\Projects)";
		tpath path(s);

		debug_resource dr;
		GMS gms(&dr);

		gms.new_graph();
		gms.new_graph();

		for (auto& g : gms.get_graphs())
		{
			auto n1 = g.add_node(CreateNode("test", "testing2"));
			auto n2 = g.add_node(CreateNode("test", "testing2"));
			g.add_link(
				n1->get_outputs()[0].get_id(),
				n2->get_inputs()[0].get_id());
			g.add_link(
				n1->get_outputs()[3].get_id(),
				n2->get_inputs()[3].get_id());
		}
		gms.save_all(path);
	}
	void testing_graph_5()
	{
		const char* s = R"(S:\Development\Projects)";
		tpath path(s);

		debug_resource dr;
		dr.set_strict(true);
		GMS gms(&dr);

		gms.load(path / "Graphs" / "Failure.json");
		gms.load(path / "Graphs" / "Untitled.json");

		LOG_ASSERT(gms.get_graphs().size() == 1);

		for (auto& g : gms.get_graphs())
		{
			g.get_nodes();
		}
	}
	void testing_algo()
	{
		tvector<uint64_t> test;
		tset<uint64_t> test2;
		for (uint32_t i = 1; i < 100; ++i)
		{
			test.push_back(concatenate_id_t(i, els::random::uniform_rand(0, 1000)));
			test2.insert(concatenate_id_t(i, els::random::uniform_rand(0, 1000)));
		}

		split_view v(test);
		split_view a(test2);
		tri_split_view b(test);
		tri_split_view c(test2);

		for (auto [start, end] : v)
		{
			LOG("{0}, {1}", start, end);
		}
		for (auto [start, end] : a)
		{
			LOG("{0}, {1}", start, end);
		}
		
		auto t = [](auto id, auto start, auto end)
		{
			LOG("{0:b}, {1:b}, {2:b}", id, start, end);
			LOG_ASSERT(id == concatenate_id_t(start, end));
		};
		c.each(t);
	}

	void testing_graph()
	{
		testing_graph_1();
		testing_graph_2();
		testing_graph_3();
		testing_graph_4();
		testing_graph_5();
		testing_algo();
	}
}