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

#include "../../Core.h"
#include "../../Memory.h"
#include "sparse_set.h"

namespace Tempest
{
	/**
	 * @brief An iterable object that allows for range access to the specified
	 * entities with the correct included and excluded components
	 */
	class runtime_view
	{
		using map_ref = tmap<size_t, tuptr<sparse_set>>&;

		template<typename Component>
		sparse_set_t<Component>* get_sparse()
		{
			if (!component_exist<Component>())
				return nullptr;

			return static_cast<sparse_set_t<Component>*>(
				pools.at(typeid(Component).hash_code()).get());
		}

		template<typename Component>
		const sparse_set_t<Component>* get_sparse() const
		{
			if (!component_exist<Component>())
				return nullptr;

			return static_cast<sparse_set_t<Component>*>(
				pools.at(typeid(Component).hash_code()).get());
		}



	public:
		/**
		 * @brief Constructs a runtime view object with include, exclude lists
		 * and a const reference to the pools
		 * @param include_l list of include keys
		 * @param exclude_l list of exclude keys
		 * @param pools_l reference to a pool of sparse sets
		 */
		runtime_view(tvector<size_t> include_l, tvector<size_t> exclude_l, map_ref pools_l) 
			:include{ std::move(include_l) }, exclude{ std::move(exclude_l) }, pools{ pools_l } 
		{
			// preprocessing
			auto lambda = [&](auto i) {
				return !pools.count(i) || pools.at(i) == nullptr;
			};

			for (auto i : include)
				if (lambda(i))
				{
					include.clear();
					return;
				}

			//include.erase(std::remove_if(include.begin(), include.end(), lambda), include.end());
			exclude.erase(std::remove_if(exclude.begin(), exclude.end(), lambda), exclude.end());

			// guaranteed all hash exist
			auto it = std::min_element(include.begin(), include.end(), [&](auto h1, auto h2)
				{
					return pools.at(h1)->size() < pools.at(h2)->size();
				});

			// if iterable exists
			if (it != include.end())
			{
				pool_id = *it;
				iterable = pools.at(pool_id).get()->extent();
			}

		}


		/**
		 * @brief iterator type for the runtime_view object
		 * @tparam It templated iterator types
		 */
		template <typename It>
		class view_iterator final
		{
			friend class runtime_view;

			/**
			 * @brief Check if curr is valid by checking if the underlying
			 * entity contains the required include components and not the 
			 * excluded components
			 * @return True or False
			 */
			bool valid() const {
				const auto entity = *it;

				for (auto i : exclude) if (pools.at(i)->contains(entity)) return false;
				for (auto i : include) if (!pools.at(i)->contains(entity)) return false;

				return true;
			}

			/**
			 * @brief Constructs the view iterator
			 * @param from begin iterator from the parent class
			 * @param to end iterator from the parent class
			 * @param curr curr iterator from the parent class
			 * @param include_l const ref to the parent's exclude list
			 * @param exclude_l const ref to the parent's exclude list
			 * @param pools_l const ref to the parent's pool
			 * 
			 */
			view_iterator(It from, It to, It curr,
				const tvector<size_t>& exclude_l,
				const tvector<size_t>& include_l,
				map_ref pools_l) :
				first{ from },
				last{ to },
				it{ curr },
				include{ include_l },
				exclude{ exclude_l },
				pools{ pools_l }
			{
				// if current isn't valid, iterate it until it is
				while (it != last && !valid()) {
					++(*this);
				}
			}

		public:
			/** @brief ttype traits for iterator */
			using difference_type = size_t;
			using value_type = Entity;
			using pointer = const value_type*;
			using reference = const value_type&;
			using iterator_category = std::bidirectional_iterator_tag;

			view_iterator() = delete;

			
			/** @brief Standard boiler plate for iterators but we do a check
			 * each time we change the position of iterator */
			view_iterator& operator++() 
			{
				while (++it != last && !valid());
				return *this;
			}

			/** @brief Standard boiler plate for iterators but we do a check
			 * each time we change the position of iterator */
			view_iterator operator++(int) 
			{
				view_iterator orig = *this;
				return ++(*this), orig;
			}

			/** @brief Standard boiler plate for iterators but we do a check
			 * each time we change the position of iterator */
			view_iterator& operator--() 
			{
				while (--it != first && !valid());
				return *this;
			}

			/** @brief Standard boiler plate for iterators but we do a check
			 * each time we change the position of iterator */
			view_iterator operator--(int) 
			{
				view_iterator orig = *this;
				return operator--(), orig;
			}

			/** @brief Standard boiler plate for iterators but we do a check
			 * each time we change the position of iterator */
			[[nodiscard]] bool operator==(const view_iterator& rhs) const 
			{
				return rhs.it == it;
			}

