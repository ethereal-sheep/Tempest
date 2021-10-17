#pragma once
#include "../Util/prototype.h"

#define DEFINE_PROTO_CATEGORIES detail::proto_category_container::proto_category_container()
#define END }

#define PROTO_CATEGORY(category_name) \
detail::proto_category category_name(#category_name, categories, []( \
	tvector<std::function<bool(const prototype&)>>& verify_fns, \
	tvector<std::function<void(prototype&)>>& create_fns) \

#define PROTO_CATEGORY_END );			\

#define REQUIRE(component_name) \
	verify_fns.emplace_back([](const prototype& p)->bool { \
		return p.has<tc::component_name>(); \
		}); \
	create_fns.emplace_back([](prototype& p) { \
		p.emplace<tc::component_name>(); \
		}); \



namespace Tempest
{
	namespace detail
	{
		struct proto_category;
		struct proto_category_container
		{
			tmap<string, proto_category> categories;

			proto_category_container();
		};

		static proto_category_container proto_categories;


		struct proto_category
		{
			string type_info;

			tvector<std::function<bool(const prototype&)>> verify_fns;
			tvector<std::function<void(prototype&)>> create_fns;

			proto_category(
				string info, 
				tmap<string, proto_category>& categories,
				std::function<
				void(
					tvector<std::function<bool(const prototype&)>>& , 
					tvector<std::function<void(prototype&)>>&)> fn) : type_info(info)
			{
				fn(verify_fns, create_fns);
				LOG_ASSERT(!categories.count(info));
				categories.emplace(std::make_pair(info, *this));
			}
			
			proto_category(const proto_category& ) = default;
			proto_category& operator=(const proto_category& ) = default;
			proto_category(proto_category&&) noexcept = default;
			proto_category& operator=(proto_category&&) = default;

			size_t size() const
			{
				return verify_fns.size();
			}

			bool verify(const prototype& p) const
			{
				for (auto fn : verify_fns) if (!fn(p)) return false;
				return true;
			}

			bool verify_strict(const prototype& p) const
			{
				return p.size() == size() && verify(p);
			}

			prototype create() const
			{
				prototype p;
				for (auto fn : create_fns) fn(p);
				return p;
			}
		};
	}

	DEFINE_PROTO_CATEGORIES
	{
		PROTO_CATEGORY(Wall)
		{
			REQUIRE(Meta)
			REQUIRE(Transform)
		});
	}

	[[no_discard]]
	inline static prototype create_new_prototype(const string& category)
	{
		if (!detail::proto_categories.categories.count(category))
			throw prototype_exception("Bad prototype category!");
		else
			return detail::proto_categories.categories.at(category).create();
	}

	[[no_discard]]
	inline static auto get_prototype_categories()
	{
		return const_range(detail::proto_categories.categories);
	}

	[[no_discard]]
	inline static bool verify(const string& category, const prototype& p)
	{
		if (!detail::proto_categories.categories.count(category))
			throw prototype_exception("Bad prototype category!");

		detail::proto_categories.categories.at(category).verify(p);
	}
	[[no_discard]]
	inline static bool verify_strict(const string& category, const prototype& p)
	{
		if (!detail::proto_categories.categories.count(category))
			throw prototype_exception("Bad prototype category!");

		detail::proto_categories.categories.at(category).verify_strict(p);
	}
	
}