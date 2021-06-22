#pragma once
#include "Instance/RuntimeInstance.h"
#include "Instance/EditTimeInstance.h"

#include "imgui/imgui.h"

namespace Tempest
{
	enum struct InstanceType
	{
		EDIT_TIME,
		RUN_TIME,
		NULL_TIME,
	};

	enum struct InstanceState
	{
		LOAD,
		RUN,
		UNLOAD,
		STOPPED,
	};

	struct InstanceConfig
	{
		InstanceConfig(const tpath& _path, MemoryStrategy strategy = MemoryStrategy{}, InstanceType type = InstanceType::EDIT_TIME)
			: project_path(_path), memory_strategy(strategy), instance_type(type) {}

		tpath project_path;
		MemoryStrategy memory_strategy;
		InstanceType instance_type;
	};

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
					}

					// assign new pointer to instance
					instance = std::move(new_instance);

					register_instance_windows();

					// call init on new instance
					instance->OnInit();
					current_state = InstanceState::RUN;
				}
				catch (const std::exception&)
				{
					// if we fail, current instance is maintained
					// but we display an error
					// dispatch some event
				}
				catch (...) // we can catch never save error if we want also
				{

				}
				[[fallthrough]];

			case Tempest::InstanceState::RUN:
				if(instance)
					instance->OnUpdate(dt);
				// if no instance, just don't update
				break;
			case Tempest::InstanceState::UNLOAD:
				// if there is a running instance
				if (instance)
				{
					// call exit on current instance
					instance->OnExit();
					// reset instance
					instance.reset(nullptr);
				}
				current_state = InstanceState::STOPPED;

				break;
			case Tempest::InstanceState::STOPPED: [[fallthrough]];
			default:
				break;
			}
		}
		void render()
		{
			if (instance)
			{
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
					if (ImGui::BeginMenu("Demo"))
					{
						ImGui::MenuItem("ImGui Demo", nullptr, &demo_visible);
						ImGui::EndMenu();
					}
				}
				ImGui::EndMainMenuBar();
			}
		}


		void load_new_instance([[maybe_unused]] InstanceConfig config)
		{
			current_state = InstanceState::LOAD;
			next_config = config;
		}
		void stop_current_instance()
		{
			current_state = InstanceState::UNLOAD;
		}

	private:
		tuptr<Instance> create_new_instance()
		{
			switch (next_config.instance_type)
			{
			case Tempest::InstanceType::EDIT_TIME:
				return make_uptr<EditTimeInstance>(next_config.project_path, next_config.memory_strategy);
				break;
			case Tempest::InstanceType::RUN_TIME:
				return make_uptr<RuntimeInstance>(next_config.project_path, next_config.memory_strategy);
				break;
			default:
				return make_uptr<EditTimeInstance>(next_config.project_path, next_config.memory_strategy);
				break;
			}
		}
		void register_instance_windows()
		{
			switch (next_config.instance_type)
			{
			case Tempest::InstanceType::EDIT_TIME:
				register_edittime_windows();
				break;
			case Tempest::InstanceType::RUN_TIME:
				register_runtime_windows();
				break;
			default:
				register_edittime_windows();
				break;
			}
		}
		void register_edittime_windows();
		void register_runtime_windows();

		InstanceState current_state = InstanceState::STOPPED;
		InstanceConfig next_config = InstanceConfig("");

		tuptr<Instance> instance = nullptr;

		bool demo_visible = false;
	};
}