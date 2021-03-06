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

#include "Util/registry.h"
#include "Util/sparse_set.h"
#include "Util/type_traits.h"
#include "Util/runtime_view.h"

#include "Components/Components.h"
#include "Entity.h"
#include "Util/prefab.h"

#include "Util.h"

namespace Tempest
{
	static const char* default_folder = "Components";


	class ECS_exception : public std::exception
	{
	public:
		ECS_exception(const string& err_msg = "ECS exception thrown!") : msg(err_msg) {}
		const char* what() const noexcept override { return msg.c_str(); }
	private:
		string msg;
	};

	/**
	 * @brief Specific type for view exclusion
	 */
	template<typename... type>
	struct exclude_t : type_list<type...> {};

	/**
	 * @brief Entity Component System to manage all components in a certain 
	 * instance.
	 * Handles registering of components, creation/destruction of entities, 
	 * assigning components to entities etc.
	 */
	class ECS final
	{
		template <typename T>
		size_t t_hash() const
		{
			return typeid(T).hash_code();
		}

		template<typename Component>
		sparse_set_t<Component>* get_sparse()
		{
			if (!component_exist<Component>())
				return nullptr;

			return static_cast<sparse_set_t<Component>*>(
				component_pools.at(t_hash<Component>()).get());
		}

		template<typename Component>
		const sparse_set_t<Component>* get_sparse() const
		{
			if (!component_exist<Component>())
				return nullptr;

			return static_cast<sparse_set_t<Component>*>(
				component_pools.at(t_hash<Component>()).get());
		}

		template<typename... Components>
		constexpr void package(tvector<size_t>& v) const
		{
			(v.push_back(t_hash<Components>()), ...);
		}

	public:
		static const bool is_entity_keyed = true;

		/**
		 * @brief Constructs an ECS object
		 * @param mem Pointer to a polymorphic memory resource; default gets
		 * the default memory resource
		 */
		ECS(m_resource* mem = std::pmr::get_default_resource(),
			string components_folder = default_folder) :
			memory_resource(mem),
			entity_registry(mem),
			component_pools(mem) {}

		/**
		 * @brief Registers a component to the ECS
		 * @tparam Component Valid component type; type must be default, copy, 
		 * and move constructible, and their assign counterparts
		 */
		template <typename Component>
		void register_component()
		{
			static_assert(std::is_default_constructible_v<Component>);
			static_assert(std::is_copy_constructible_v<Component>);
			static_assert(std::is_copy_assignable_v<Component>);
			static_assert(std::is_move_constructible_v<Component>);
			static_assert(std::is_move_assignable_v<Component>);

			if (component_exist<Component>()) return;
				// warning here

			component_pools.insert({ t_hash<Component>(), make_uptr<sparse_set_t<Component>>(memory_resource)});
		}

		/**
		 * @brief Checks if entity has a component
		 * @tparam Component Valid component type
		 * @param entity An entity identifier, either valid or not
		 * @return True if entity has the component; False if not
		 */
		template <typename Component>
		[[nodiscard]] bool has(Entity entity) const
		{
			// check if component pool exist
			if (!component_exist<Component>())
				return false;

			return get_sparse<Component>()->contains(entity);
		}

		/**
		 * @brief Returns the component owned by entity
		 * @tparam Component Valid component type
		 * @throw sparse_set_exception if entity doesn't own component
		 * @throw index_out_of_range_exception if component sparse_set doesn't exist
		 * @param entity An entity identifier, either valid or not
		 * @return Reference to entity component
		 */
		template <typename Component>
		[[nodiscard]] Component& get(Entity entity)
		{
			return get_sparse<Component>()->get(entity);
		}

		/**
		 * @brief Returns the component owned by entity
		 * @tparam Component Valid component type
		 * @param entity An entity identifier, either valid or not
		 * @return Pointer to the component; nullptr if it doesn't exist
		 */
		template <typename Component>
		[[nodiscard]] Component* get_if(Entity entity)
		{
			if (!component_exist<Component>())
				return nullptr;

			return get_sparse<Component>()->get_if(entity);
		}

