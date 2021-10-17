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
#include "coptional.h"

namespace Tempest
{
	class prefab
	{
		friend class prototype;

		string name;

		// only prototype can create a prefab
		prefab() = default;

	public:
		prefab(const prefab& rhs)
		{
			*this = rhs;
		}
		prefab& operator=(const prefab& rhs)
		{
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

	private:
		tmap<size_t, tuptr<c_base>> components;
	};
}