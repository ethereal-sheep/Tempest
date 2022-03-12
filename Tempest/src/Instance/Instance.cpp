/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "Instance.h"
#include "Graphics/Basics/RenderSystem.h"

#include "Particles/Particles_2D/ParticleSystem_2D.h"

namespace Tempest
{
	void Instance::internal_init()
	{
		window_manager.init(*this);

		// clean up destroyed
		auto view = ecs.view<Components::Destroyed>();
		tvector<Entity> entities{ view.begin(), view.end() };
		std::for_each(entities.begin(), entities.end(), std::bind(&ECS::destroy, &ecs, std::placeholders::_1));
	}
	void Instance::internal_update(float dt)
	{
		auto view = ecs.view<tc::Door>(exclude_t<tc::Destroyed>());
		for (auto id : view)
		{
			auto& door = ecs.get<tc::Door>(id);
			door.update(dt);
		}
		
		ParticleSystem_2D::GetInstance().Update();
	}
	void Instance::internal_render()
	{
		window_manager.show(*this);


		// move this to instance call when test finish
		auto view = ecs.view<tc::Mesh>(exclude_t<tc::Destroyed>());
		for (auto id : view)
		{
			auto& mesh = ecs.get<tc::Mesh>(id);
			auto& transform = ecs.get<tc::Transform>(id);
			Service<RenderSystem>::Get().Submit(mesh.code, transform);
		}

		// without local
		auto view2 = ecs.view<tc::Model, tc::Transform>(exclude_t<tc::Destroyed, tc::Local>());
		for (auto id : view2)
		{
			auto& model = ecs.get<tc::Model>(id);
			auto& transform = ecs.get<tc::Transform>(id);
			Service<RenderSystem>::Get().SubmitModel(model.path.c_str(), transform);
		}

		// with local
		auto view3 = ecs.view<tc::Model, tc::Local, tc::Transform>(exclude_t<tc::Destroyed, tc::Door>());
		for (auto id : view3)
		{
			auto model = ecs.get_if<tc::Model>(id);
			auto transform = ecs.get_if<tc::Transform>(id);
			auto local = ecs.get_if<tc::Local>(id);

			auto test = glm::translate(transform->position)
				* glm::mat4(transform->rotation)
				* glm::translate(local->local_position)
				* glm::mat4(local->local_rotation)
				* glm::scale(local->local_scale)
				* glm::scale(transform->scale);

			auto character = ecs.get_if<tc::Character>(id);
			if (character != nullptr)
			{
				Service<RenderSystem>::Get().SubmitModel(model->path, test, character->color);
			}

			Service<RenderSystem>::Get().SubmitModel(model->path, test);

			//Service<RenderSystem>::Get().SubmitModel(model.path.c_str(), transform);
		}

		// with local & door
		auto view4 = ecs.view<tc::Model, tc::Local, tc::Door, tc::Transform>(exclude_t<tc::Destroyed>());
		for (auto id : view4)
		{
			auto door = ecs.get_if<tc::Door>(id);
			auto transform = ecs.get_if<tc::Transform>(id);

			{
				auto model = ecs.get_if<tc::Model>(id);
				auto local = ecs.get_if<tc::Local>(id);
				auto door_t = door->get_current_local();
				auto door_tf = &door_t;

				auto test = glm::translate(transform->position)
					* glm::mat4(transform->rotation)
					* glm::translate(door_tf->local_position)
					* glm::mat4(door_tf->local_rotation)
					* glm::translate(local->local_position)
					* glm::mat4(local->local_rotation)
					* glm::scale(local->local_scale)
					* glm::scale(door_tf->local_scale)
					* glm::scale(transform->scale);

				Service<RenderSystem>::Get().SubmitModel(model->path, test);
			}

			{
				auto local = &door->frame_local;

				auto test = glm::translate(transform->position)
					* glm::mat4(transform->rotation)
					* glm::translate(local->local_position)
					* glm::mat4(local->local_rotation)
					* glm::scale(local->local_scale)
					* glm::scale(transform->scale);

				Service<RenderSystem>::Get().SubmitModel(door->frame.path, test);
			}

			//Service<RenderSystem>::Get().SubmitModel(model.path.c_str(), transform);
		}

		auto view5 = ecs.view<tc::PointLight>(exclude_t<tc::Destroyed, tc::Local>());
		for (auto id : view5)
		{
			auto& light = ecs.get<tc::PointLight>(id);
			auto& ptlight = Service<RenderSystem>::Get().pt_lights[Service<RenderSystem>::Get().NumPLight];
			ptlight.hide = false;
			ptlight.Position = light.pos;
			ptlight.Color = light.color;
			Service<RenderSystem>::Get().NumPLight++;
		}

		auto view6 = ecs.view<tc::Animation, tc::Model, tc::Transform>(exclude_t<tc::Destroyed, tc::Local>());
		for (auto id : view6)
		{
			auto transform = ecs.get_if<tc::Transform>(id);
			auto model = ecs.get_if<tc::Model>(id);
			auto animation = ecs.get_if<tc::Animation>(id);

			glm::mat4 mdl(1.f);
			glm::mat4 translate = glm::translate(glm::vec3(transform->position.x, transform->position.y, transform->position.z));
			glm::mat4 rotate(transform->rotation);
			glm::mat4 scale = glm::scale(glm::vec3(transform->scale.x, transform->scale.y, transform->scale.z));

			mdl = translate * rotate * scale;
			
			Service<RenderSystem>::Get().SubmitModel(model->path, mdl, animation->id);
		}
	}
	void Instance::internal_exit()
	{
		window_manager.exit(*this);
	}

