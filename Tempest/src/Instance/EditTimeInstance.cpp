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
		auto view = ecs.view<Components::Rigidbody, tc::Transform>(exclude_t<tc::Destroyed>());


		auto sl = ecs.view<Components::Statline>(exclude_t<tc::Destroyed>());
		if (sl.size_hint() == 0)
		{
			auto StatsLine = ecs.create();
			auto meta = ecs.emplace<tc::Meta>(StatsLine);
			meta->name = "StatsData";
			ecs.emplace<tc::Statline>(StatsLine);
		}
		

		/*for (auto id : view)
		{
			auto& rb = ecs.get<Components::Rigidbody>(id);
			auto& transform = ecs.get<Components::Transform>(id);
			rigidbody_config staticBody;
			staticBody.is_static = true;
			rb.internal_rb = po.create_actor(staticBody, rb.shape_data, transform.position, transform.rotation, id);
			po.AddActorToScene(rb.internal_rb.get());

		}*/
	}
	void EditTimeInstance::_update(float)
	{
		scene.get_map().update();

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
		}

		// destroy all deleted stuff in ecs
		auto view = ecs.view<Components::Destroyed>();
		tvector<Entity> entities{ view.begin(), view.end() };
		std::for_each(entities.begin(), entities.end(), std::bind(&ECS::destroy, &ecs, std::placeholders::_1));
	}
	void EditTimeInstance::_render()
	{

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
		//ecs.save(root);
		//graph_manager.save_all_to(root);
	}

	void EditTimeInstance::save_as(const tpath&)
	{
	}

	void EditTimeInstance::load_new_scene(const string& scene_name)
	{
		current_scene_name = scene_name;
	}

	void EditTimeInstance::load_new_conflict_resolution(const string& res_name)
	{
		current_res_name = res_name;
	}

	void EditTimeInstance::load_new_scene_by_path(const tpath& path)
	{
		// try to get the name in the path
		string scene_name;
		for (auto entry : fs::directory_iterator(path))
		{
			if (entry.path().extension() == ".json")
				scene_name = entry.path().string();
		}
		current_scene_name = scene_name;
		scene = Scene();
		scene.load(path);
	}

	void EditTimeInstance::load_new_conflict_resolution_by_path(const tpath& path)
	{
		// try to get the name in the path
		string res_name;
		for (auto entry : fs::directory_iterator(path))
		{
			if (entry.path().extension() == ".json")
				res_name = entry.path().string();
		}
		current_res_name = res_name;
		ecs.clear();
		ecs.load(path);
	}

	void EditTimeInstance::save_current_scene()
	{
		scene.save(root / "scenes" / current_scene_name);
	}

	void EditTimeInstance::save_current_conflict_resolution()
	{
		ecs.save(root / "conflict_resolutions" / current_res_name);
	}

	void EditTimeInstance::create_new_scene(const string& scene_name)
	{

	}

	void EditTimeInstance::create_new_conflict_resolution(int i, const string& res_name)
	{
		auto path = root / "conflict_resolutions" / std::to_string(i);
		if (!fs::exists(path))
			fs::create_directory(path);

		current_res_name = res_name;
		ecs.clear();
	}

	tvector<tpair<int, tpath>> EditTimeInstance::get_scene_paths()
	{
		tvector<tpair<int, tpath>> paths;
		
		int i = 1;
		for (auto entry : fs::directory_iterator(root / "scenes"))
		{
			// each directory is a scene
			paths.push_back(std::make_pair(i++, entry.path()));
		}

		return paths;
	}

	tvector<tpair<bool, tpath>> EditTimeInstance::get_conflict_resolution_paths()
	{
		tvector<tpair<bool, tpath>> paths(3);

		for (auto entry : fs::directory_iterator(root / "conflict_resolutions"))
		{
			// each directory is a conflict res
			// it should be from 1 to 3
			if (fs::is_directory(entry.path()))
			{
				string a = entry.path().filename().string();
				if(a == "0" || a == "1" || a == "2")
					paths[std::atoi(a.c_str())] = std::make_pair(true, entry.path());
			}
		}

		return paths;
	}

}