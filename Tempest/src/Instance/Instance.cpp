/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "Instance.h"
#include "Graphics/Basics/RenderSystem.h"

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
			auto model = ecs.get_if<tc::Model>(id);
			auto transform = ecs.get_if<tc::Transform>(id);
			auto local = ecs.get_if<tc::Local>(id);
			auto door = ecs.get_if<tc::Door>(id);

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
	}
	void Instance::internal_exit()
	{
		window_manager.exit(*this);
	}
}