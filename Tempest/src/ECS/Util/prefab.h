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

#include "../../Memory.h"
#include "../Entity.h"
#include "ECS/Components/Components.h"
#include "Util.h"
#include "coptional.h"

namespace Tempest
{
	class prefab
	{
		friend class prototype;
		friend class ECS;
		friend class Scene;

		string cat;
		string proto;
		tc::Transform transform;

		// only prototype and scene can create a prefab
		prefab() = default;

	public:
		static const bool is_entity_keyed = false;

		prefab(const prefab& rhs)
		{
			*this = rhs;
		}
		prefab& operator=(const prefab& rhs)
		{
			cat = rhs.cat;
			proto = rhs.proto;
			transform = rhs.transform;

			components.clear();
			for (auto& [hash, ptr] : rhs.components)
			{
				components[hash] = ptr->clone();
			}
			return *this;
		}
		prefab(prefab&&) noexcept = default;
		prefab& operator=(prefab&&) = default;

		template<typename Component>
		bool has() const
		{
			return components.count(t_hash<Component>());
		}

		template<typename Component>
		const Component& get() const
		{
			return static_cast<coptional<Component>*>(
				components.at(t_hash<Component>()).get())->get();
		}

		template<typename Component>
		const Component* get_if() const
		{
			if (!has<Component>())
				return nullptr;

			return &get<Component>();
		}

		template<typename Component>
		bool is_overriding() const
		{
			if (!has<Component>())
				return false;

			return components.at(t_hash<Component>())->is_overriding();
		}

		template<typename Component>
		bool is_inheriting() const
		{
			if (!has<Component>())
				return false;

			return components.at(t_hash<Component>())->is_inheriting();
		}

		template<typename Component>
		bool has_value() const
		{
			if (!has<Component>())
				return false;

			return components.at(t_hash<Component>())->has_value();
		}

		template<typename Component>
		void toggle()
		{
			if (!has<Component>()) return;

			components.at(t_hash<Component>())->toggle();
		}

		template<typename Component>
		void inherit(bool in)
		{
			if (!has<Component>()) return;

			return components.at(t_hash<Component>())->inherit(in);
		}

		template<typename Component>
		Component& force()
		{
			// case 1: prefab has component
			// case 1.1: copt is inheriting -> override
			// case 1.2: copt is overriding -> do nothing
			// case 1.3: copt is bad        -> make it okay (should not happen)
			// case 3: prefab has no component

			return static_cast<coptional<Component>*>(
				components.at(t_hash<Component>()).get())->force();
		}

		template<typename Component>
		Component* force_if()
		{
			if (!has<Component>())
				return nullptr;

			return &force<Component>();
		}

		template<typename Component>
		Component* try_emplace()
		{
			if (!has<Component>())
				return nullptr;

			force<Component>();

			return static_cast<coptional<Component>*>(
				components.at(t_hash<Component>()).get())->get_if();
		}

		

		void serialize(Writer& writer)
		{
			writer.StartArray("Components");
			for (auto& [hash, ptr] : components)
			{
				// serialize only if overriding
				if (ptr->is_overriding())
				{
					writer.StartObject();
					writer.StartMeta();
					writer.Member("Type", ptr->type_info);
					writer.EndMeta();
					ptr->serialize(writer);

					writer.EndObject();
				}
			}

			writer.EndArray();
		}

		void deserialize(Reader& reader)
		{
			size_t size;
			reader.StartArray("Components", &size);
			for (size_t i = 0; i < size; ++i)
			{
				reader.StartObject();

				string typeinfo;
				reader.StartMeta();
				reader.Member("Type", typeinfo);
				reader.EndMeta();

				auto type = magic_enum::enum_cast<ComponentType>(typeinfo);
				if (!type.has_value())
				{
					LOG_WARN("Component of type " + typeinfo + " cannot be found!");
					reader.EndObject();
					continue;
				}

				deserialize_helper(reader, *this, *type);

				reader.EndObject();
			}
			reader.EndArray();
		}
		
		const string& get_category() const
		{
			return cat;
		}

		const string& get_prototype() const
		{
			return proto;
		}

	private:
		tmap<size_t, tuptr<c_base>> components;
	};
}