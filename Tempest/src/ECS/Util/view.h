#pragma once

#include "type_traits.h"

namespace Tempest
{

	using vmap_ref = const tmap<size_t, tuptr<sparse_set>>&;


	template<typename...>
	class view;

	template<typename... Excludes, typename... Components>
	class view<exclude_t<Excludes...>, Components...>
	{


	public:
		view(vmap_ref pools) {

		}
	};

	template<typename Component>
	class view<exclude_t<>, Component>
	{

	public:
	};


	

	
}