/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#include "Core.h"
#include "internal.h"

namespace Tempest
{
	/**
	 * @brief An iterable object that allows for range access to a container of uint64_t,
	 * split by higher and lower order bits.
	 */
	template <typename Cont, typename = std::enable_if_t<std::is_same_v<typename Cont::value_type, uint64_t>>>
	class split_view
	{
		using con_ref = Cont&;

		static constexpr bool is_set = std::is_same_v<typename Cont::iterator::iterator_category, std::forward_iterator_tag>;
		static constexpr bool is_vector = !is_set;

	public:
		/**
		 * @brief Constructs a split_view object with a reference to a container of
		 * uint64_t
		 */
		split_view(con_ref _container) : container{ _container }
		{
			static_assert(
				std::is_same_v<typename Cont::value_type, uint64_t>,
				"Container type in split_view must have value_type of uint64_t!");
		}


		/**
		 * @brief iterator type for the split_view object
		 * @tparam It templated iterator types
		 */
		template <typename It>
		class view_iterator final
		{
			friend class split_view;

			/**
			 * @brief Constructs the view iterator
			 * @param curr curr iterator from the parent class
			 */
			view_iterator(It curr) : it{ curr } {}

		public:
			/** @brief ttype traits for iterator */
			using difference_type = size_t;
			using value_type = tpair<uint32_t, uint32_t>;
			using pointer = const value_type*;
			using reference = const value_type&;
			using iterator_category = std::forward_iterator_tag;

			view_iterator() = delete;


			/** @brief Standard boiler plate for iterators but we do a check
			 * each time we change the position of iterator */
			view_iterator& operator++()
			{
				return ++it, * this;
			}

			/** @brief Standard boiler plate for iterators  */
			view_iterator operator++(int)
			{
				view_iterator orig = *this;
				return ++(*this), orig;
			}

			/** @brief Standard boiler plate for iterators  */
			[[nodiscard]] bool operator==(const view_iterator& rhs) const
			{
				return rhs.it == it;
			}

			/** @brief Standard boiler plate for iterators  */
			[[nodiscard]] bool operator!=(const view_iterator& rhs) const
			{
				return !(*this == rhs);
			}

			/** @brief Standard boiler plate for iterators. Here we split the value
			and set a mutable temp variable so we can maintain const-correctness. */
			[[nodiscard]] pointer operator->() const
			{
				temp = split_uint64_t(*it);
				return &temp;
			}

			/** @brief Standard boiler plate for iterators  */
			[[nodiscard]] reference operator*() const
			{
				return *operator->();
			}

		private:
			It it;
			mutable value_type temp;
		};

		/** @brief type traits for container */
		using value_type = tpair<uint32_t, uint32_t>;
		using size_type = std::size_t;
		using iterator = view_iterator<typename Cont::const_iterator>;
		using iterator_category = std::forward_iterator_tag;


		/**
		 * @brief Standard boiler plate for iterable objects
		 * @return Size of range
		 */
		[[nodiscard]] size_type size() const
		{
			return container.size();
		}
		/**
		 * @brief Standard boiler plate for iterable objects
		 * @return True if range is empty
		 */
		[[nodiscard]] bool empty() const
		{
			return container.size();
		}

		/**
		 * @brief Standard boiler plate for iterable objects
		 * @return Iterator to the beginning of range
		 */
		[[nodiscard]] iterator begin() const
		{
			return iterator{ container.begin() };
		}

		/**
		 * @brief Standard boiler plate for iterable objects
		 * @return Iterator to the end of range
		 */
		[[nodiscard]] iterator end() const
		{
			return iterator{ container.end() };
		}

		/**
		 * @brief Standard boiler plate for iterable objects
		 * @return Iterator to the found element.
		 */
		[[nodiscard]] iterator find(uint64_t element) const
		{
			if constexpr (is_set)
			{
				return iterator{ container.find(element) };
			}
			else
			{
				return iterator{ std::find(container.begin(), container.end(), element) };
			}
		}

		/**
		 * @brief Standard boiler plate for iterable objects
		 * @return Iterator to the found element.
		 */
		[[nodiscard]] iterator find(uint32_t first, uint32_t second) const
		{
			return find(concatenate_id_t(first, second));
		}

		/**
		 * @brief Standard boiler plate for iterable objects
		 * @return Iterator to the found element.
		 */
		[[nodiscard]] iterator find(const value_type& pair) const
		{
			return find(concatenate_id_t(pair.first, pair.second));
		}

		/**
		 * @brief Calls a callable object for each entity in view
		 * @tparam Func Callable object type
		 * @tparam func Callable object
		 */
		template<typename Func>
		[[nodiscard]] void each(Func func) const
		{
			for (auto [a, b] : *this)
				func(a, b);
		}

	private:
		con_ref container;
	};

