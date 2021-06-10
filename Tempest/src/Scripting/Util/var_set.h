#pragma once
#include "Core.h"
#include "var_data.h"
#include "Util/range.h"

namespace Tempest
{
	/**
	 * @brief Constructor from a graph file path.
	 * @throw Throws graph_exception when the graph is not constructed properly
	 */
	template <typename T, typename Enable = void>
	struct serial;

	template <typename Integral>
	struct serial<Integral, std::enable_if<std::is_integral_v<Integral>>>
	{
		string operator()(Integral element)
		{
			return std::to_string(element);
		}

		Integral operator[](string elem)
		{
			return static_cast<Integral>(std::stoll(elem));
		}
	};

	template <>
	struct serial<string>
	{
		string operator()(const string& str)
		{
			return str;
		}

		string operator[](string element)
		{
			return std::move(element);
		}
	};

	/**
	 * @brief Variable Set that stores variables. Uses an unordered_map as
	 * underlying container type.
	 * @tparam Key key type of map
	 * @tparam Hash hash function for hashing key
	 * @tparam Serial Serial function for serializing/deserializing key
	 * @tparam Pred Predicate function for equality of key
	 */
	template <
		typename Key, 
		typename Hash = std::hash<Key>,
		typename Serial = serial<Key>,
		typename Pred = std::equal_to<Key>
	>
	class var_set
	{
	public:
		using set_type = tmap<Key, var_data, Hash, Pred>;

		/** @brief type traits for container */
		using key_type = Key;
		using mapped_type = var_data;
		using value_type = std::tuple<key_type, pin_type, mapped_type>;
		using hasher = Hash;
		using key_equal = Pred;
		using serializer = Serial;

		using size_type = typename set_type::size_type;
		using difference_type = typename set_type::difference_type;

		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const value_type*;
		

		/** @brief iterator type */
		class var_iterator
		{
			friend class var_set;
			using It = typename set_type::iterator;

			/**
			 * @brief Constructs the var iterator
			 * @param curr curr iterator from the parent class
			 */
			var_iterator(It curr) : it{ curr } {}

		public:
			/** @brief ttype traits for iterator */
			using difference_type = size_t;
			using value_type = std::tuple<key_type, pin_type, mapped_type>;
			using pointer = value_type*;
			using reference = value_type&;
			using iterator_category = std::forward_iterator_tag;

			var_iterator() = delete;


			/** @brief Standard boiler plate for iterators. */
			var_iterator& operator++()
			{
				return ++it, * this;
			}

			/** @brief Standard boiler plate for iterators  */
			var_iterator operator++(int)
			{
				var_iterator orig = *this;
				return ++(*this), orig;
			}

			/** @brief Standard boiler plate for iterators  */
			[[nodiscard]] bool operator==(const var_iterator& rhs) const
			{
				return rhs.it == it;
			}

			/** @brief Standard boiler plate for iterators  */
			[[nodiscard]] bool operator!=(const var_iterator& rhs) const
			{
				return !(*this == rhs);
			}

			/** @brief Standard boiler plate for iterators. */
			[[nodiscard]] pointer operator->()
			{
				auto& [key, val] = *it;
				temp = std::make_tuple(key, val.get_type(), val);
				return &temp;
			}

			/** @brief Standard boiler plate for iterators  */
			[[nodiscard]] reference operator*()
			{
				return *operator->();
			}

		private:
			It it;
			mutable value_type temp;
		};

		/** @brief const_iterator type */
		class const_var_iterator
		{
			friend class var_set;
			using It = typename set_type::const_iterator;

			/**
			 * @brief Constructs the var iterator
			 * @param curr curr iterator from the parent class
			 */
			const_var_iterator(It curr) : it{ curr } {}

		public:
			/** @brief ttype traits for iterator */
			using difference_type = size_t;
			using value_type = std::tuple<key_type, pin_type, mapped_type>;
			using pointer = const value_type*;
			using reference = const value_type&;
			using iterator_category = std::forward_iterator_tag;

			const_var_iterator() = delete;


			/** @brief Standard boiler plate for iterators. */
			const_var_iterator& operator++()
			{
				return ++it, * this;
			}

			/** @brief Standard boiler plate for iterators  */
			const_var_iterator operator++(int)
			{
				const_var_iterator orig = *this;
				return ++(*this), orig;
			}

			/** @brief Standard boiler plate for iterators  */
			[[nodiscard]] bool operator==(const const_var_iterator& rhs) const
			{
				return rhs.it == it;
			}

			/** @brief Standard boiler plate for iterators  */
			[[nodiscard]] bool operator!=(const const_var_iterator& rhs) const
			{
				return !(*this == rhs);
			}

			/** @brief Standard boiler plate for iterators. */
			[[nodiscard]] pointer operator->()
			{
				auto& [key, val] = *it;
				temp = std::make_tuple(key, val.get_type(), val);
				return &temp;
			}

			/** @brief Standard boiler plate for iterators  */
			[[nodiscard]] reference operator*()
			{
				return *operator->();
			}

		private:
			It it;
			mutable value_type temp;
		};


