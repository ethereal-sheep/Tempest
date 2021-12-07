/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "../GMS.h"
#include "test_scripting.h"
#include <numeric>

namespace Tempest
{
	void testing_graph_1()
	{
		debug_resource dr;
		GMS gms(&dr);
		auto& s1 = gms.new_graph();
		auto& s2 = gms.new_graph();
		LOG_ASSERT(s1.get_name() != s2.get_name());
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
			auto& s = gms.new_graph();
			LOG_ASSERT(!ss.count(s.get_name()));
			ss.insert(s.get_name());
		}
	}
	void testing_graph_3()
	{
		const char* s = R"(S:\Development\Projects)";
		tpath path(s);

		debug_resource dr;
		GMS gms(&dr);

		auto& a = gms.new_graph();
		auto an = a.get_name();

		auto& b = gms.new_graph();
		auto bn = b.get_name();

		gms.save_all_to(path);

		gms.clear();
		try
		{
			gms.load(path / an);
			gms.load(path / bn);
		}
		catch (const std::exception& a)
		{
			LOG_CRITICAL("{}", a.what());
		}
	}

	void testing_graph_4()
	{
		const char* s = R"(S:\Development\Projects)";
		tpath path(s);

		debug_resource dr;
		GMS gms(&dr);

		auto& a = gms.new_graph();
		auto an = a.get_name();

		auto& b = gms.new_graph();
		auto bn = b.get_name();




		for (auto& g : gms.get_graphs())
		{
			auto n1 = g.add_node(CreateNode("test", "testing2"));
			auto n2 = g.add_node(CreateNode("test", "testing2"));
			auto n3 = g.add_node(CreateNode("test", "testing2"));

			g.add_link(
				n1->get_outputs()[0].get_id(),
				n2->get_inputs()[0].get_id());
			g.add_link(
				n1->get_outputs()[3].get_id(),
				n2->get_inputs()[3].get_id());

			n1->get_inputs()[1].default_var.set(1.f);
			n2->get_inputs()[1].default_var.set(1.f);
			n3->get_inputs()[1].default_var.set(1.f);

			gms.save_to(path, g);
		}

		try
		{
			gms.load(path / an);
			gms.load(path / bn);
		}
		catch (const std::exception& a)
		{
			LOG_CRITICAL("{}", a.what());
		}

		gms.clear();
		try
		{
			gms.load(path / an);
			gms.load(path / bn);
		}
		catch (const std::exception& a)
		{
			LOG_CRITICAL("{}", a.what());
		}
	}
	void testing_graph_5()
	{
		const char* s = R"(S:\Development\Projects)";
		tpath path(s);

		debug_resource dr;
		dr.set_strict(true);
		GMS gms(&dr);
		
		try
		{
			gms.load(path / "Graphs" / "Failure.json");
			LOG_CRITICAL("fail");
		}
		catch (const std::exception& a)
		{
			LOG_ERROR(a.what());
		}

		try
		{
			gms.load(path / "Graphs" / "Untitled.json");
		}
		catch (const std::exception&)
		{
			LOG_CRITICAL("fail");
		}

		gms.load(path / "Graphs" / "Untitled.json");

		LOG_ASSERT(gms.get_graphs().size() == 1);

		for (auto& g : gms.get_graphs())
		{
			for (auto& [id, node] : g.get_nodes())
			{
				LOG_ASSERT(node->get_inputs()[1].default_var.get<float>() == 1.f);
			}
		}

	}
	void testing_graph_6()
	{
		const char* s = R"(S:\Development\Projects)";
		const char* f = R"(Hello)";
		tpath path(s);
		{
			debug_resource dr;
			GMS gms(&dr);
			gms.new_graph();
			for (auto& graph : gms.get_graphs())
			{
				graph.add_node(CreateNode("test", "all"));
				graph.add_node(CreateNode("test", "testing1"));
				graph.add_node(CreateNode("test", "testing2"));
				for (auto& [id, node] : graph.get_nodes())
				{
					for (auto& pin : node->get_inputs())
					{
						switch (pin.get_type())
						{
						case Tempest::pin_type::Bool: pin.default_var.set(true);
							break;
						case Tempest::pin_type::Byte: pin.default_var.set<uint8_t>('1');
							break;
						case Tempest::pin_type::Int: pin.default_var.set(1);
							break;
						case Tempest::pin_type::Int64: pin.default_var.set(1LL);
							break;
						case Tempest::pin_type::Float: pin.default_var.set(1.f);
							break;
						case Tempest::pin_type::String: pin.default_var.set<string>("1");
							break;
						case Tempest::pin_type::Vec2: pin.default_var.set(vec2{1.f});
							break;
						case Tempest::pin_type::Vec3: pin.default_var.set(vec3{ 1.f });
							break;
						case Tempest::pin_type::Vec4: pin.default_var.set(vec4{ 1.f });
							break;
						case Tempest::pin_type::END:
						case Tempest::pin_type::Flow:
						case Tempest::pin_type::Entity:
						case Tempest::pin_type::Vector:
						default:
							break;
						}
					}
				}
				try
				{
					gms.save_as(path, f, graph);
				}
				catch (const std::exception& a)
				{
					LOG_CRITICAL("{}", a.what());
				}
			}
		}
		{
			debug_resource dr;
			GMS gms(&dr);

			gms.load(path / f);
			LOG_ASSERT(gms.get_graphs().size() == 1);

			for (auto& g : gms.get_graphs())
			{
				for (auto& [id, node] : g.get_nodes())
				{
					for (auto& pin : node->get_inputs())
					{
						switch (pin.get_type())
						{
						case Tempest::pin_type::Bool: LOG_ASSERT(pin.default_var.get<bool>() == true);
							break;
						case Tempest::pin_type::Byte: LOG_ASSERT(pin.default_var.get<uint8_t>() == '1');
							break;
						case Tempest::pin_type::Int: LOG_ASSERT(pin.default_var.get<int>() == 1);
							break;
						case Tempest::pin_type::Int64: LOG_ASSERT(pin.default_var.get<int64_t>() == 1LL);
							break;
						case Tempest::pin_type::Float: LOG_ASSERT(pin.default_var.get<float>() == 1.f);
							break;
						case Tempest::pin_type::String: LOG_ASSERT(pin.default_var.get<string>() == "1");
							break;
						case Tempest::pin_type::Vec2: LOG_ASSERT(pin.default_var.get<vec2>() == vec2{ 1.f });
							break;
						case Tempest::pin_type::Vec3: LOG_ASSERT(pin.default_var.get<vec3>() == vec3{ 1.f });
							break;
						case Tempest::pin_type::Vec4: LOG_ASSERT(pin.default_var.get<vec4>() == vec4{ 1.f });
							break;
						case Tempest::pin_type::END:
						case Tempest::pin_type::Flow:
						case Tempest::pin_type::Entity:
						case Tempest::pin_type::Vector:
						default:
							break;
						}
					}
				}
			}
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
			//LOG("{0}, {1}", start, end);
		}
		for (auto [start, end] : a)
		{
			//LOG("{0}, {1}", start, end);
		}
		
		auto t = [](auto id, auto start, auto end)
		{
			//LOG("{0:b}, {1:b}, {2:b}", id, start, end);
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
		testing_graph_6();
		testing_algo();
	}
}