	bool Instance::load_new_scene_by_path(const tpath& path)
	{
		// try to get the name in the path
		// and assume it is valid
		auto okay = fs::exists(path) && fs::is_directory(path) && fs::exists(path / "scene.json");
		if (!okay)
		{
			LOG_WARN("Failed to load map at {0}", path.string());
			return false;
		}

		string scene_name = path.stem().string();
		current_scene_name = scene_name;
		scene = Scene();
		scene.load(path);
		LOG_INFO("Loaded {0}", current_scene_name);
		return true;
	}
	bool Instance::load_new_scene_by_name(const string& scene_name)
	{
		return load_new_scene_by_path(root / "scenes" / scene_name);
	}

	bool Instance::load_new_conflict_resolution_by_path(const tpath& load_path)
	{
		// try to get the name in the path
		string res_name = load_path.stem().string();
		auto path = load_path.parent_path();
		int a = std::atoi(path.stem().string().c_str());
		if (a >= 1 && a <= 3)
		{
			current_res_name = res_name;
			current_res_index = a;
			ecs.clear();
			ecs.load(path, res_name);
			LOG_INFO("Loaded {0}:{1}", current_res_name, current_res_index);
			return true;
		}
		LOG_WARN("Failed to load resolution at {0}", load_path.string());
		return false;
	}

	bool Instance::load_new_conflict_resolution_by_int(int i)
	{
		auto path = root / "conflict_resolutions" / std::to_string(i);
		if (i >= 1 && i <= 3 && fs::exists(path))
		{
			for (auto inside : fs::directory_iterator(path))
			{
				if (fs::is_directory(inside.path()))
				{
					return load_new_conflict_resolution_by_path(inside.path());
				}
			}
		}


		LOG_WARN("Failed to load resolution at {0}", path.string());
		return false;

	}

	bool Instance::unload_current_scene()
	{
		if (current_scene_name != "")
		{
			LOG_INFO("Unloaded {0}", current_scene_name);
			current_scene_name = "";
			scene = Scene();
			return true;
		}
		return false;
	}

	bool Instance::unload_current_conflict_resolution()
	{
		if (current_res_name != "")
		{
			LOG_INFO("Unloaded {0}:{1}", current_res_name, current_res_index);
			current_res_name = "";
			current_res_index = 0;
			ecs.clear();
			return true;
		}
		return false;
	}


	tvector<tpair<int, tpath>> Instance::get_scene_paths()
	{
		tvector<tpair<int, tpath>> paths;
		try
		{
			int i = 1;
			for (auto entry : fs::directory_iterator(root / "scenes"))
			{
				// each directory is a scene
				// if it contains a scene.json
				if (fs::exists(entry.path() / "scene.json"))
					paths.push_back(std::make_pair(i++, entry.path()));
			}
		}
		catch (const std::exception&)
		{
			// do nothing
		}

		return paths;
	}

	tvector<tpair<bool, tpath>> Instance::get_conflict_resolution_paths()
	{
		tvector<tpair<bool, tpath>> paths(3);
		try
		{
			for (auto entry : fs::directory_iterator(root / "conflict_resolutions"))
			{
				// each directory is a conflict res
				// it should be from 1 to 3
				if (fs::is_directory(entry.path()))
				{
					string a = entry.path().filename().string();
					if (a == "1" || a == "2" || a == "3")
					{
						for (auto inside : fs::directory_iterator(entry.path()))
						{
							if (fs::is_directory(inside.path()))
							{
								paths[std::atoi(a.c_str()) - 1] = std::make_pair(true, inside.path());
								break;
							}
						}
					}
				}
			}
		}
		catch (const std::exception&)
		{
			// do nothing
		}

		return paths;
	}
}