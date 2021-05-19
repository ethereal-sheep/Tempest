#pragma once

#include "../Entity.h"

namespace Tempest
{
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
	};

	template <typename Component>
	class sparse_set_t : public sparse_set
	{

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
		~sparse_set_t() = default;


        using component_type = Component;

		size_t size() const override
		{ 
			return entityList.size(); 
		}

		bool empty() const override
		{ 
			return entityList.empty();
		}

		bool contains(Entity entity) const override
		{
			if (empty())
				return false;
			return entityList[sparse[entity]] == entity;
		}

		const EnVector* extent() const override
		{
			return &entityList;
		}

		EnVector::const_iterator find(Entity entity) const override
		{
			if (!contains(entity))
				return entityList.cend();

			return entityList.cbegin() + sparse[entity];
		}

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


		void clear() 
		{
			entityList.clear();
			componentList.clear();
		}

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

		bool remove(Entity entity)
		{
			return contains(entity) ? (erase(entity), true) : false;
		}

		component_type* get(Entity entity)
		{
			if (!contains(entity))
				return nullptr;

			return &componentList[sparse[entity]];
		}

		void sort() {/*TODO*/ }


	private:
		dense_el entityList;
		dense_cl componentList;
		sparse_l sparse = { 0 };
	};



	template<typename SysCall>
	void for_entities(SysCall process)
	{
		/*for (auto id : dense)
			process(id);*/
	}
}