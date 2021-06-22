#pragma once
#include "InstanceConfig.h"
#include "Triggers/Triggers.h"
#include "imgui/imgui.h"
#include "Events/EventManager.h"

namespace Tempest
{

	class InstanceManager
	{
	public:
		void update(float dt)
		{
			switch (current_state)
			{
			case Tempest::InstanceState::LOAD:
				// we try to load a new instance
				try
				{
					auto new_instance = create_new_instance();

					// if there is a running instance
					if (instance)
					{
						// call exit on current instance
						instance->OnExit();
						// reset instance
						instance.reset(nullptr);
						// reset event manager
						Service<EventManager>::Get().clear();
					}

					// assign new pointer to instance
					instance = std::move(new_instance);

					register_instance_windows();

					// call init on new instance
					instance->OnInit();
					current_state = InstanceState::RUN;

					// register the one event that instance manager needs
					Service<EventManager>::Get().register_listener<LoadNewInstance>(
						&InstanceManager::load_new_instance, this
					);
				}
				catch (const std::exception& a)
				{
					// if we fail, current instance is maintained
					// but we display an error
					// dispatch some event
					current_state = InstanceState::RUN;
					Service<EventManager>::Get().instant_dispatch<ErrorTrigger>(a.what());
				}
				[[fallthrough]];

			case Tempest::InstanceState::RUN:
				if(instance)
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
				// for debugging only (remove on release)
				if (demo_visible)
					ImGui::ShowDemoWindow();


				instance->OnRender();
			}
			else
			{
				// some other shit for empty
				// we can do some ui do load the instance
			}
		}
		// global menu bar
		void menubar()
		{
			if (instance)
			{
				if (ImGui::BeginMainMenuBar())
				{
					if (ImGui::BeginMenu("Windows"))
					{
						for (auto& window : instance->window_manager.get_windows())
						{
							ImGui::MenuItem(window->window_name(), nullptr, &window->visible);
						}
						ImGui::EndMenu();
					}
					// for debugging only (remove on release)
					if (ImGui::BeginMenu("Demo"))
					{
						ImGui::MenuItem("ImGui Demo", nullptr, &demo_visible);
						ImGui::EndMenu();
					}
				}
				ImGui::EndMainMenuBar();
			}
		}


		void load_new_instance(const Event& e)
		{
			auto& a = event_cast<LoadNewInstance>(e);
			current_state = InstanceState::LOAD;
			next_config = a.config;
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
				return make_uptr<RuntimeInstance>(next_config.project_path, next_config.memory_strategy);
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
			default:
				register_nulltime_windows();
				break;
			}
		}

		void register_nulltime_windows();
		void register_edittime_windows();
		void register_runtime_windows();

		InstanceState current_state = InstanceState::LOAD;
		InstanceConfig next_config = {};

		tuptr<Instance> instance = nullptr;

		// for debugging only (remove on release)
		bool demo_visible = false;
	};
}