/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

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
template <typename T, typename... Ts>
struct is_any : std::bool_constant<(std::is_same_v<T, Ts> || ...)> {};

/**
 * @brief checks parameter pack for duplicate types. Returns true
 * if all types are the same
 */
template <typename T, typename... Ts>
struct are_same : std::bool_constant<(std::is_same_v<T, Ts> && ...)> {};


/**
 * @brief checks if a typelist's parameter pack contains member. True if 
 * T is a member
 */
template <typename...>
struct is_type_list_member;

template <typename T, template<typename> class Ttypelist, typename ... Ts>
struct is_type_list_member<T, Ttypelist<Ts...>> : is_any<T, Ts...> {};


/**
 * @brief Hashes a type to a size_t
 */
template <typename T>
size_t t_hash()
{
	return typeid(T).hash_code();
}