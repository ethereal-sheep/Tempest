#pragma once

#include "Util/registry.h"
#include "Util/sparse_set.h"
#include "Entity.h"

namespace Tempest
{
	class view;

	template <typename T>
	size_t t_hash()
	{
		return typeid(T).hash_code();
	}

	class ECS
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

	public:
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

		template <typename Component>
		bool has(Entity entity) const
		{
			// check if component pool exist
			if (!component_exist<Component>())
				return false; // warn here
			
			return get_sparse<Component>()->contains(entity);
		}

		template <typename Component>
		Component* get(Entity entity)
		{
			if (!component_exist<Component>())
				return nullptr;

			return get_sparse<Component>()->get(entity);

		}

		template <typename Component, typename... TArgs>
		Component* emplace(Entity entity, TArgs... args)
		{
			if (!valid(entity) || !component_exist<Component>() || has<Component>(entity))
				return nullptr; // warn here

			auto sparse = get_sparse<Component>();
			sparse->emplace(entity, std::forward<TArgs>(args)...);
			return sparse->get(entity);
		}

		template <typename Component, typename... TArgs>
		Component* replace(Entity entity, TArgs... args)
		{
			if (!valid(entity) || !component_exist<Component>() || !has<Component>(entity))
				return nullptr; // warn here

			return &(*(get_sparse<Component>()->get(entity)) = Component{ args... });
		}

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

		template<typename Component>
		bool component_exist() const
		{
			return component_pools.count(t_hash<Component>());
		}

		template <typename Component>
		view all()
		{

		}
		template <typename... Components>
		view all()
		{

		}

		bool valid(Entity entity) const
		{
			return entity_registry.valid(entity);
		}

		Entity create()
		{
			return entity_registry.create();
		}

		bool destroy(Entity entity)
		{
			return entity_registry.destroy(entity);
		}

	private:
		tmap<size_t, tuptr<sparse_set>> component_pools;
		registry entity_registry;
	};


}