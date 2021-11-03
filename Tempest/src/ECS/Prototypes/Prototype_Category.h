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

			inline proto_category_container();
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
				prototype p(type_info, type_info);
				for (auto fn : create_fns) fn(p);
				return p;
			}
		};
	}

	DEFINE_PROTO_CATEGORIES
	{
		PROTO_CATEGORY(Door)
		{
			REQUIRE(Meta)
			REQUIRE(Transform)
			REQUIRE(Local)
			REQUIRE(Shape)
			REQUIRE(Model)
		});

		PROTO_CATEGORY(Wall)
		{
			REQUIRE(Meta)
			REQUIRE(Transform)
			REQUIRE(Local)
			REQUIRE(Shape)
			REQUIRE(Model)
		});

		PROTO_CATEGORY(Tile)
		{
			REQUIRE(Meta)
			REQUIRE(Transform)
			REQUIRE(Local)
			REQUIRE(Shape)
			REQUIRE(Model)
		});

		PROTO_CATEGORY(Decoration)
		{
			REQUIRE(Meta)
			REQUIRE(Transform)
			REQUIRE(Local)
			REQUIRE(Shape)
			REQUIRE(Model)
		});

		PROTO_CATEGORY(Obstacle)
		{
			REQUIRE(Meta)
			REQUIRE(Transform)
			REQUIRE(Local)
			REQUIRE(Shape)
			REQUIRE(Model)
		});
	}

	[[nodiscard]]
	inline static prototype create_new_prototype(const string& category)
	{
		if (!detail::proto_categories.categories.count(category))
			throw prototype_exception("Bad prototype category!");
		else
			return detail::proto_categories.categories.at(category).create();
	}

	[[nodiscard]]
	inline static auto get_prototype_categories()
	{
		return const_range(detail::proto_categories.categories);
	}

	[[nodiscard]]
	inline static bool verify(const string& category, const prototype& p)
	{
		if (!detail::proto_categories.categories.count(category))
			throw prototype_exception("Bad prototype category!");

		detail::proto_categories.categories.at(category).verify(p);
	}
	[[nodiscard]]
	inline static bool verify_strict(const string& category, const prototype& p)
	{
		if (!detail::proto_categories.categories.count(category))
			throw prototype_exception("Bad prototype category!");

		detail::proto_categories.categories.at(category).verify_strict(p);
	}
	
}