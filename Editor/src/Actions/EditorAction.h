/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once
#include "Actions/IAction.h"
#include "Core.h"
#include "ECS/Entity.h"
#include "Instance/Instance.h"

namespace Tempest
{
	class CreatePrefab final : public IAction
	{
		id_t id;
		std::optional<tmap<id_t, prefab>::node_type> node;

	public:
		CreatePrefab(id_t prefab_id) : id(prefab_id) {  }
		const char* Name() override { return "Create Prefab"; }
		void Undo(Instance& instance) override { LOG_ASSERT(instance.scene.get_map().exist(id)); node = instance.scene.get_map().extract(id); }
		void Redo(Instance& instance) override { 
			LOG_ASSERT(node); 
			auto [it, b, a] = instance.scene.get_map().insert(std::move(*node));
			LOG_ASSERT(b);
			id = it->first;
		}
	};

	class DeletePrefab final : public IAction
	{
		id_t id;
		std::optional<tmap<id_t, prefab>::node_type> node;

	public:
		DeletePrefab(tmap<id_t, prefab>::node_type&& node_t) : id(node_t.key()), node(std::move(node_t)) {  }
		const char* Name() override { return "Delete Prefab"; }
		void Undo(Instance& instance) override {
			LOG_ASSERT(node);
			auto [it, b, a] = instance.scene.get_map().insert(std::move(*node));
			LOG_ASSERT(b);
			id = it->first;
		}
		void Redo(Instance& instance) override { LOG_ASSERT(instance.scene.get_map().exist(id)); node = instance.scene.get_map().extract(id); }
	};

	class TransformPrefab final : public IAction
	{
		id_t id;
		tc::Transform form;

	public:
		TransformPrefab(id_t prefab_id, const tc::Transform& f) : id(prefab_id), form(f) {  }
		const char* Name() override { return "Transform Prefab"; }
		void Undo(Instance& instance) override {
			LOG_ASSERT(instance.scene.get_map().exist(id)); 
			std::swap(instance.scene.get_map().get(id).force<tc::Transform>(), form);
		}

		void Redo(Instance& instance) override {
			LOG_ASSERT(instance.scene.get_map().exist(id));
			std::swap(instance.scene.get_map().get(id).force<tc::Transform>(), form);
		}
	}; 

}
