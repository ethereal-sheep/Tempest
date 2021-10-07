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
#include "IAction.h"
#include "Core.h"
#include "ECS/Entity.h"
#include "Instance/Instance.h"

namespace Tempest
{
	/************************************************************
	* Action:	AddEntity
	* Undo:		Removes added entity
	* Redo:		Readds added entity
	*************************************************************/
	class AddEntity final : public IAction
	{
		Entity added;
	public:
		AddEntity(
			Entity entity
		) : added{ entity }
		{

		}

		const char* Name() override
		{
			return "Add Entity";
		}
		void Undo(Instance& instance) override
		{
			if (added == instance.selected)
				instance.selected = INVALID;
			// need to do rb stuff here as well
			if (auto rb = instance.ecs.get_if<tc::Rigidbody>(added))
			{
				instance.po.RemoveActorFromScene(rb->internal_rb.get());
			}
			instance.ecs.emplace<tc::Destroyed>(added);
		}
		void Redo(Instance& instance) override
		{
			instance.ecs.remove<tc::Destroyed>(added);
			// need to do rb stuff here as well
			if (auto rb = instance.ecs.get_if<tc::Rigidbody>(added))
			{
				instance.po.AddActorToScene(rb->internal_rb.get());
			}
		}
	};

	/************************************************************
	* Action:	RemoveEntity
	* Undo:		Renames Layer to old name
	* Redo:		Renames Layer to correct new name
	*************************************************************/
	class RemoveEntity final : public IAction
	{
		Entity removed;
	public:
		RemoveEntity(
			Entity entity
		) : removed{ entity }
		{

		}

		const char* Name() override
		{
			return "Remove Entity";
		}
		void Undo(Instance& instance) override
		{
			instance.ecs.remove<tc::Destroyed>(removed);
			// need to do rb stuff here as well
			if (auto rb = instance.ecs.get_if<tc::Rigidbody>(removed))
			{
				instance.po.AddActorToScene(rb->internal_rb.get());
			}
		}
		void Redo(Instance& instance) override
		{
			if (removed == instance.selected)
				instance.selected = INVALID;
			// need to do rb stuff here as well
			if (auto rb = instance.ecs.get_if<tc::Rigidbody>(removed))
			{
				instance.po.RemoveActorFromScene(rb->internal_rb.get());
			}
			instance.ecs.emplace<tc::Destroyed>(removed);
		}
	};
	/**********************************************************************
	* Physx Rigidbody
	***********************************************************************/
	/************************************************************
	* Action:	EditShape
	* Undo:		Change Shape to old shape
	* Redo:		Change Shape to new shape
	*************************************************************/
	class EditShape final : public IAction
	{
		Entity ID;
		SHAPE_TYPE oldShape;
		SHAPE_TYPE newShape;
	public:
		EditShape(Entity entity, SHAPE_TYPE old_Shape, SHAPE_TYPE new_Shape);
		const char* Name() override;
		void Undo(Instance&) override;
		void Redo(Instance&) override;
	};
	/************************************************************
	* Action:	EditGravity
	* Undo:		Change gravity to previous bool
	* Redo:		Change gravity to new bool
	*************************************************************/
	class EditGravity final : public IAction
	{
		Entity ID;
		bool gravity;
	public:
		EditGravity(Entity entity, bool _gravity);
		const char* Name() override;
		void Undo(Instance&) override;
		void Redo(Instance&) override;
	};
	/************************************************************
	* Action:	EditStatic
	* Undo:		Change isStatic to previous bool
	* Redo:		Change isStatic to new bool
	*************************************************************/
	class EditStatic final : public IAction
	{
		Entity ID;
		bool isStatic;
	public:
		EditStatic(Entity entity, bool _static);
		const char* Name() override;
		void Undo(Instance&) override;
		void Redo(Instance&) override;
	};

	/************************************************************
	* Action:	EditPosition
	* Undo:		Change position to previous vec3
	* Redo:		Change position to new vec3
	*************************************************************/
	class EditPosition final : public IAction
	{
		Entity ID;
		vec3 old_position;
		vec3 new_position;

	public:
		EditPosition(Entity entity, vec3 oldPos, vec3 newPos);
		const char* Name() override;
		void Undo(Instance& instance) override;
		void Redo(Instance& instance) override;
	};

	/************************************************************
	* Action:	EditRotation
	* Undo:		Change rotation to previous vec3
	* Redo:		Change rotation to new vec3
	*************************************************************/
	class EditRotation final : public IAction
	{
		Entity ID;
		glm::highp_vec3 old_rotation;
		glm::highp_vec3 new_rotation;

	public:
		EditRotation(Entity entity, glm::highp_vec3 oldRotation, glm::highp_vec3 newRotation);
		const char* Name() override;
		void Undo(Instance& instance) override;
		void Redo(Instance& instance) override;
	};

	/************************************************************
	* Action:	EditScale
	* Undo:		Change scale to previous vec3
	* Redo:		Change scale to new vec3
	*************************************************************/
	class EditScale final : public IAction
	{
		Entity ID;
		vec3 old_scale;
		vec3 new_scale;

	public:
		EditScale(Entity entity, vec3 oldScale, vec3 newScale);
		const char* Name() override;
		void Undo(Instance& instance) override;
		void Redo(Instance& instance) override;
	};

}