		using iterator = var_iterator;
		using const_iterator = const_var_iterator;
		using iterator_category = std::forward_iterator_tag;


		/**
		 * @brief Return true if set is empty.
		 */
		[[nodiscard]] bool empty() const
		{
			return vars.empty();
		}

		/**
		 * @brief Returns 1 if key exist in set. Returns 0 if it doesn't.
		 */
		[[nodiscard]] size_type count(const Key& key) const
		{
			return vars.count(key);
		}

		/**
		 * @brief Returns size of set.
		 */
		[[nodiscard]] size_type size() const
		{
			return vars.size();
		}

		/**
		 * @brief Gets type of variable at key. Throws if key doesn't exist.
		 * @param key key to lookup
		 * @throw index_out_of_range exception
		 */
		[[nodiscard]] pin_type type_at(const Key& key) const
		{
			return vars.at(key).get_type();
		}

		/**
		 * @brief Gets variable at key. Throws if key doesn't exist or if typing is wrong.
		 * @param key key to lookup
		 * @throw index_out_of_range exception if key doesn't exist
		 * @throw bad_variant_access exception if typing is wrong
		 */
		template <typename T>
		[[nodiscard]] T& get_at(const Key& key)
		{
			return vars.at(key).get<T>();
		}
		template <typename T>
		[[nodiscard]] const T& get_at(const Key& key) const
		{
			return vars.at(key).get<T>();
		}

		/**
		 * @brief Gets variable pointer at key. Returns nullptr if key 
		 * doesn't exist.
		 * @param key key to lookup
		 */
		template <typename T>
		[[nodiscard]] T* get_if_at(const Key& key) 
		{
			if (!count(key)) return nullptr;

			return vars.at(key).get_if<T>();
		}
		template <typename T>
		[[nodiscard]] const T* get_if_at(const Key& key) const
		{
			if (!count(key)) return nullptr;

			return vars.at(key).get_if<T>();
		}

		/**
		 * @brief Gets var_data at key. Throws if key doesn't exist.
		 * @param key key to lookup
		 * @throw index_out_of_range exception
		 */
		[[nodiscard]] var_data& at(const Key& key)
		{
			return vars.at(key);
		}
		[[nodiscard]] const var_data& at(const Key& key) const
		{
			return vars.at(key);
		}

		/**
		 * @brief Gets var_data pointer at key. Returns nullptr if key
		 * doesn't exist.
		 * @param key key to lookup
		 */
		[[nodiscard]] var_data* at_if(const Key& key)
		{
			if (count(key)) return nullptr;

			return &vars.at(key);
		}
		[[nodiscard]] const var_data* at_if(const Key& key) const
		{
			if (count(key)) return nullptr;

			return &vars.at(key);
		}

		/**
		 * @brief Creates a variable of type T, initializes by data.
		 * @tparam T type of variable
		 * @param key Key of variable
		 * @param data Data to pass in
		 * @return Pointer to created variable if successful
		 */
		template <typename T>
		T* create(const Key& key, T data = T{})
		{
			if(count(key)) return nullptr;

			vars.emplace(key, var_data{ data });
			return get_if_at<T>(key);
		}

		/**
		 * @brief Creates a variable of type in pin_type. Default initializes 
		 * variable.
		 * @param key Key of variable
		 * @param t Pin type
		 * @return True if varaible created successfully
		 */
		bool create(const Key& key, pin_type t)
		{
			if (count(key)) return false;

			vars.emplace(key, var_data{ t });
			return true;
		}

		/**
		 * @brief Erases key from the var set
		 * @param key Key to remove from the set
		 */
		void erase(const Key& key)
		{
			if (!count(key)) return;
			vars.erase(key);
		}

		/**
		 * @brief Standard boiler plate for iterable objects
		 * @return Iterator to the beginning of range
		 */
		[[nodiscard]] iterator begin()
		{
			return iterator{ vars.begin() };
		}
		[[nodiscard]] const_iterator begin() const
		{
			return const_iterator{ vars.begin() };
		}

		/**
		 * @brief Standard boiler plate for iterable objects
		 * @return Iterator to the end of range
		 */
		[[nodiscard]] iterator end()
		{
			return iterator{ vars.end() };
		}
		[[nodiscard]] const_iterator end() const
		{
			return const_iterator{ vars.end() };
		}

		/**
		 * @brief Standard boiler plate for iterable objects
		 * @return Const Iterator to the beginning of range
		 */
		[[nodiscard]] const_iterator cbegin() const
		{
			return const_iterator{ vars.begin() };
		}

		/**
		 * @brief Standard boiler plate for iterable objects
		 * @return Const Iterator to the end of range
		 */
		[[nodiscard]] const_iterator cend() const
		{
			return const_iterator{ vars.end() };
		}
		/**
		 * @brief Standard boiler plate for iterable objects
		 * @return Iterator to found object. Returns end if object
		 * is not found.
		 */
		[[nodiscard]] iterator find(const Key& key) 
		{
			return iterator{ vars.find(key) };
		}
		[[nodiscard]] const_iterator find(const Key& key) const
		{
			return const_iterator{ vars.find(key) };
		}

	private:
		set_type vars;
	};
}