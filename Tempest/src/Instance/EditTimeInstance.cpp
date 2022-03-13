/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "EditTimeInstance.h"
#include "Graphics/Basics/RenderSystem.h"
#include "ECS/Components/Components.h"
#include <Physics/Util/px_math.h>

namespace Tempest
{
	void EditTimeInstance::_init()
	{
	}
	void EditTimeInstance::_update(float)
	{
		auto sl = ecs.view<Components::Statline>(exclude_t<tc::Destroyed>());
		if (sl.size_hint() == 0)
		{
			auto StatsLine = ecs.create();
			auto meta = ecs.emplace<tc::Meta>(StatsLine);
			meta->name = "StatsData";
			ecs.emplace<tc::Statline>(StatsLine);
		}
		
		/*else
		{
			for (auto id : sl)
			{
				auto& StatsLine = ecs.get<Components::Statline>(id);
				if (StatsLine.size() < tc::STAT_TOTAL)
				{
					for (auto i = StatsLine.size(); i < tc::STAT_TOTAL; i++)
					{
						string str = "Stats" + std::to_string(i);
						LOG("FK");
					}
				
				}
			}
			
		}

		for (auto id : view)
		{
			auto& rb = ecs.get<Components::Rigidbody>(id);
			auto& transform = ecs.get<Components::Transform>(id);
			rigidbody_config staticBody;
			staticBody.is_static = true;
			rb.internal_rb = po.create_actor(staticBody, rb.shape_data, transform.position, transform.rotation, id);
			po.AddActorToScene(rb.internal_rb.get());
		}*/

		scene.get_map().update();


		// destroy all deleted stuff in ecs
		auto view = ecs.view<Components::Destroyed>();
		tvector<Entity> entities{ view.begin(), view.end() };
		std::for_each(entities.begin(), entities.end(), std::bind(&ECS::destroy, &ecs, std::placeholders::_1));
	}

	void EditTimeInstance::_render()
	{
		for (auto& [id, pf] : scene.get_map())
		{

			if (auto model = pf.get_if<tc::Model>())
			{
				auto transform = pf.get_if<tc::Transform>();
				auto local = pf.get_if<tc::Local>();

				auto test = glm::translate(transform->position)
					* glm::mat4(transform->rotation)
					* glm::translate(local->local_position)
					* glm::mat4(local->local_rotation)
					* glm::scale(local->local_scale)
					* glm::scale(transform->scale);

				std::filesystem::path p{ model->path };
				if (strcmp(p.extension().string().c_str(), ".a"))
				{
					p.replace_extension(".a");
				}
				Service<RenderSystem>::Get().SubmitModel(p.string(), test);
			}

			if (auto door = pf.get_if<tc::Door>())
			{

				auto transform = pf.get_if<tc::Transform>();

				const tc::Local* local = &door->frame_local;

				auto test = glm::translate(transform->position)
					* glm::mat4(transform->rotation)
					* glm::translate(local->local_position)
					* glm::mat4(local->local_rotation)
					* glm::scale(local->local_scale)
					* glm::scale(transform->scale);


				std::filesystem::path p{ door->frame.path };
				if (strcmp(p.extension().string().c_str(), ".a"))
				{
					p.replace_extension(".a");
				}
				Service<RenderSystem>::Get().SubmitModel(p.string(), test);
			}
		}
	}
	void EditTimeInstance::_exit()
	{
		/*auto view = ecs.view<Components::Rigidbody, tc::Transform>();
		for (auto id : view)
		{
			auto& rb = ecs.get<Components::Rigidbody>(id);
			po.RemoveActorFromScene(rb.internal_rb.get());
			rb.internal_rb = nullptr;

		}*/
	}

	void EditTimeInstance::save()
	{
		//scene.save(root);
		save_current_scene();
		//ecs.save(root);
		save_current_conflict_resolution();
		//graph_manager.save_all_to(root);
	}

	void EditTimeInstance::save_as(const tpath&)
	{
	}

	

	void EditTimeInstance::save_current_scene()
	{
		if (current_scene_name != "")
		{
			scene.save(root / "scenes" / current_scene_name);
			LOG_INFO("Saved {0}", current_scene_name);
		}
	}

	void EditTimeInstance::save_current_conflict_resolution()
	{
		if (current_res_name != "" && current_res_index != 0)
		{
			ecs.save(root / "conflict_resolutions" / std::to_string(current_res_index), current_res_name);
			LOG_INFO("Saved {0}:{1}", current_res_name, current_res_index);
		}
	}

	bool EditTimeInstance::delete_scene(const string& scene_name)
	{
		// assume not loaded
		auto path = root / "scenes" / scene_name;
		if (fs::exists(path)) {
			fs::remove_all(path);
			return true;
		}
		return false;
	}

	bool EditTimeInstance::delete_conflict_resolution(int i)
	{
		// assume not loaded
		auto path = root / "conflict_resolutions" / std::to_string(i);
		if (fs::exists(path)) {
			fs::remove_all(path);
			return true;
		}
		return false;
	}

	string EditTimeInstance::create_new_scene(const string& scene_name)
	{
		if (scene_name == "")
		{
			LOG_INFO("Failed to create scene; Scene name is empty!");
			return "";
		}

		auto path = root / "scenes";
		if (!fs::exists(path))
			fs::create_directories(path);
		tvector<string> names;
		for (auto entry : fs::directory_iterator(path))
		{
			if (fs::is_directory(entry.path()))
			{
				names.push_back(entry.path().stem().string());
			}
		}

		auto new_name = algo::get_next_name(scene_name, names.begin(), names.end(), [](const string& a, const string& new_name) { return a == new_name; });
		path /= new_name;
		if (!fs::exists(path))
			fs::create_directories(path);

		Scene().save(path);
		LOG_INFO("Created {0}", scene_name);

		return new_name;
	}

	void EditTimeInstance::create_new_conflict_resolution(int i, const string& res_name)
	{
		if (res_name == "")
		{
			LOG_INFO("Failed to create resolution; Resolution name is empty!");
			return;
		}
		if (i <= 0 || i > 3)
		{
			LOG_INFO("Failed to create resolution; Slot out of range!");
			return;
		}

		auto path = root / "conflict_resolutions" / std::to_string(i) / res_name;
		if (!fs::exists(path))
			fs::create_directories(path);

		LOG_INFO("Created {0}:{1}", res_name, i);
	}

	

}