		/**
		 * @brief Emplaces a component to the entity; entity must not own the
		 * component
		 * @tparam Component Valid component type
		 * @tparam Targs... Parameter pack
		 * @param entity An entity identifier, either valid or not.
		 * @param args Variadic arguments to be passed to the component's 
		 * constructor
		 * @return Pointer to the emplaced component; nullptr if failed
		 */
		template <typename Component, typename... TArgs>
		Component* emplace(Entity entity, TArgs... args)
		{
			if (!valid(entity))
				return nullptr; // warn here

			// register component if it doesn't exist
			register_component<Component>();

			if (has<Component>(entity))
				return nullptr;

			auto sparse = get_sparse<Component>();
			sparse->emplace(entity, std::forward<TArgs>(args)...);
			return sparse->get_if(entity);
		}
		/**
		 * @brief Replaces an entity's component; entity must currently own
		 * the component
		 * @tparam Component Valid component type
		 * @tparam Targs... Parameter pack
		 * @param entity An entity identifier, either valid or not.
		 * @param args Variadic arguments to be passed to the component's 
		 * constructor
		 * @return Pointer to the replaced component; nullptr if failed
		 */
		template <typename Component, typename... TArgs>
		Component* replace(Entity entity, TArgs... args)
		{
			if (!valid(entity))
				return nullptr; // warn here

			// register component if it doesn't exist
			register_component<Component>();

			if (!has<Component>(entity))
				return nullptr;

			return &(*(get_sparse<Component>()->get(entity)) = Component{ args... });
		}

		/**
		 * @brief Tries to replace the component in an entity; if it doesn't 
		 * exist, it is emplaced instead
		 * @tparam Component Valid component type
		 * @tparam Targs... Parameter pack
		 * @param entity An entity identifier, either valid or not.
		 * @param args Variadic arguments to be passed to the component's 
		 * constructor
		 * @return Pointer to the emplaced/replaced component; nullptr 
		 * if failed
		 */
		template <typename Component, typename... TArgs>
		Component* replace_else_emplace(Entity entity, TArgs... args)
		{
			if (!valid(entity))
				return nullptr; // warn here

			// register component if it doesn't exist
			register_component<Component>();

			if (has<Component>(entity))
				return replace(entity, std::forward<TArgs>(args)...);
			else
				return emplace(entity, std::forward<TArgs>(args)...);
		}

		/**
		 * @brief Tries to erase a component from an entity
		 * @tparam Component Valid component type
		 * @param entity An entity identifier, either valid or not
		 */
		template <typename Component>
		void erase(Entity entity)
		{
			if (!valid(entity) || !component_exist<Component>())
				return;
			
			get_sparse<Component>()->erase(entity);
		}

		/**
		 * @brief Tries to erase a component from an entity and returns the
		 * success state
		 * @tparam Component Valid component type
		 * @param entity An entity identifier, either valid or not
		 * @return Success state of the component removal
		 */
		template <typename Component>
		bool remove(Entity entity)
		{
			if (!valid(entity) || !component_exist<Component>())
				return false;
			
			return get_sparse<Component>()->remove(entity);
		}

		/**
		 * @brief Checks if the component is registered in the ECS
		 * @tparam Component Valid component type
		 * @return True if component has been registered; False if not
		 */
		template<typename Component>
		[[nodiscard]] bool component_exist() const
		{
			return component_pools.count(t_hash<Component>());
		}

		/**
		 * @brief Returns an iterable object that iterates a subset of the 
		 * given parameters; types included must be unique
		 * @tparam Components... Valid component types; includes in subset
		 * @tparam Exclude... Valid component type; excludes in subset
		 * @param exclude_t<Exclude...> excludes types in subset
		 * @return Constructed runtime_view Object
		 */
		template<typename... Components, typename... Exclude>
		[[nodiscard]] runtime_view view(exclude_t<Exclude...> = {})
		{
			// check if types are unique
			//[[maybe_unused]] unique_types<> useless;

			static_assert(!is_any<Components..., Exclude...>(), "Components must be unique");
			static_assert(type_list<Components...>::size != 0, "No empty set");

			tvector<size_t> inc, exc;
			package<Components...>(inc);
			package<Exclude...>(exc);

			return runtime_view(std::move(inc), std::move(exc), component_pools);
		}

		template<typename... Components, typename... Exclude>
		[[nodiscard]] Entity view_first(exclude_t<Exclude...> = {})
		{
			// check if types are unique
			//[[maybe_unused]] unique_types<> useless;

			static_assert(!is_any<Components..., Exclude...>(), "Components must be unique");
			static_assert(type_list<Components...>::size != 0, "No empty set");

			tvector<size_t> inc, exc;
			package<Components...>(inc);
			package<Exclude...>(exc);

			auto view = runtime_view(std::move(inc), std::move(exc), component_pools);

			if (view.begin() == view.end())
				return INVALID;
			return *view.begin();
		}

		template<typename... Components, typename... Exclude>
		[[nodiscard]] Entity view_first_or(exclude_t<Exclude...> = {}, Entity id = INVALID)
		{
			auto first = view_first<Components..., Exclude...>();
			return first ? first : id;
		}

		/**
		 * @brief Checks if entity is valid or not
		 * @param entity An entity identifier, either valid or not
		 * @return True if entity is valid; False if not
		 */
		[[nodiscard]] bool valid(Entity entity) const
		{
			return entity_registry.valid(entity);
		}

