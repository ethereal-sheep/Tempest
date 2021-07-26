
#pragma once
#include "IAction.h"
#include "Core.h"
#include "ECS/Entity.h"
#include "Instance/Instance.h"

namespace Tempest
{
	/************************************************************
	* Action:	Rename Layer
	* Undo:		Renames Layer to old name
	* Redo:		Renames Layer to correct new name
	*************************************************************/
	class AddEntity final : public IAction
	{
		Entity added;
	public:
		AddEntity(
			Entity entity
		) : added{ entity }	
		{}

		const char* Name() override
		{
			return "Add Entity";
		}
		void Undo(Instance& ) override
		{

		}
		void Redo(Instance& ) override
		{

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
