#pragma once
#include "graph.h"

namespace Tempest
{
	class modifier_graph : public graph
	{
		float& input;
		float& output;

	public:
		modifier_graph(const string& _name = "Default", m_resource* mem = std::pmr::get_default_resource())
			: 
			graph(_name, mem),
			input(graph::add_var("In", pin_type::Float)->get<float>()),
			output(graph::add_var("Out", pin_type::Float)->get<float>())

		{
			


		}

		float operator()(float fn_input)
		{
			input = fn_input;

			// i need some way to build scripts without instance



			return output;
		}
	};
}