	/**
	 * @brief An iterable object that allows for range access to a container of uint64_t,
	 * split by higher and lower order bits.
	 */
	template <typename Cont, typename = std::enable_if_t<std::is_same_v<typename Cont::value_type, uint64_t>>>
	class tri_split_view
	{
		using con_ref = Cont&;

		static constexpr bool is_set = std::is_same_v<typename Cont::iterator::iterator_category, std::forward_iterator_tag>;
		static constexpr bool is_vector = !is_set;

	public:
		/**
		 * @brief Constructs a split_view object with a reference to a container of
		 * uint64_t
		 */
		tri_split_view(con_ref _container) : container{ _container }
		{
			static_assert(
				std::is_same_v<typename Cont::value_type, uint64_t>,
				"Container type in split_view must have value_type of uint64_t!");
		}


		/**
		 * @brief iterator type for the split_view object
		 * @tparam It templated iterator types
		 */
		template <typename It>
		class view_iterator final
		{
			friend class tri_split_view;

			/**
			 * @brief Constructs the view iterator
			 * @param curr curr iterator from the parent class
			 */
			view_iterator(It curr) : it{ curr } {}

		public:
			/** @brief type traits for iterator */
			using difference_type = size_t;
			using value_type = std::tuple<uint64_t, uint32_t, uint32_t>;
			using pointer = const value_type*;
			using reference = const value_type&;
			using iterator_category = std::forward_iterator_tag;

			view_iterator() = delete;


			/** @brief Standard boiler plate for iterators but we do a check
			 * each time we change the position of iterator */
			view_iterator& operator++()
			{
				return ++it, * this;
			}

			/** @brief Standard boiler plate for iterators  */
			view_iterator operator++(int)
			{
				view_iterator orig = *this;
				return ++(*this), orig;
			}

			/** @brief Standard boiler plate for iterators  */
			[[nodiscard]] bool operator==(const view_iterator& rhs) const
			{
				return rhs.it == it;
			}

			/** @brief Standard boiler plate for iterators  */
			[[nodiscard]] bool operator!=(const view_iterator& rhs) const
			{
				return !(*this == rhs);
			}

			/** @brief Standard boiler plate for iterators. Here we split the value
			and set a mutable temp variable so we can maintain const-correctness. */
			[[nodiscard]] pointer operator->() const
			{
				auto element = *it;
				auto [first, second] = split_uint64_t(element);
				temp = std::make_tuple(element, first, second);
				return &temp;
			}

			/** @brief Standard boiler plate for iterators  */
			[[nodiscard]] reference operator*() const
			{
				return *operator->();
			}

		private:
			It it;
			mutable value_type temp;
		};

		/** @brief type traits for container */
		using value_type = std::tuple<uint64_t, uint32_t, uint32_t>;
		using size_type = std::size_t;
		using iterator = view_iterator<typename Cont::const_iterator>;
		using iterator_category = std::forward_iterator_tag;


		/**
		 * @brief Standard boiler plate for iterable objects
		 * @return Size of range
		 */
		[[nodiscard]] size_type size() const
		{
			return container.size();
		}
		/**
		 * @brief Standard boiler plate for iterable objects
		 * @return True if range is empty
		 */
		[[nodiscard]] bool empty() const
		{
			return container.size();
		}

		/**
		 * @brief Standard boiler plate for iterable objects
		 * @return Iterator to the beginning of range
		 */
		[[nodiscard]] iterator begin() const
		{
			return iterator{ container.begin() };
		}

		/**
		 * @brief Standard boiler plate for iterable objects
		 * @return Iterator to the end of range
		 */
		[[nodiscard]] iterator end() const
		{
			return iterator{ container.end() };
		}

		/**
		 * @brief Standard boiler plate for iterable objects
		 * @return Iterator to the found element.
		 */
		[[nodiscard]] iterator find(uint64_t element) const
		{
			if constexpr (is_set)
			{
				return iterator{ container.find(element) };
			}
			else
			{
				return iterator{ std::find(container.begin(), container.end(), element) };
			}
		}

		/**
		 * @brief Standard boiler plate for iterable objects
		 * @return Iterator to the found element.
		 */
		[[nodiscard]] iterator find(uint32_t first, uint32_t second) const
		{
			return find(concatenate_id_t(first, second));
		}

		/**
		 * @brief Standard boiler plate for iterable objects
		 * @return Iterator to the found element.
		 */
		[[nodiscard]] iterator find(const tpair<uint32_t, uint32_t>& pair) const
		{
			return find(concatenate_id_t(pair.first, pair.second));
		}

		/**
		 * @brief Calls a callable object for each entity in view
		 * @tparam Func Callable object type
		 * @tparam func Callable object
		 */
		template<typename Func>
		[[nodiscard]] void each(Func func) const
		{
			for (auto [a, b, c] : *this)
				func(a, b, c);
		}

	private:
		con_ref container;
	};


}
