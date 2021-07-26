
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



}
