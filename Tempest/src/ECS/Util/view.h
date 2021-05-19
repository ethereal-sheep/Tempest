#pragma once

namespace Tempest
{

	template<typename... Type>
	struct type_list {
		using type = type_list;
		static constexpr auto size = sizeof...(Type);
	};

	template<typename... Type>
	struct exclude_t : type_list<Type...> {};

	template<typename...>
	class view;

	template<typename... Excludes, typename... Components>
	class view<exclude_t<Excludes...>, Components...>
	{

	};
}