#pragma once
#include "Graph/modifier_graph.h"

namespace Tempest
{
	class resolution
	{
	public:
		double resolve() const
		{
			
		}

		template <typename... Args>
		void insert_modifier(size_t index, Args&&... args)
		{

		}

		void remove_modifier(size_t index)
		{

		}

		void reorder_modifier(size_t curr, size_t n)
		{

		}

		auto get_modifiers()
		{
			return make_range(mods);
		}


	private:
		tvector<modifier_graph> mods;

	};
}