#pragma once
#include <type_traits>

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
 * @brief checks parameter pack for duplicate types. Returns true
 * if any duplicate found
 */
template <class T, class... Ts>
struct is_any : std::bool_constant<(std::is_same_v<T, Ts> || ...)> {};

/**
 * @brief checks parameter pack for duplicate types. Returns true
 * if all types are the same
 */
template <class T, class... Ts>
struct are_same : std::bool_constant<(std::is_same_v<T, Ts> && ...)> {};


/**
 * @brief Hashes a type to a size_t
 */
template <typename T>
size_t t_hash()
{
	return typeid(T).hash_code();
}