#pragma once

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <array>
#include <deque>

#include <memory>
#include <algorithm>
#include <atomic>
#include <memory_resource>
#include <iostream>

#include <filesystem>


namespace Tempest
{
	template <typename T>
	using tuptr = std::unique_ptr<T>;
	template <typename T>
	using tsptr = std::shared_ptr<T>;


	template <typename T, typename ... Args>
	constexpr tuptr<T> make_uptr(Args&& ... args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}
	template <typename T, typename ... Args>
	constexpr tsptr<T> make_sptr(Args&& ... args) {
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template <typename T>
	using tvector = std::pmr::vector<T>;
	template <typename T>
	using tstack = std::pmr::vector<T>;
	template <typename T>
	using tqueue = std::pmr::deque<T>;
	template <typename T>
	using tset = std::pmr::unordered_set<T>;
	template <typename Key, typename Val>
	using tmap = std::pmr::unordered_map<Key, Val>;

	using string = std::string;

	using id_t = std::uint32_t;

	using m_resource = std::pmr::memory_resource;

	constexpr static id_t UNDEFINED = static_cast<id_t>(-1);

	using tpath = std::filesystem::path;
	using tentry = std::filesystem::directory_entry;
}