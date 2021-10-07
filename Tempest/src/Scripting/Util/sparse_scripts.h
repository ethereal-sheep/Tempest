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

#include "Memory.h"
#include "Util.h"
#include "ECS/Entity.h"
#include "script.h"

namespace Tempest
{
	class sparse_scripts_exception : public std::exception
	{
	public:
		sparse_scripts_exception(const string& err_msg = "sparse_scripts exception thrown!") : msg(err_msg) {}
		const char* what() const noexcept override { return msg.c_str(); }
	private:
		string msg;
	};


	/**
	 * @brief Sparse Set data structure specifically for Entity type and scripts. Provides 
	 * Contant time insert, remove, and lookup, while providing locality of 
	 * reference.
	 * We cannot use ECS sparse due to different requirements.
	 */
	class sparse_scripts
	{
		/** @brief typedefs for regularly used types */
		template<typename T>
		using dense_type = tvector<T>;

		using dense_el = dense_type<id_t>;
		using dense_sl = dense_type<script*>;
		using sparse_l = dense_type<id_t>;

	protected:
		void swap(id_t lhs, id_t rhs)
		{
			auto from = sparse[lhs];
			auto to = sparse[rhs];

			std::swap(sparse[lhs], sparse[rhs]);
			std::swap(entityList[from], entityList[to]);
			std::swap(scriptList[from], scriptList[to]);
		}

    public:
		/**
		 * @brief Constructs the sparse set
		 * @param mem Pointer to a polymorphic memory resource; defaults to
		 * default resource provided by the standard library
		 */
		sparse_scripts(m_resource* mem = std::pmr::get_default_resource()) :
			entityList(mem),
			scriptList(mem),
			sparse(MAX_ENTITY, 0, mem) {}

		/**
		 * @brief Clears the sparse set before destructing
		 */
		~sparse_scripts() { clear(); }

		/**
		 * @brief iterator type for the split_view object
		 * @tparam It templated iterator types
		 */
		template <typename It1, typename It2>
		class sparse_iterator final
		{
			friend class sparse_scripts;

			/**
			 * @brief Constructs the view iterator
			 * @param curr curr iterator from the parent class
			 */
			sparse_iterator(It1 curr1, It2 curr2) : it1{ curr1 }, it2{ curr2 }
			{
			}

		public:
			/** @brief ttype traits for iterator */
			using difference_type = size_t;
			using value_type = tpair<typename It1::value_type, typename It2::value_type>;
			using pointer = value_type*;
			using reference = value_type&;
			using iterator_category = std::forward_iterator_tag;

			sparse_iterator() = delete;


			/** @brief Standard boiler plate for iterators but we do a check
			 * each time we change the position of iterator */
			sparse_iterator& operator++()
			{
				return ++it1, ++it2, *this;
			}

			/** @brief Standard boiler plate for iterators  */
			sparse_iterator operator++(int)
			{
				sparse_iterator orig = *this;
				return ++(*this), orig;
			}

			/** @brief Standard boiler plate for iterators  */
			[[nodiscard]] bool operator==(const sparse_iterator& rhs) const
			{
				return rhs.it1 == it1 || rhs.it2 == it2;
			}

			/** @brief Standard boiler plate for iterators  */
			[[nodiscard]] bool operator!=(const sparse_iterator& rhs) const
			{
				return !(*this == rhs);
			}

			/** @brief Standard boiler plate for iterators. Here we split the value
			and set a mutable temp variable so we can maintain const-correctness. */
			[[nodiscard]] pointer operator->() const
			{
				temp = std::make_pair(*it1, *it2);
				return &temp;
			}

			/** @brief Standard boiler plate for iterators  */
			[[nodiscard]] reference operator*() const
			{
				return *operator->();
			}

		private:
			It1 it1;
			It2 it2;
			mutable value_type temp;
		};

		using iterator = sparse_iterator<dense_el::const_iterator, dense_sl::const_iterator>;

		/**
		 * @return Number of entities in the sparse set
		 */
		size_t size() const
		{ 
			return entityList.size(); 
		}

		/**
		 * @return True if the sparse set is empty
		 */
		bool empty() const
		{ 
			return entityList.empty();
		}

		/**
		 * @brief Checks if the entity exist in the dense array. With sparse
		 * sets, this operation takes constant time.
		 * @param entity An entity identifier, either valid or not
		 * @return True if entity exists in the sparse set
		 */
		bool contains(Entity entity) const
		{
			if (empty())
				return false;

			return entityList[sparse[entity]] == entity;
		}

		/**
		 * @brief Clears all entities in the sparse set
		 */
		void clear()
		{
			// destroy the scripts first
			scriptList.clear();
			// destroy the entities
			entityList.clear();
		}


		/**
		 * @brief Emplaces an entity and component into the sparse set. If the 
		 * entity already exists, nothing happens.
		 * @tparam TArgs... Parameter pack
		 * @param entity An entity identifier, either valid or not
		 * @param args Parameter pack of arguments, forwarded to the 
		 * component's constructor
		 */
		void emplace(Entity entity, script* s)
		{
			// warn here
			if (!contains(entity))
			{
				sparse[entity] = static_cast<Entity>(entityList.size());
				entityList.push_back(entity);
				scriptList.push_back(s);
			}
		}

		/**
		 * @brief Erases an entity from the sparse set. If it doesn't exist,
		 * nothing happens.
		 * @param entity An entity identifier, either valid or not
		 */
		void erase(Entity entity)
		{
			// warn here
			if (contains(entity))
			{
				auto other = entityList.back();
				swap(entity, other);

				sparse[entity] = INVALID;

				entityList.pop_back();
				scriptList.pop_back();
			}
		}

		/**
		 * @brief Removes an entity from the sparse set and returns the success
		 * state
		 * @param entity An entity identifier, either valid or not
		 * @return Success state of the removal.
		 */
		bool remove(Entity entity)
		{
			return contains(entity) ? (erase(entity), true) : false;
		}


		/**
		 * @brief Gets the component that belongs to the entity
		 * @param entity An entity identifier, either valid or not
		 * @throw sparse_set_exception if entity doesn't own component
		 * @return Pointer to the component; nullptr if entity doesn't exist
		 */
		script* get(Entity entity)
		{
			if (!contains(entity))
				throw sparse_scripts_exception(
					string("sparse_set: Failed to get script at entity ") + 
					std::to_string(entity));

			return scriptList[sparse[entity]];
		}

		/**
		 * @brief TODO: Sorts the sparse set. Default ascending.
		 * @tparam Pred Predicate function type
		 * @param pred Predicate function object; default std::less
		 */
		template<typename Pred>
		void sort(Pred pred = std::less<Entity>()) {/*TODO*/ }


		/**
		 * @brief Gets the component that belongs to the entity
		 * @param entity An entity identifier, either valid or not
		 * @throw sparse_set_exception if entity doesn't own component
		 * @return Pointer to the component; nullptr if entity doesn't exist
		 */
		iterator begin() const
		{
			return iterator( entityList.begin(), scriptList.begin() );
		}

		/**
		 * @brief Gets the component that belongs to the entity
		 * @param entity An entity identifier, either valid or not
		 * @throw sparse_set_exception if entity doesn't own component
		 * @return Pointer to the component; nullptr if entity doesn't exist
		 */
		iterator end() const
		{
			return iterator( entityList.end(), scriptList.end() );
		}

	private:
		dense_el entityList;
		dense_sl scriptList;
		sparse_l sparse = { 0 };
	};
}