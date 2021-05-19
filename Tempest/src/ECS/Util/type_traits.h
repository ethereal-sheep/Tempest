#pragma once

namespace Tempest
{

	template<typename... T>
	struct type_list {
		using type = type_list;
		static constexpr auto size = sizeof...(T);
	};

	template<typename... type>
	struct exclude_t : type_list<type...> {};

	template <typename... Trest>
	struct unique_types;

	template <typename T1, typename T2, typename ... Trest>
	struct unique_types<T1, T2, Trest ...>
		: unique_types<T1, T2>, unique_types<T1, Trest ...>, unique_types<T2, Trest ...> {};

	template <typename T1, typename T2>
	struct unique_types<T1, T2>
	{
		static_assert(!std::is_same<T1, T2>::value, "Types must be unique");
	};

	template <typename T>
	struct unique_types<T> {};
}