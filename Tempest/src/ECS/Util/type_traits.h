#pragma once


/**
 * @brief Type list type for providing compile time constexpr
 * size of a parameter pack
 */
template<typename... T>
struct type_list {
	using type = type_list;
	static constexpr auto size = sizeof...(T);
};

/**
 * @brief Specific type for view exclusion
 */
template<typename... type>
struct exclude_t : type_list<type...> {};

/**
 * @brief Statically asserts a parameter pack is unique
 */
template <typename... TArgs>
struct unique_types;

template <>
struct unique_types<> {};
template <typename T>
struct unique_types<T> {};
template <typename T1, typename T2>
struct unique_types<T1, T2> {
	static_assert(!std::is_same<T1, T2>::value, "Types must be unique");
};
template <typename T1, typename T2, typename ... TArgs>
struct unique_types<T1, T2, TArgs ...>
	: unique_types<T1, T2>, unique_types<T1, TArgs ...>, unique_types<T2, TArgs ...> {};

