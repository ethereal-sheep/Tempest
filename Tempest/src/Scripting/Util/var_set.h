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
#include "Core.h"
#include "var_data.h"
#include "Util/range.h"
#include "Util.h"
#include <map>

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


	class var_set_exception : public std::exception
	{
	public:
		var_set_exception(const string& err_msg = "variable set exception thrown!") : msg(err_msg) {}
		const char* what() const noexcept override { return msg.c_str(); }
	private:
		string msg;
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
		bool Ordered,
		typename Hash_Or_Comp,
		typename Serial
	>
	class var_set
	{
		/** @brief type traits to deduce ordered or unordered */
		template<bool Ordered, typename...>
		struct set_ordered;

		template <typename Key, typename Hash_Or_Comp>
		struct set_ordered<true, Key, Hash_Or_Comp> { using type = tomap<Key, var_data, Hash_Or_Comp>; };
		template <typename Key, typename Hash_Or_Comp>
		struct set_ordered<false, Key, Hash_Or_Comp> { using type = tmap<Key, var_data, Hash_Or_Comp>; };

		template <typename Key, bool Ordered, typename Hash_Or_Comp>
		using set_ordered_t = typename set_ordered<Ordered, Key, Hash_Or_Comp>::type;

	public:
		using set_type = set_ordered_t<Key, Ordered, Hash_Or_Comp>;

		/** @brief type traits for container */
		using key_type = Key;
		using mapped_type = var_data;
		using value_type = std::tuple<key_type, pin_type, mapped_type>;
		using hasher = Hash_Or_Comp;
		using key_compare = Hash_Or_Comp;
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

		/** @brief Default constructor */
		var_set(m_resource* mem = std::pmr::get_default_resource()) : vars{ mem } {}

		/**
		 * @brief Constructor from a variable file path.
		 * @throw Throws var_set_exception when the variable_set is not constructed properly
		 */
		var_set(const tpath& var_file, m_resource* mem = std::pmr::get_default_resource()) : var_set{ mem }
		{
			deserialize(var_file);
		}

		/**
		 * @brief Constructor from a variable file path.
		 * @throw Throws var_set_exception when the var_set is not constructed properly
		 */
		void deserialize(const tpath& var_file)
		{
			Serializer serializer;
			string json = serializer.GetJson(var_file);

			Reader reader(json.c_str());
			if (reader.HasError())
				throw var_set_exception(var_file.filename().string() + ": File cannot be opened!");

			deserialize(reader);
		}

		/**
		 * @brief Deserializes var_set data from a reader object.
 		 * @throw Throws var_set_exception when the var_set is not loaded properly
		 */
		void deserialize(Reader& reader)
		{
			clear(); // make sure set is cleared

			reader.StartObject("var_set");
			{
				// meta part
				{
					reader.StartMeta();
					{
						string meta;
						reader.Member("Type", meta);
						if (meta != "Variables")
							throw var_set_exception("var_set: Bad Meta Member!");
					}
					reader.EndMeta();
				}
				// variables part
				{
					size_t size = 0;
					reader.StartArray("Variables", &size);
					for (size_t i = 0; i < size; ++i)
					{
						reader.StartObject();
						{
							string key_string;
							reader.Member("Key", key_string);
							Key key = serializer()[key_string];
							var_data new_var;
							reader.Member("Data", new_var);

							if(!insert(key, std::move(new_var)))
								throw var_set_exception("var_set: Bad Variable Key! Key has already been loaded!");


						}
						reader.EndObject();
					}
					reader.EndArray();
				}
			}
			reader.EndObject();
		}

		/**
		 * @brief Serializes the var_set to a given path
		 * @throw Throws var_set_exception when the var_set is not constructed properly
		 */
		void serialize(const tpath& folder, const string& name) const
		{
			if (!Serializer::SaveJson(folder / (name + ".json"), ""))
				throw var_set_exception(name + ".json" + ": Invalid filename!");

			Writer writer;
			serialize(writer);

			if (!Serializer::SaveJson(folder / (name + ".json"), writer.GetString()))
				throw var_set_exception(name + ".json" + ": Failed to save!");
		}

		/**
		 * @brief Serializes the var_set to a writer object.
		 * @throw Throws var_set_exception when the var_set is not constructed properly
		 */
		void serialize(Writer& writer) const
		{
			writer.StartObject("var_set");
			{
				// meta part
				{
					writer.StartMeta();
					{
						writer.Member("Type", "Variables");
					}
					writer.EndMeta();
				}
				// variables part
				{
					writer.StartArray("Variables");
					for (auto& [key, var] : vars)
					{
						writer.StartObject();
						{
							string key_string = serializer()(key);
							writer.Member("Key", key_string);
							var_data temp = var;
							writer.Member("Data", temp);
						}
						writer.EndObject();
					}
					writer.EndArray();
				}
			}
			writer.EndObject();
		}

		/**
		 * @brief Return true if set is empty.
		 */
		void clear()
		{
			vars.clear();
		}

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
			if (!count(key)) return nullptr;
			
			return &vars.at(key);
		}
		[[nodiscard]] const var_data* at_if(const Key& key) const
		{
			if (!count(key)) return nullptr;

			return &vars.at(key);
		}

		[[nodiscard]] bool rekey(const Key& old_key, const Key& new_key)
		{
			if (!count(old_key)) return false;
			if (count(new_key)) return false;

			auto node = vars.extract(old_key);
			node.key() = new_key;
			vars.insert(std::move(node));

			return true;
		}

		/**
		 * @brief Inserts var_data at key and returns a pointer to the initialized
		 * value
		 * @param key Key of variable
		 * @param data Data to pass in
		 * @return True if insert successfully.
		 */
		var_data* insert(const Key& key, var_data data)
		{
			auto ret = vars.emplace(key, std::move(data));
			if (!ret.second)
				return nullptr;

			return &ret.first->second;
		}

		/**
		 * @brief Creates a variable of type T, initializes by data.
		 * @tparam T type of variable
		 * @param key Key of variable
		 * @param data Data to pass in
		 * @return Pointer to created variable if successful. Returns nullptr if fail.
		 */
		template <typename T>
		T* create(const Key& key, T data = T{})
		{
			auto ret = vars.emplace(key, var_data{ data });
			if (!ret.second) return nullptr;

			return ret.first->second.get_if<T>();
		}

		/**
		 * @brief Creates a variable of type in pin_type. Default initializes 
		 * variable.
		 * @param key Key of variable
		 * @param t Pin type
		 * @return True if variable created successfully.
		 */
		bool create(const Key& key, pin_type t)
		{
			auto ret = vars.emplace(key, var_data{ t });
			return ret.second;
		}

		/**
		 * @brief Erases key from the var set
		 * @param key Key to remove from the set
		 */
		void erase(const Key& key)
		{
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


	template <
		typename Key,
		typename Comp = std::less<Key>,
		typename Serial = serial<Key>
	>
	using ordered_var_set = var_set<Key, true, Comp, Serial>;

	template <
		typename Key,
		typename Hash = std::hash<Key>,
		typename Serial = serial<Key>
	>
	using unordered_var_set = var_set<Key, false, Hash, Serial>;
}