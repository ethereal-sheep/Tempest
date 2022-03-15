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
#include "InstanceConfig.h"
#include "Triggers/Triggers.h"
#include "Extern/imgui/imgui.h"
#include "Extern/imgui/implot.h"
#include "Events/EventManager.h"

namespace Tempest
{

	class InstanceManager
	{
	public:
		~InstanceManager()
		{
			if (instance)
			{
				instance->OnExit();
				// reset instance
				instance.reset(nullptr);
			}
		}

		void exit()
		{
			if (instance)
			{
				instance->OnExit();
				// reset instance
				instance.reset(nullptr);
			}
		}

		void update(float dt)
		{
			switch (current_state)
			{
			case Tempest::InstanceState::LOAD:
			{
				// we try to load a new instance
				// if there is a running instance
				if (instance)
				{
					// call exit on current instance
					instance->OnExit();
					// reset instance
					instance.reset(nullptr);
				}
				// reset event manager
				Service<EventManager>::Get().clear();

				// register the one event that instance manager needs
				Service<EventManager>::Get().register_listener<LoadNewInstance>(
					&InstanceManager::load_new_instance, this
				);
				// guess what there is two now
				Service<EventManager>::Get().register_listener<LoadPrevInstance>(
					&InstanceManager::load_prev_instance, this
					);

				try
				{
					// assign new pointer to instance
					instance = create_new_instance();
					instance->tutorial_enable = next_config.enable_tutorial;
				}
				catch (const std::exception& a)
				{
					// if we fail, current instance is maintained
					// but we display an error
					// dispatch some event
					next_config = {};
					instance = create_new_instance();
					Service<EventManager>::Get().instant_dispatch<ErrorTrigger>(a.what());
				}
				catch (...) // runtime_create_exception
				{
					// reload edit

				}

				register_instance_windows();

				// call init on new instance
				instance->OnInit();
				current_state = InstanceState::RUN;

			}
			[[fallthrough]];

			case Tempest::InstanceState::RUN:
				if (instance)
					instance->OnUpdate(dt);
				// if no instance, just don't update
				break;
			default:
				break;
		}
	}
	void render()
	{
		if (instance)
		{
			instance->OnRender();
		}
	}


	void load_new_instance(const Event& e)
	{
		auto& a = event_cast<LoadNewInstance>(e);
		current_state = InstanceState::LOAD;
		next_config = a.config;
	}

	void load_prev_instance(const Event&)
	{
		//auto& a = event_cast<LoadPrevInstance>(e);
		// check if current has prev can load or not


		current_state = InstanceState::LOAD;
		//next_config = a.config;
	}

private:
	tuptr<Instance> create_new_instance()
	{
		switch (next_config.instance_type)
		{
		case Tempest::InstanceType::NULL_TIME:
			return make_uptr<NullTimeInstance>(next_config.memory_strategy);
			break;
		case Tempest::InstanceType::EDIT_TIME:
			return make_uptr<EditTimeInstance>(next_config.project_path, next_config.memory_strategy);
			break;
		case Tempest::InstanceType::RUN_TIME:
		{
			// we try to load instance with map, con res, and seq
			auto ptr = make_uptr<RuntimeInstance>(next_config.project_path, next_config.memory_strategy);

			if (!ptr->load_new_conflict_resolution_by_int(next_config.conflict_resolution))
				throw std::exception("Bad Conflict Resolution!");
			if (!ptr->load_new_scene_by_name(next_config.map_name))
				throw std::exception("Bad Map Name!");
			if (next_config.sequences.empty())
				throw std::exception("Bad Sequences!");
			ptr->sequences = next_config.sequences;

			return ptr;
			break;
		}
		case Tempest::InstanceType::PHYSICS_TIME:
			return make_uptr<PhysicsInstance>(next_config.project_path, next_config.memory_strategy);
			break;
		default:
			return make_uptr<NullTimeInstance>(next_config.memory_strategy);
			break;
		}
	}
	void register_instance_windows()
	{
		switch (next_config.instance_type)
		{
		case Tempest::InstanceType::NULL_TIME:
			register_nulltime_windows();
			break;
		case Tempest::InstanceType::EDIT_TIME:
			register_edittime_windows();
			break;
		case Tempest::InstanceType::RUN_TIME:
			register_runtime_windows();
			break;
		case Tempest::InstanceType::PHYSICS_TIME:
			register_physicstime_windows();
			break;
		default:
			register_nulltime_windows();
			break;
		}
	}

	void register_nulltime_windows();
	void register_edittime_windows();
	void register_runtime_windows();
	void register_physicstime_windows();

	InstanceState current_state = InstanceState::LOAD;
	InstanceConfig next_config = {};

	tuptr<Instance> instance = nullptr;

};
}