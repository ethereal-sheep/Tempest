#pragma once

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <array>
#include <deque>

#include <memory>
#include <algorithm>


namespace Tempest
{
	template <typename T>
	using tuptr = std::unique_ptr<T>;
	template <typename T>
	using tsptr = std::shared_ptr<T>;


	template <typename T, typename ... Args>
	constexpr tuptr<T> make_ptr(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template <typename T>
	using tvector = std::vector<T>;
	template <typename T>
	using tstack = std::vector<T>;
	template <typename T>
	using tqueue = std::deque<T>;
	template <typename T>
	using tset = std::unordered_set<T>;
	template <typename Key, typename Val>
	using tmap = std::unordered_map<Key, Val>;

	using string = std::string;

	using id_t = std::uint32_t;

	constexpr static id_t UNDEFINED = static_cast<id_t>(-1);
}