		/**
		 * @return Number of live entities in the ECS
		 */
		[[nodiscard]] size_t size() const
		{
			return entity_registry.size();
		}

		/**
		 * @return Number of availabe entities in the ECS
		 */
		[[nodiscard]] size_t available() const
		{
			return entity_registry.available();
		}

		/**
		 * @brief Creates a new empty entity
		 * @return New entity identifier
		 */
		[[nodiscard]] Entity create()
		{
			return entity_registry.create();
		}

		/**
		 * @brief Creates a new entity from prefab
		 * @return New entity identifier
		 */
		[[nodiscard]] Entity create(const prefab& p)
		{
			auto e = create();
			for (auto& [hash, compo] : p.components)
			{
				if (!compo)
					continue;

				compo->create(e, component_pools[hash], memory_resource);
			}
			return e;
		}

		/**
		 * @brief Clones a new entity given an existing entity, with some 
		 * exclusion if needed
		 * @warning USE SPARINGLY IF YOU HAVE A LOT OF COMPONENT TYPES
		 * @tparam Exclude... Valid component types
		 * @param entity An entity identifier, either valid or not
		 * @param exclude_t<Exclude...> excludes types from entity
		 * @return New cloned entity identifier
		 */
		template<typename... Exclude>
		[[nodiscard]] Entity clone(Entity entity, exclude_t<Exclude...> = {})
		{
			// make sure unique
			//static_assert(!is_any<Components..., Exclude...>(), "Components must be unique");
			// create new entity
			Entity new_e = entity_registry.create();
			// package exclude components
			tvector<size_t> exc;
			package<Exclude...>(exc);

			// clone for each component type registered
			for (auto& [hash, sparse] : component_pools)
				sparse->clone(entity, new_e);

			// remove at every component type specified
			// we check to make sure the pool exist first
			for (auto i : exc)
				if(component_pools.count(i))
					component_pools.at(i)->erase(new_e);

			return new_e;
		}

		/**
		 * @brief Destroys the entity and returns the success state
		 * @param entity An entity identifier, either valid or not
		 * @return Success state of the destruction
		 */
		bool destroy(Entity entity)
		{
			for (auto& [hash, pool] : component_pools)
				pool->erase(entity);

			return entity_registry.destroy(entity);
		}

		/**
		 * @brief Clears all live entities and their components.
		 */
		void clear()
		{
			// destroy components first
			for(auto& [hash, sparse_ptr] : component_pools)
				sparse_ptr->clear();
			
			// destroy entities
			entity_registry.clear();
		}

		/**
		 * @brief Resets the ECS. All components and entities are removed and 
		 * cannot be accessed after this.
		 */
		void reset()
		{
			// destroy component ppols first
			component_pools.clear();

			// destroy entities
			entity_registry.clear();
		}


		/**
		 * @brief Force creates an entity in the registry
		 * @warning Registry must be cleaned after to maintain safe id!
		 * @return New entity identifier
		 */
		Entity force_create(Entity entity)
		{
			return entity_registry.force_create(entity);
		}


		/**
		 * @brief Cleans the available pool to maintain safe id
		 * @warning Registry must be cleaned after any force_create!
		 */
		void clean_registry()
		{
			entity_registry.clean();
		}

		/**
		 * @brief Saves the ECS to a components folder in the filepath 
		 * provided. 
		 * If folder doesn't exist, a folder is created. 
		 * If it exists, the folder's contents will be overwritten.
		 */
		void save(const tpath& root_filepath, const string& save_folder = default_folder) const
		{
			tpath folder = root_filepath / save_folder;

			// if directory doesn't exist, create new_directory
			if(!std::filesystem::exists(folder))
				std::filesystem::create_directories(folder);
			
			// delete everything in components
			for(auto file : std::filesystem::directory_iterator(folder))
				std::filesystem::remove_all(file.path());


			for (auto& [hash, sparse] : component_pools)
			{
				sparse->serialize(folder);
			}
		}

		/**
		 * @brief Loads the ECS from an components folder. 
		 * If folder doesn't exist, a folder is created.
		 * If it exists, the folder's contents will be overwritten.
		 */
		void load(const tpath& root_filepath, const string& load_folder = default_folder)
		{
			tpath folder = root_filepath / load_folder;

			// check if file path exists
			if (!std::filesystem::exists(folder))
				return; // warn here

			// else, we make sure ECS is empty first
			clear();

			// for each file in the folder, we try to deserialize
			for (auto file : std::filesystem::directory_iterator(folder))
			{
				deserialize_component(file, *this);
			}
			// cleans registry since some entities have been forced created
			clean_registry();
		}


	private:

		m_resource* memory_resource;
		registry entity_registry;
		tmap<size_t, tuptr<sparse_set>> component_pools;
		string components_folder;
	};


}