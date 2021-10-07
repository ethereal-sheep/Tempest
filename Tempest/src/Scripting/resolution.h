/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

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