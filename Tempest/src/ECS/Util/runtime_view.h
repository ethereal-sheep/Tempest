#pragma once

#include "..\..\Core.h"
#include "sparse_set.h"

namespace Tempest
{
	class runtime_view
	{
		using vmap_ref = const tmap<size_t, tuptr<sparse_set>>&;

	public:
		runtime_view(tvector<size_t> include_l, tvector<size_t> exclude_l, const tmap<size_t, tuptr<sparse_set>>& pools_l) 
			:include{ std::move(include_l) }, exclude{ std::move(exclude_l) }, pools{ pools_l } 
		{
			// preprocessing
			auto lambda = [&](auto i) {
				return !pools.count(i);
			};

			include.erase(std::remove_if(include.begin(), include.end(), lambda), include.end());
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


		template <typename It>
		class view_iterator final
		{
			friend class runtime_view;


			bool valid() const {
				const auto entity = *it;

				for (auto i : exclude) if (pools.at(i)->contains(entity)) return false;
				for (auto i : include) if (!pools.at(i)->contains(entity)) return false;

				return true;
			}

			view_iterator(It from, It to, It curr,
				const tvector<size_t>& exclude_l,
				const tvector<size_t>& include_l,
				vmap_ref pools_l) :
				first{ from },
				last{ to },
				it{ curr },
				exclude{ exclude_l },
				include{ include_l },
				pools{ pools_l }
			{
				while (it != last && !valid()) {
					++(*this);
				}
			}

		public:


			using difference_type = size_t;
			using value_type = Entity;
			using pointer = const value_type*;
			using reference = const value_type&;
			using iterator_category = std::bidirectional_iterator_tag;

			view_iterator() = delete;

			view_iterator& operator++() 
			{
				while (++it != last && !valid());
				return *this;
			}

			view_iterator operator++(int) 
			{
				view_iterator orig = *this;
				return ++(*this), orig;
			}

			view_iterator& operator--() 
			{
				while (--it != first && !valid());
				return *this;
			}

			view_iterator operator--(int) 
			{
				view_iterator orig = *this;
				return operator--(), orig;
			}

			[[nodiscard]] bool operator==(const view_iterator& rhs) const 
			{
				return rhs.it == it;
			}

			[[nodiscard]] bool operator!=(const view_iterator& rhs) const
			{
				return !(*this == rhs);
			}

			[[nodiscard]] pointer operator->() const 
			{
				return &*it;
			}

			[[nodiscard]] reference operator*() const 
			{
				return *operator->();
			}

		private:
			It first;
			It last;
			It it;
			const tvector<size_t>& exclude;
			const tvector<size_t>& include;
			vmap_ref pools;
		};

		using value_type = Entity;
		using size_type = std::size_t;
		using iterator = view_iterator<EnVector::const_iterator>;
		using reverse_iterator = view_iterator<EnVector::const_reverse_iterator>;
		
		
		[[nodiscard]] size_type size_hint() const
		{
			if (!iterable)
				return 0;
			return iterable->size();
		}

		[[nodiscard]] iterator begin() const
		{
			return iterator{
				iterable->begin(),
				iterable->end(),
				iterable->begin(),
				exclude, include, pools };
		}

		[[nodiscard]] iterator end() const
		{
			return iterator{
				iterable->begin(),
				iterable->end(),
				iterable->end(),
				exclude, include, pools };
		}
		[[nodiscard]] reverse_iterator rbegin() const
		{
			return reverse_iterator{
				iterable->rbegin(),
				iterable->rend(),
				iterable->rbegin(),
				exclude, include, pools };
		}

		[[nodiscard]] reverse_iterator rend() const
		{
			return reverse_iterator{
				iterable->rbegin(),
				iterable->rend(),
				iterable->rend(),
				exclude, include, pools };
		}

		[[nodiscard]] Entity front() const 
		{
			const auto it = begin();
			return it != end() ? *it : INVALID;
		}

		[[nodiscard]] Entity back() const
		{
			const auto it = rbegin();
			return it != rend() ? *it : INVALID;
		}

		[[nodiscard]] iterator find(Entity entity) const 
		{
			const auto it = iterator{ 
				iterable->begin(), 
				iterable->end(), 
				pools.at(pool_id).get()->find(entity), 
				exclude, include, pools };

			return (it != end() && *it == entity) ? it : end();
		}

		template<typename Func>
		[[nodiscard]] void each(Func func) const
		{
			for (auto i : *this)
				func(i);
		}

	private:
		tvector<size_t> include;
		tvector<size_t> exclude;
		vmap_ref pools;

		// TEMP SOLUTION
		EnVector empty{};
		const EnVector* iterable = &empty;
		size_t pool_id = 0;
	};


}
