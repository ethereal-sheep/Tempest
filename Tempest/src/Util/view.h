/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include <iterator>

namespace Tempest
{

	/**
	 * @brief An iterable object that allows for view access toa underlying
	 * range. 
	 * @tparam It iterator type of the underlying range.
	 */
	template <typename It>
	class simple_view
	{

	public:
		/**
		 * @brief Constructs the simple_view
		 * @param from Iterator to start of view
		 * @param to Iterator to end of view
		 */
		simple_view(It from, It to) : start(from), finish(to) {}

		/** @brief type traits for container */
		using value_type = typename It::value_type;
		using size_type = size_t;
		using difference_type = typename It::difference_type;

		using reference = typename It::reference;
		using pointer = typename It::pointer;
		using iterator = It;
		using iterator_category = std::forward_iterator_tag;


		/**
		 * @brief Standard boiler plate for iterable objects
		 * @return Size of range
		 */
		[[nodiscard]] size_type size() const
		{
			return finish - start;
		}
		/**
		 * @brief Standard boiler plate for iterable objects
		 * @return True if range is empty
		 */
		[[nodiscard]] bool empty() const
		{
			return !(size());
		}

		/**
		 * @brief Standard boiler plate for iterable objects
		 * @return Iterator to the beginning of range
		 */
		[[nodiscard]] iterator begin() const
		{
			return start;
		}

		/**
		 * @brief Standard boiler plate for iterable objects
		 * @return Iterator to the end of range
		 */
		[[nodiscard]] iterator end() const
		{
			return finish;
		}

		/**
		 * @brief Calls a callable object for each value in range
		 * @tparam Func Callable object type
		 * @param func Callable object
		 */
		[[nodiscard]] iterator find(reference& element) const
		{
			for (auto it = begin(); it != end(); ++it)
				if (*it == element)	
					return it;

			return end();
		}

		/**
		 * @brief Calls a callable object for each value in range
		 * @tparam Func Callable object type
		 * @param func Callable object
		 */
		template<typename Func>
		void each(Func func) const
		{
			for (reference i : *this)
				func(i);
		}


	private:
		It start, finish;
	};




	/**
	 * @brief An iterable object that allows for view access toa underlying
	 * range.
	 * @tparam It iterator type of the underlying range.
	 * @tparam Pred Predicate function to determine if an element of view
	 * should be skipped. Predicate should return true if element should be skipped.
	 */
	template <typename It, typename Pred>
	class skipable_view
	{
	public:
		/**
		 * @brief Constructs the simple_view
		 * @param from Iterator to start of view
		 * @param to Iterator to end of view
		 */
		skipable_view(It from, It to, Pred _pred) : start(from), finish(to), pred(_pred) {}

		/** @brief type traits for container */
		using predicate = Pred;
		using value_type = typename It::value_type;
		using size_type = size_t;
		using difference_type = typename It::difference_type;

		using reference = typename It::reference;
		using pointer = typename It::pointer;

		using iterator_category = std::forward_iterator_tag;

		/**
		 * @brief iterator type for the skipable_view object
		 */
		class view_iterator final
		{
			friend class skipable_view;

			/**
			 * @brief Check if curr is valid by checking the element against
			 * the predicate
			 * @return True or False
			 */
			bool valid() const {
				return !pred(*it);
			}

			/**
			 * @brief Constructs the view iterator
			 * @param from begin iterator from the parent class
			 * @param to end iterator from the parent class
			 * @param curr curr iterator from the parent class
			 *
			 */
			view_iterator(It from, It to, It curr, Pred _pred) :
				first{ from },
				last{ to },
				it{ curr },
				pred{ _pred }
			{
				// if current isn't valid, iterate it until it is
				while (it != last && !valid()) {
					++(*this);
				}
			}

		public:
			/** @brief ttype traits for iterator */
			using difference_type = size_t;
			using value_type = skipable_view::value_type;
			using reference = skipable_view::reference;
			using pointer = skipable_view::pointer;
			using iterator_category = skipable_view::iterator_category;

			view_iterator() = delete;


			/** @brief Standard boiler plate for iterators but we do a check
			 * each time we change the position of iterator */
			view_iterator& operator++()
			{
				while (++it != last && !valid());
				return *this;
			}

			/** @brief Standard boiler plate for iterators but we do a check
			 * each time we change the position of iterator */
			view_iterator operator++(int)
			{
				view_iterator orig = *this;
				return ++(*this), orig;
			}

			/** @brief Standard boiler plate for iterators but we do a check
			 * each time we change the position of iterator */
			[[nodiscard]] bool operator==(const view_iterator& rhs) const
			{
				return rhs.it == it;
			}

			/** @brief Standard boiler plate for iterators but we do a check
			 * each time we change the position of iterator */
			[[nodiscard]] bool operator!=(const view_iterator& rhs) const
			{
				return !(*this == rhs);
			}

			/** @brief Standard boiler plate for iterators but we do a check
			 * each time we change the position of iterator */
			[[nodiscard]] pointer operator->() const
			{
				return &*it;
			}

			/** @brief Standard boiler plate for iterators but we do a check
			 * each time we change the position of iterator */
			[[nodiscard]] reference operator*() const
			{
				return *operator->();
			}

		private:
			It first;
			It last;
			It it;
			Pred pred;
		};

		using iterator = view_iterator;
		using iterator_category = std::forward_iterator_tag;


		/**
		 * @brief Standard boiler plate for iterable objects. Size of
		 * skippable range cannot be found in constant time so returns
		 * a hint of the size.
		 */
		[[nodiscard]] size_type size_hint() const
		{
			return end() - begin();
		}

		/**
		 * @brief Standard boiler plate for iterable objects
		 * @return Iterator to the beginning of range
		 */
		[[nodiscard]] iterator begin() const
		{
			return iterator(start, finish, start, pred);
		}

		/**
		 * @brief Standard boiler plate for iterable objects
		 * @return Iterator to the end of range
		 */
		[[nodiscard]] iterator end() const
		{
			return iterator(start, finish, finish, pred);
		}

		/**
		 * @brief Calls a callable object for each value in range
		 * @tparam Func Callable object type
		 * @param func Callable object
		 */
		[[nodiscard]] iterator find(reference& element) const
		{
			for (auto it = begin(); it != end(); ++it)
				if (*it == element)	
					return it;

			return end();
		}

		/**
		 * @brief Calls a callable object for each value in range
		 * @tparam Func Callable object type
		 * @param func Callable object
		 */
		template<typename Func>
		void each(Func func) const
		{
			for (reference i : *this)
				func(i);
		}


	private:
		It start, finish;
		Pred pred;
	};
}