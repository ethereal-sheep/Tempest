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

namespace Tempest
{
	struct c_base
	{
		c_base() = delete;
		c_base(const string& type) : type_info{ type } {}
		virtual ~c_base() = default;

		virtual Writer& serialize(Writer& writer) const = 0;
		//virtual Reader& deserialize(Reader& reader) const = 0;

		virtual bool is_parent() const = 0;
		virtual bool is_child() const = 0;
		virtual bool is_overriding() const = 0;
		virtual bool is_inheriting() const = 0;
		virtual bool has_value() const = 0;
		virtual operator bool() const = 0;
		virtual void inherit(bool) = 0;
		virtual void toggle() = 0;
		virtual tuptr<c_base> instance() const = 0;
		virtual tuptr<c_base> clone() const = 0;

		string type_info;
	};


	template<typename Component>
	class coptional : public c_base
	{
		std::weak_ptr<Component> parent;
		std::shared_ptr<Component> mine;
		/*
			own => inherit

			states
			holds a Component
			holds a weak_reference to a Component
			
		*/
		friend class coptional;

		coptional(std::weak_ptr<Component> ptr) :
			c_base(Component::get_type()),
			parent{ ptr },
			mine{ nullptr }
		{
			// create a child component
		}

	public:
		coptional() :
			c_base(Component::get_type()),
			mine{ std::make_shared<Component>() }
		{
			// create a standalone component
		}
		
		coptional(const coptional& rhs) : 
			c_base(Component::get_type()),
			parent(rhs.parent)
		{
			mine = rhs.mine ? std::make_shared<Component>(*rhs.mine) : nullptr;
		}

		coptional& operator=(const coptional& rhs)
		{
			parent = rhs.parent;
			mine = rhs.mine ? std::make_shared<Component>(*rhs) : nullptr;
		}

		coptional(coptional&& rhs) noexcept = default;
		coptional& operator=(coptional&&) = default;

		Writer& serialize(Writer& writer) const override
		{
			// assume its not bad
			Component c = get();
			writer.Member("Component", c);
			return writer;
		}


		bool is_parent() const override
		{
			return (bool)!parent.lock();
		}

		bool is_child() const override
		{
			return (bool)parent.lock();
		}

		bool is_overriding() const override
		{
			if (mine)
				return true;
			return false;
		}

		bool is_inheriting() const override
		{
			return !is_overriding();
		}

		void inherit(bool in) override
		{
			bool o = is_overriding();
			if (in && o)
				mine.reset();
			else if (!in && !o)
			{
				if(auto p = parent.lock())
					mine = std::make_shared<Component>(*p);
			}
		}

		bool has_value() const override
		{
			return !parent.lock() && !mine;
		}

		operator bool() const override
		{
			return has_value();
		}

		tuptr<c_base> instance() const override
		{
			return make_uptr<coptional<Component>>(coptional<Component>(mine));
		}

		tuptr<c_base> clone() const override
		{
			return make_uptr<coptional<Component>>(*this);
		}

		Component& operator*()
		{
			return get();
		}

		Component* operator->()
		{
			return get_if();
		}

		Component& value()
		{
			return get();
		}

		Component& get()
		{
			if (mine)
				return *mine;
			return *parent.lock();
		}

		const Component& get() const
		{
			if (mine)
				return *mine;
			return *parent.lock();
		}

		const Component* get_if() const
		{
			if (mine)
				return mine.get();
			else if (parent.lock())
				return parent.lock().get();

			return nullptr;
		}

		Component* get_if()
		{
			if (mine)
				return mine.get();
			else if (parent.lock())
				return parent.lock().get();

			return nullptr;
		}

		Component& force()
		{
			// if inheriting or bad, force an override or clean
			if (is_inheriting() || !has_value())
				mine = std::make_shared<Component>();

			return get();
		}

		void toggle()
		{
			// case 1: highest level component, cannot inherit so we cannot remove mine 
			// case 2: child level component and overriding, remove mine
			// case 3: child level component and inheriting, copy from parent

			auto p = parent.lock();

			if (!p) return;									// case 1
			else if (mine) mine.reset();					// case 2
			else mine = std::make_shared<Component>(*p);	// case 3
		}
		
		
	};
}