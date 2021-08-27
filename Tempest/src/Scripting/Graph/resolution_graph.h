#pragma once
#include "graph.h"

namespace Tempest
{
	class resolution : public graph
	{

	public:

		resolution(const string& _name = "Default Resolution", m_resource* mem = std::pmr::get_default_resource())
			:
			graph(_name, mem)

		{
			// start nodes

			// start event
			add_node(ResolutionNode::create_node("Start")); 

			// start links to System 1
			// attacking calculates the value and passes it on
			add_node(ResolutionNode::create_node("System1"));

			// attacking links to defending
			// defending calculates the value and passes it on
			add_node(ResolutionNode::create_node("System2"));

			// resolve attacking and defending
			add_node(ResolutionNode::create_node("Resolve"));
		}

	};
}
