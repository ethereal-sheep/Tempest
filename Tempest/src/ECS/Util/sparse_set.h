#pragma once

#include "../Entity.h"

namespace Tempest
{
	/**
	 * @brief Sparse Set data structure specifically for Entity type. Provides 
	 * Contant time insert, remove, and lookup, while providing locality of 
	 * reference.
	 * Here we use a pure virtual interface class so we can store pools in
	 * the ECS in a single structure.
	 */
	struct sparse_set
	{
		template<typename T>
		using dense_type = tvector<T>;

		sparse_set() = default;
		virtual ~sparse_set() = default;

		[[nodiscard]] virtual size_t size() const = 0;
		[[nodiscard]] virtual bool empty() const = 0;
		[[nodiscard]] virtual bool contains(Entity entity) const = 0;
		virtual const EnVector* extent() const = 0;
		virtual EnVector::const_iterator find(Entity entity) const = 0;
		virtual void clear() = 0;
	};

	/**
	 * @brief Sparse Set data structure specifically for Entity type. Provides 
	 * Contant time insert, remove, and lookup, while providing locality of 
	 * reference.
	 * Inherits from the sparse_set interface.
	 */
	template <typename Component>
	class sparse_set_t : public sparse_set
	{
		/** @brief typedefs for regularly used types */
		using dense_el = dense_type<Entity>;
		using dense_cl = dense_type<Component>;
		using sparse_l = std::array<Entity, MAX_ENTITY>;

	protected:
		void swap(Entity lhs, Entity rhs)
		{
			auto from = sparse[lhs];
			auto to = sparse[rhs];

			std::swap(sparse[lhs], sparse[rhs]);
			std::swap(entityList[from], entityList[to]);
			std::swap(componentList[from], componentList[to]);
		}

    public:
		sparse_set_t() = default;
		~sparse_set_t() { clear(); }

        using component_type = Component;

		/**
		 * @return Number of entities in the sparse set
		 */
		size_t size() const override
		{ 
			return entityList.size(); 
		}

		/**
		 * @return True if the sparse set is empty
		 */
		bool empty() const override
		{ 
			return entityList.empty();
		}

		/**
		 * @brief Checks if the entity exist in the dense array. With sparse
		 * sets, this operation takes constant time.
		 * @param entity An entity identifier, either valid or not
		 * @return True if entity exists in the sparse set
		 */
		bool contains(Entity entity) const override
		{
			if (empty())
				return false;
			return entityList[sparse[entity]] == entity;
		}

		/**
		 * @brief Returns the extent (range of dense array) of the sparse set
		 * @return Pointer to underlying dense array container
		 */
		const EnVector* extent() const override
		{
			return &entityList;
		}

		/**
		 * @brief Finds the entity in the sparse set and returns an iterator
		 * to it. With sparse sets, this operation takes constant time.
		 * @param entity An entity identifier, either valid or not
		 * @return Iterator to the entity; end() is returned if it cannot be
		 * found
		 */
		EnVector::const_iterator find(Entity entity) const override
		{
			if (!contains(entity))
				return entityList.cend();

			return entityList.cbegin() + sparse[entity];
		}

		/**
		 * @brief Clears all entities in the sparse set
		 */
		void clear() override
		{
			// destroy the components first
			componentList.clear();
			// destroy the entities
			entityList.clear();
		}

		/**
		 * @brief Inserts an entity into the sparse set. If the entity already
		 * exists, nothing happens.
		 * @param entity An entity identifier, either valid or not
		 */
		void insert(Entity entity)
		{
			// warn here
			if (!contains(entity))
			{
				sparse[entity] = static_cast<Entity>(entityList.size());
				entityList.push_back(entity);
				componentList.push_back(Component());
			}
		}

		/**
		 * @brief Emplaces an entity and component into the sparse set. If the 
		 * entity already exists, nothing happens.
		 * @tparam TArgs... Parameter pack
		 * @param entity An entity identifier, either valid or not
		 * @param args Parameter pack of arguments, forwarded to the 
		 * component's constructor
		 */
		template <typename... TArgs>
		void emplace(Entity entity, TArgs&&... args)
		{
			// warn here
			if (!contains(entity))
			{
				sparse[entity] = static_cast<Entity>(entityList.size());
				entityList.push_back(entity);
				componentList.emplace_back(std::forward<TArgs>(args)...);
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
				componentList.pop_back();
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
		 * @return Pointer to the component; nullptr if entity doesn't exist
		 */
		component_type* get(Entity entity)
		{
			if (!contains(entity))
				return nullptr;

			return &componentList[sparse[entity]];
		}
		/**
		 * @brief TODO: Sorts the sparse set. Default ascending.
		 * @tparam Pred Predicate function type
		 * @param pred Predicate function object; default std::less
		 */
		template<typename Pred>
		void sort(Pred pred = std::less<Entity>()) {/*TODO*/ }


	private:
		dense_el entityList;
		dense_cl componentList;
		sparse_l sparse = { 0 };
	};
}