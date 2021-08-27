#pragma once
#include "graph.h"

namespace Tempest
{
	class system_graph : public graph
	{

	public:
		system_graph(const string& _name = "Default", m_resource* mem = std::pmr::get_default_resource())
			: 
			graph(_name, mem)

		{
			add_var("End", pin_type::Int)->get<int>();

		}

	};
}