#pragma once

namespace Tempest
{
	template <typename Cont>
	class simple_range
	{
	public:
		/**
		 * @brief Constructs a range of a standard container.
		 * @param _container Container reference
		 */
		simple_range(Cont& _container) : container{ _container } {}

		/** @brief type traits for container */
		using value_type = typename Cont::value_type;
		using size_type = typename Cont::size_type;
		using difference_type = typename Cont::difference_type;

		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = typename Cont::pointer;
		using const_pointer = typename Cont::const_pointer;
		using iterator = typename Cont::iterator;
		using const_iterator = typename Cont::const_iterator;
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
		[[nodiscard]] iterator begin()
		{
			return container.begin();
		}
		[[nodiscard]] const_iterator begin() const
		{
			return container.begin();
		}

		/**
		 * @brief Standard boiler plate for iterable objects
		 * @return Iterator to the end of range
		 */
		[[nodiscard]] iterator end()
		{
			return container.end();
		}
		[[nodiscard]] const_iterator end() const
		{
			return container.end();
		}
		/**
		 * @brief Standard boiler plate for iterable objects
		 * @return Const iterator to the beginning of range
		 */
		[[nodiscard]] const_iterator cbegin() const
		{
			return container.begin();
		}

		/**
		 * @brief Standard boiler plate for iterable objects
		 * @return Const iterator to the end of range
		 */
		[[nodiscard]] const_iterator cend() const
		{
			return container.end();
		}

		/**
		 * @brief Calls a callable object for each value in range
		 * @tparam Func Callable object type
		 * @param func Callable object
		 */
		template<typename Func>
		void each(Func func)
		{
			for (value_type& i : *this)
				func(i);
		}

		/**
		 * @brief Calls a callable object for each value in range
		 * @tparam Func Callable object type
		 * @param func Callable object
		 */
		template<typename Func>
		void each(Func func) const
		{
			for (const value_type& i : *this)
				func(i);
		}

	private:
		Cont& container;
	};

	template <typename Cont>
	class const_range
	{
	public:
		/**
		 * @brief Constructs a const range of a standard container.
		 * @param _container Container reference
		 */
		const_range(const Cont& _container) : container{ _container } {}

		/** @brief type traits for container */
		using value_type = typename Cont::value_type;
		using size_type = typename Cont::size_type;
		using difference_type = typename Cont::difference_type;

		using reference = const value_type&;
		using const_reference = const value_type&;
		using pointer = typename Cont::const_pointer;
		using const_pointer = typename Cont::const_pointer;
		using iterator = typename Cont::const_pointer;
		using const_iterator = typename Cont::const_iterator;
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
		[[nodiscard]] const_iterator begin() const
		{
			return container.begin();
		}

		/**
		 * @brief Standard boiler plate for iterable objects
		 * @return Iterator to the end of range
		 */
		[[nodiscard]] const_iterator end() const
		{
			return container.end();
		}
		/**
		 * @brief Standard boiler plate for iterable objects
		 * @return Const iterator to the beginning of range
		 */
		[[nodiscard]] const_iterator cbegin() const
		{
			return container.begin();
		}

		/**
		 * @brief Standard boiler plate for iterable objects
		 * @return Const iterator to the end of range
		 */
		[[nodiscard]] const_iterator cend() const
		{
			return container.end();
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
		const Cont& container;
	};

	template <typename T>
	using range = simple_range<T>;
	template <typename T>
	using vec_range = range<tvector<T>>;
	template <typename T>
	using set_range = range<tset<T>>;
	template <typename TKey, typename TVal>
	using map_range = range<tmap<TKey, TVal>>;


	template <typename Cont>
	static inline constexpr [[nodiscard]] simple_range<Cont> make_range(Cont& _container)
	{
		return simple_range(_container);
	}
	template <typename Cont>
	static inline constexpr [[nodiscard]] const_range<Cont> make_const_range(const Cont& _container)
	{
		return const_range(_container);
	}
}