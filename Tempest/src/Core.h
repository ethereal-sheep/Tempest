#pragma once

#include <vector>
#include <unordered_set>
#include <unordered_map>


namespace Tempest
{
	template <typename T>
	using tvector = std::vector<T>;
	template <typename T>
	using tset = std::unordered_set<T>;
	template <typename T>
	using tmap = std::unordered_map<T>;

	using string = std::string;

	using id_t = unsigned int;

	constexpr static id_t UNDEFINED = static_cast<id_t>(-1);
}