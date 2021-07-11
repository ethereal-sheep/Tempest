
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
		void Undo(Instance& instance) override
		{

		}
		void Redo(Instance& instance) override
		{

		}
	};


}
