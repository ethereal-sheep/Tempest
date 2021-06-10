#pragma once

#define TRACY_ENABLE

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <array>
#include <deque>

#include <memory>
#include <functional>
#include <algorithm>
#include <atomic>
#include <memory_resource>
#include <iostream>

#include <filesystem>


namespace Tempest
{
	/**
	 * @brief Alias for polymorphic resource
	 */
	using m_resource = std::pmr::memory_resource;
	/**
	 * @brief Alias for unique_ptr
	 */
	template <typename T>
	using tuptr = std::unique_ptr<T>;
	/**
	 * @brief Alias for shared_ptr
	 */
	template <typename T>
	using tsptr = std::shared_ptr<T>;


	/**
	 * @brief Forwarding for make_unique
	 */
	template <typename T, typename ... Args>
	static inline constexpr [[nodiscard]] tuptr<T> make_uptr(Args&& ... args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}
	/**
	 * @brief Forwarding for make_shared
	 */
	template <typename T, typename ... Args>
	static inline constexpr [[nodiscard]] tsptr<T> make_sptr(Args&& ... args) {
		return std::make_shared<T>(std::forward<Args>(args)...);
	}


	/**
	 * @brief Alias for vector
	 */
	template <typename T>
	using tvector = std::pmr::vector<T>;
	/**
	 * @brief Alias for stack
	 */
	template <typename T>
	using tstack = std::pmr::vector<T>;
	/**
	 * @brief Alias for deque
	 */
	template <typename T>
	using tqueue = std::pmr::deque<T>;
	/**
	 * @brief Alias for unordered_set
	 */
	template <
		typename Key,
		typename Hash = std::hash<Key>,
		typename Pred = std::equal_to<Key>
	>
	using tset = std::pmr::unordered_set<Key, Hash, Pred>;
	/**
	 * @brief Alias for unordered_map
	 */
	template <
		typename Key,
		typename Val,
		typename Hash = std::hash<Key>,
		typename Pred = std::equal_to<Key>
	>
	using tmap = std::pmr::unordered_map<Key, Val, Hash, Pred>;
	/**
	 * @brief Alias for pair
	 */
	template <typename T1, typename T2>
	using tpair = std::pair<T1, T2>;

	/**
	 * @brief Alias for Forwarding for make_pair
	 */
	template <typename T1, typename T2>
	static inline constexpr [[nodiscard]] tpair<T1, T2> make_tpair(T1&& arg1, T2&& arg2) {
		return std::make_pair<T1, T2>(std::forward<T1>(arg1), std::forward<T2>(arg2));
	}

	/**
	 * @brief Alias for string
	 */
	using string = std::string;

	/**
	 * @brief Global id type
	 */
	using id_t = std::uint32_t;

	/**
	 * @brief Global constant for undefined id
	 */
	constexpr static id_t UNDEFINED = static_cast<id_t>(-1);


	/**
	 * @brief Alias for filesystem patj
	 */
	using tpath = std::filesystem::path;
	/**
	 * @brief Alias for filesystem directory entry
	 */
	using tentry = std::filesystem::directory_entry;
}