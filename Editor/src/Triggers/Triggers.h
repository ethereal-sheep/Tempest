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
	struct OpenSimulateTrigger : public Event {};
	struct OpenCombatModeTrigger : public Event {};
	struct OpenConflictResTrigger : public Event {};
	struct OpenActionGraphTrigger : public Event 
	{
		OpenActionGraphTrigger(Entity entityid, Instance& in) : id{ entityid }, instance{in} {}
		Entity id = UNDEFINED;
		Instance& instance;
	};
	struct OpenUnitSheetTrigger : public Event 
	{
		OpenUnitSheetTrigger(bool isAddUnit) : addUnit{isAddUnit} {}
		bool addUnit = false;
	};
	struct SaveCurrentBeforeOpenTrigger : public Event 
	{
		SaveCurrentBeforeOpenTrigger(const tpath& path) : open_path{ path } {}
		tpath open_path;
	};

	struct ShowRecentUtil : public Event {};
	struct AddRecentUtil : public Event {};

}