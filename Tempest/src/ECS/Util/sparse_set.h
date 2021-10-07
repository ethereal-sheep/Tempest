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

#include "../../Memory.h"
#include "../Entity.h"
#include "ECS/Components/Components.h"
#include "Util.h"

namespace Tempest
{

	class sparse_set_exception : public std::exception
	{
	public:
		sparse_set_exception(const string& err_msg = "sparse set exception thrown!") : msg(err_msg) {}
		const char* what() const noexcept override { return msg.c_str(); }
	private:
		string msg;
	};


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

		sparse_set() = delete;
		sparse_set(const string& type) : type_info{ type } {}
		virtual ~sparse_set() = default;

		/**
		 * @return Number of entities in the sparse set
		 */
		[[nodiscard]] virtual size_t size() const = 0;

		/**
		 * @return True if the sparse set is empty
		 */
		[[nodiscard]] virtual bool empty() const = 0;

		/**
		 * @brief Checks if the entity exist in the dense array. With sparse
		 * sets, this operation takes constant time.
		 * @param entity An entity identifier, either valid or not
		 * @return True if entity exists in the sparse set
		 */
		[[nodiscard]] virtual bool contains(Entity entity) const = 0;

		/**
		 * @brief Returns the extent (range of dense array) of the sparse set
		 * @return Pointer to underlying dense array container
		 */
		virtual const EnVector* extent() const = 0;

		/**
		 * @brief Finds the entity in the sparse set and returns an iterator
		 * to it. With sparse sets, this operation takes constant time.
		 * @param entity An entity identifier, either valid or not
		 * @return Iterator to the entity; end() is returned if it cannot be
		 * found
		 */
		virtual EnVector::const_iterator find(Entity entity) const = 0;

		/**
		 * @brief Copy a component from an existing entity. If the entity
		 * doesn't exists, or the new entity is already contained in the set,
		 *  nothing happens.
		 * @param old_e An entity identifier to clone from
		 * @param new_e An entity identifier to clone to
		 */
		virtual void clone(Entity old_e, Entity new_e) = 0;

		/**
		 * @brief Clears all entities in the sparse set
		 */
		virtual void clear() = 0;


		/**
		 * @brief Erases an entity from the sparse set. If it doesn't exist,
		 * nothing happens.
		 * @param entity An entity identifier, either valid or not
		 */
		virtual void erase(Entity entity) = 0;

		/**
		 * @brief Removes an entity from the sparse set and returns the success
		 * state
		 * @param entity An entity identifier, either valid or not
		 * @return Success state of the removal.
		 */
		virtual bool remove(Entity entity) = 0;

		/**
		 * @brief Serializes the sparse set into a json file at given path
		 * @param components_folder Folder to save the json file
		 * @return Success state of the saving
		 */
		virtual bool serialize(const tpath& components_folder) const = 0;


		string type_info;
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
		//using sparse_l = std::array<Entity, MAX_ENTITY>;
		using sparse_l = dense_type<Entity>;

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
		/**
		 * @brief Constructs the sparse set
		 * @param mem Pointer to a polymorphic memory resource; defaults to
		 * default resource provided by the standard library
		 */
		sparse_set_t(m_resource* mem = std::pmr::get_default_resource()) :
			sparse_set(Component::get_type()),
			entityList(mem),
			componentList(mem),
			sparse(MAX_ENTITY, 0, mem) {}

		/**
		 * @brief Clears the sparse set before destructing
		 */
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
			if (empty() || entity >= MAX_ENTITY || sparse[entity] >= size())
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
		 * @brief Copy a component from an existing entity. If the entity
		 * doesn't exists, or the new entity is already contained in the set,
		 *  nothing happens.
		 * @param old_e An entity identifier to clone from
		 * @param new_e An entity identifier to clone to
		 */
		void clone(Entity old_e, Entity new_e) override
		{
			// warn here
			if (contains(old_e) && !contains(new_e))
			{
				sparse[new_e] = static_cast<Entity>(entityList.size());
				entityList.push_back(new_e);
				componentList.push_back(componentList[sparse[old_e]]);
			}
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
		void erase(Entity entity) override
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
		bool remove(Entity entity) override
		{
			return contains(entity) ? (erase(entity), true) : false;
		}


		/**
		 * @brief Gets the component that belongs to the entity
		 * @param entity An entity identifier, either valid or not
		 * @throw sparse_set_exception if entity doesn't own component
		 * @return Pointer to the component; nullptr if entity doesn't exist
		 */
		component_type& get(Entity entity)
		{
			if (!contains(entity))
				throw sparse_set_exception(
					string("sparse_set: Failed to get component at entity ") + 
					std::to_string(entity));

			return componentList[sparse[entity]];
		}

		/**
		 * @brief Gets the component that belongs to the entity
		 * @param entity An entity identifier, either valid or not
		 * @return Pointer to the component; nullptr if entity doesn't exist
		 */
		component_type* get_if(Entity entity)
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

		/**
		 * @brief Serializes the sparse set into a json file at given path
		 * @param components_folder Folder to save the json file
		 * @return Success state of the saving
		 */
		bool serialize(const tpath& folder) const override
		{
			if (!entityList.size())
				return false;

			Writer writer;
			writer.StartObject();
			writer.StartMeta();
			writer.Member("Type", sparse_set::type_info);
			writer.EndMeta();

			writer.StartArray("Components");
			for (size_t i = 0; i < entityList.size(); ++i)
			{
				writer.StartObject();
				writer.Member("Entity", entityList[i]);
				using namespace Components;
				auto c = componentList[i];
				writer.Member("Component", c);
				writer.EndObject();
			}

			writer.EndArray();
			
			writer.EndObject();
			Serializer serializer;
			serializer.SaveJson(folder / (sparse_set::type_info + ".json"), writer.GetString());

			return true;
		}

	private:
		dense_el entityList;
		dense_cl componentList;
		sparse_l sparse = { 0 };
	};
}