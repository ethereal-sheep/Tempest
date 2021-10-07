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
#include "Events/Events/IEvent.h"

namespace Tempest
{
	struct BottomRightOverlayTrigger : public Event
	{
		BottomRightOverlayTrigger(std::string a) : msg(a) {}
		std::string msg;
	};

	struct ErrorTrigger : public Event
	{
		ErrorTrigger(std::string a) : msg(a) {}
		std::string msg;
	};

	struct NewProjectTrigger : public Event {};
	struct OpenProjectTrigger : public Event {};
	struct SaveProjectTrigger : public Event {};
	struct SaveProjectAsTrigger : public Event {};
	struct CloseProjectTrigger : public Event {};
	struct ExportProjectTrigger : public Event {};
	struct ImportAssetTrigger : public Event {};
	//Popup Trigger
	struct DefineStatsTrigger : public Event {};
	struct ConfirmationTrigger : public Event {};

	//Overlay Trigger
	struct OpenSimulateTrigger : public Event {};
	struct OpenSimulateResultTrigger : public Event
	{
		OpenSimulateResultTrigger(Entity a, Entity d, Entity c) :
			atk(a), def(d), conflict(c){}
		Entity atk;
		Entity def;
		Entity conflict;
	};
	struct OpenCombatModeTrigger : public Event {};
	struct OpenConflictResTrigger : public Event {};
	struct OpenMainMenuTrigger : public Event {};
	struct OpenActionGraphTrigger : public Event 
	{
		OpenActionGraphTrigger(Entity entityid, Instance& in) : id{ entityid }, instance{in} {}
		Entity id = UNDEFINED;
		Instance& instance;
	};
	struct OpenUnitSheetTrigger : public Event 
	{
		OpenUnitSheetTrigger(bool isAddUnit, Entity id = UNDEFINED) : addUnit{ isAddUnit }, entityID{ id } {}
		bool addUnit = false;
		Entity entityID = UNDEFINED;
	};

	
	struct SaveCurrentBeforeOpenTrigger : public Event 
	{
		SaveCurrentBeforeOpenTrigger(const tpath& path) : open_path{ path } {}
		tpath open_path;
	};

	struct ShowRecentUtil : public Event {};
	struct AddRecentUtil : public Event {};
}