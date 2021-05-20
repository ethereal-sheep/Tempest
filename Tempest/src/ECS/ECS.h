#pragma once

#include "Util/registry.h"
#include "Util/sparse_set.h"
#include "Util/type_traits.h"
#include "Util/runtime_view.h"
#include "Entity.h"

namespace Tempest
{
	template <typename T>
	size_t t_hash()
	{
		return typeid(T).hash_code();
	}

	/**
	 * @brief Entity Component System to manage all components in a certain 
	 * instance.
	 * Handles registering of components, creation/destruction of entities, 
	 * assigning components to entities etc.
	 */
	class ECS final
	{
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

			component_pools.insert({ t_hash<Component>(), make_ptr<sparse_set_t<Component>>()});
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
				return false; // warn here

			return get_sparse<Component>()->contains(entity);
		}

		/**
		 * @brief Returns the component owned by entity
		 * @tparam Component Valid component type
		 * @param entity An entity identifier, either valid or not
		 * @return Pointer to the component; nullptr if it doesn't exist
		 */
		template <typename Component>
		[[nodiscard]] Component* get(Entity entity)
		{
			if (!component_exist<Component>())
				return nullptr;

			return get_sparse<Component>()->get(entity);

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
			if (!valid(entity) || !component_exist<Component>() || has<Component>(entity))
				return nullptr; // warn here

			auto sparse = get_sparse<Component>();
			sparse->emplace(entity, std::forward<TArgs>(args)...);
			return sparse->get(entity);
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
			if (!valid(entity) || !component_exist<Component>() || !has<Component>(entity))
				return nullptr; // warn here

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
			if (!valid(entity) || !component_exist<Component>())
				return nullptr; // warn here

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
		[[nodiscard]] runtime_view view(exclude_t<Exclude...> = {}) const
		{
			// check if types are unique
			unique_types<Components..., Exclude...>();
			static_assert(type_list<Components...>::size != 0, "No empty set");

			tvector<size_t> inc, exc;
			package<Components...>(inc);
			package<Exclude...>(exc);

			return runtime_view(std::move(inc), std::move(exc), component_pools);
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
		 * @brief Destroys the entity and returns the success state
		 * @param entity An entity identifier, either valid or not
		 * @return Success state of the destruction
		 */
		bool destroy(Entity entity)
		{
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

	private:
		tmap<size_t, tuptr<sparse_set>> component_pools;
		registry entity_registry;
	};


}