			/** @brief Standard boiler plate for iterators but we do a check
			 * each time we change the position of iterator */
			[[nodiscard]] bool operator!=(const view_iterator& rhs) const
			{
				return !(*this == rhs);
			}

			/** @brief Standard boiler plate for iterators but we do a check
			 * each time we change the position of iterator */
			[[nodiscard]] pointer operator->() const 
			{
				return &*it;
			}

			/** @brief Standard boiler plate for iterators but we do a check
			 * each time we change the position of iterator */
			[[nodiscard]] reference operator*() const 
			{
				return *operator->();
			}

		private:
			It first;
			It last;
			It it;
			const tvector<size_t>& include;
			const tvector<size_t>& exclude;
			map_ref pools;
		};

		/** @brief type traits for container */
		using value_type = Entity;
		using size_type = std::size_t;
		using iterator = view_iterator<EnVector::const_iterator>;
		using reverse_iterator = view_iterator<EnVector::const_reverse_iterator>;
		
		
		/**
		 * @brief Returns an approximate size of the view; calculating actual
		 * size is more costly
		 * @return Underlying sparse_set size
		 */
		[[nodiscard]] size_type size_hint() const
		{
			if (!iterable)
				return 0;
			return iterable->size();
		}

		/**
		 * @brief Standard boiler plate for iterable objects
		 * @return Iterator to the beginning of range
		 */
		[[nodiscard]] iterator begin() const
		{
			return iterator{
				iterable->begin(),
				iterable->end(),
				iterable->begin(),
				exclude, include, pools };
		}

		/**
		 * @brief Standard boiler plate for iterable objects
		 * @return Iterator to the end of range
		 */
		[[nodiscard]] iterator end() const
		{
			return iterator{
				iterable->begin(),
				iterable->end(),
				iterable->end(),
				exclude, include, pools };
		}

		/**
		 * @brief Standard boiler plate for iterable objects
		 * @return Reverse iterator to the beginning of view
		 */
		[[nodiscard]] reverse_iterator rbegin() const
		{
			return reverse_iterator{
				iterable->rbegin(),
				iterable->rend(),
				iterable->rbegin(),
				exclude, include, pools };
		}

		/**
		 * @brief Standard boiler plate for iterable objects
		 * @return Reverse iterator to the end of view
		 */
		[[nodiscard]] reverse_iterator rend() const
		{
			return reverse_iterator{
				iterable->rbegin(),
				iterable->rend(),
				iterable->rend(),
				exclude, include, pools };
		}

		/**
		 * @brief Standard boiler plate for iterable objects
		 * @return Iterator to the found Entity; end() is returned if Entity
		 * cannot be found in the view
		 */
		[[nodiscard]] iterator find(Entity entity) const 
		{
			const auto it = iterator{ 
				iterable->begin(), 
				iterable->end(), 
				pools.at(pool_id).get()->find(entity), 
				exclude, include, pools };

			return (it != end() && *it == entity) ? it : end();
		}

		/**
		 * @brief Standard boiler plate for iterable objects
		 * @return Entity at start of range; INVALID if empty
		 */
		[[nodiscard]] Entity front() const 
		{
			const auto it = begin();
			return it != end() ? *it : INVALID;
		}

		/**
		 * @brief Standard boiler plate for iterable objects
		 * @return Entity at end of range; INVALID if empty
		 */
		[[nodiscard]] Entity back() const
		{
			const auto it = rbegin();
			return it != rend() ? *it : INVALID;
		}


		/**
		 * @brief Calls a callable object for each entity in view
		 * @tparam Func Callable object type
		 * @tparam func Callable object
		 */
		template<typename Func>
		[[nodiscard]] void each(Func func) const
		{
			for (auto i : *this)
				func(i);
		}

		/**
		 * @brief Checks if the component is registered in the ECS and thus
		 * in the view
		 * @tparam Component Valid component type
		 * @return True if component has been registered; False if not
		 */
		template<typename Component>
		[[nodiscard]] bool component_exist() const
		{
			return pools.count(typeid(Component).hash_code());
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
			// we can check if it exists in the pool but doesnt matter
			if (!component_exist<Component>())
				return nullptr;

			return get_sparse<Component>()->get(entity);
		}

		/**
		 * @brief Checks if the entity exists in the view
		 * @param entity An entity identifier, either valid or not
		 * @return True if entity exists in the view
		 */
		[[nodiscard]] bool contains(Entity entity)
		{
			for (auto i : exclude) if (!pools.count(i) || pools.at(i)->contains(entity)) return false;
			for (auto i : include) if (!pools.count(i) || !pools.at(i)->contains(entity)) return false;
			return true;
		}



	private:
		tvector<size_t> include;
		tvector<size_t> exclude;
		map_ref pools;

		// TEMP SOLUTION
		EnVector empty{};
		const EnVector* iterable = &empty;
		size_t pool_id = 0;
	};


}
