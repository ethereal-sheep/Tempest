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

	//Overlay Trigger
	struct OpenSimulateTrigger : public Event {};
	struct OpenCombatModeTrigger : public Event {};
	struct OpenConflictResTrigger : public Event {};
	struct OpenActionGraphTrigger : public Event 
	{
		OpenActionGraphTrigger(Entity entityid) : id{ entityid } {}
		Entity id = UNDEFINED;
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