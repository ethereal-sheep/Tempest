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
	enum SIMULATE_POPUP_TYPE{UNIT,WEAPON,ACTION, SEQUENCE, EDIT_UNIT, EDIT_WEAPON};
	enum QUICKMENU_POPUP_TYPE{SIMULATE,UNITS,ACTIONS,SEQUENCES,WEAPONS};
	enum TUTORIAL_POPUP_TYPES{SIMULATE_TUT,UNITS_TUT,GRAPH_ACTION_TUT, GRAPH_SEQUENCE_TUT, TURN_TUT, COMBATMODE_TUT, MAP_TUT};
	struct SimulateTutorialP2Trigger : public Event {};
	struct DefineStatsTrigger : public Event {};
	struct ConfirmationTrigger : public Event {};
	struct AddingUnitsTrigger : public Event {};
	struct AddingWeaponsTrigger : public Event {};
	struct AddingActionsTrigger : public Event {};
	struct SelectSequenceTrigger : public Event {};
	struct CloseAllConResOverlayTrigger : public Event {};
	struct ViewportCameraMoveTrigger : public Event
	{
		ViewportCameraMoveTrigger(bool canMove) : canMove{ canMove } {}
		bool canMove;
	};
	struct CharacterStatsTrigger : public Event
	{
		CharacterStatsTrigger(Entity a) : entity{ a } {}
		Entity entity;
	};
	struct WeaponStatsTrigger : public Event 
	{
		WeaponStatsTrigger(Entity a) : entity{ a } {}
		Entity entity;
	};
	struct SimulatePopupTrigger : public Event
	{
		SimulatePopupTrigger(SIMULATE_POPUP_TYPE type, bool is_attacker, Entity data, bool for_unitpage = false) :
			type{ type }, is_attacker{ is_attacker }, data{ data }, for_unitpage{ for_unitpage }{}
		SIMULATE_POPUP_TYPE type;
		bool is_attacker;
		bool for_unitpage;
		Entity data;
	};

	struct QuickMenuPopupTrigger : public Event
	{
		QuickMenuPopupTrigger(QUICKMENU_POPUP_TYPE current) : current{ current } {}
		QUICKMENU_POPUP_TYPE current;
	};

	struct CloseOverlayTrigger : public Event
	{
		CloseOverlayTrigger(QUICKMENU_POPUP_TYPE current) : current{ current } {}
		QUICKMENU_POPUP_TYPE current;
	};


	// add another bool to check for results
	struct PauseOverlayTrigger : public Event
	{
		PauseOverlayTrigger(bool canOpenGraph = false, bool fromCombatMode = false, bool fromPlaceUnit = false) :
			canOpenGraph{ canOpenGraph }, fromCombatMode{ fromCombatMode }, fromPlaceUnit{ fromPlaceUnit }{}
		bool canOpenGraph;
		bool fromCombatMode;
		bool fromPlaceUnit;
	};

	//Confirm Trigger
	struct SimulateSelectionConfirm : public Event
	{
		SimulateSelectionConfirm(SIMULATE_POPUP_TYPE type, bool is_attacker, Entity data, bool for_unitpage = false) :
			type{ type }, is_attacker{ is_attacker }, data{ data }, for_unitpage{ for_unitpage }{}
		SIMULATE_POPUP_TYPE type;
		bool is_attacker;
		bool for_unitpage;
		Entity data;
	};
	struct TutorialPopupTrigger : public Event 
	{
		TutorialPopupTrigger(TUTORIAL_POPUP_TYPES t) : type{ t } {}
		TUTORIAL_POPUP_TYPES type;
	};

	struct MainMenuSequencePopupTrigger : public Event
	{
		MainMenuSequencePopupTrigger(std::vector<int>& selections) : selected_seqs{ selections } {}

		std::vector<int>& selected_seqs;
	};

	//Overlay Trigger
	struct OpenSimulateTrigger : public Event 
	{
		OpenSimulateTrigger(Instance& in): instance{ in }  {}
		Instance& instance;
	};

	struct OpenSimulateResultTrigger : public Event
	{
		OpenSimulateResultTrigger(Entity a, Entity d, Entity c):
			atk(a), def(d), conflict(c){}
		Entity atk;
		Entity def;
		Entity conflict;
	};
	struct OpenCombatModeTrigger : public Event 
	{
		OpenCombatModeTrigger(Instance& in, const tvector<Entity>& _units) : instance{ in }, units { _units }  {}
		Instance& instance;
		tvector<Entity> units;
	};
	struct CombatModeVisibility : public Event
	{
		CombatModeVisibility(bool val, Instance& in) : isVisible{ val }, instance{ in } {}
		bool isVisible = false;
		Instance &instance;
	};

	struct OpenConflictResTrigger : public Event {};
	struct OpenMainMenuTrigger : public Event
	{
		OpenMainMenuTrigger(int type = 0) : menuType{ type } {}
		int menuType;
	};

	struct OpenActionGraphTrigger : public Event // not using for new change
	{
		OpenActionGraphTrigger(Entity entityid, Instance& in) : id{ entityid }, instance{in} {}
		Entity id = UNDEFINED;
		Instance& instance;
	};

	enum OPEN_GRAPH_TYPE{GRAPH_ACTION, GRAPH_SEQUENCE};
	struct OpenGraphTrigger : public Event
	{
		OpenGraphTrigger(Entity entityid, Instance& in, OPEN_GRAPH_TYPE type, bool isDuringCombat = false) :
			id{ entityid }, instance{ in }, type{ type }, duringCombat{ isDuringCombat }{}
		Entity id = UNDEFINED;
		Instance& instance;
		OPEN_GRAPH_TYPE type;
		bool duringCombat;
	};
	struct OpenUnitSheetTrigger : public Event 
	{
		OpenUnitSheetTrigger(bool isAddUnit, Instance& in, Entity id = UNDEFINED) : addUnit{ isAddUnit }, instance{ in }, entityID{ id } {}
		bool addUnit = false;
		Entity entityID = UNDEFINED;
		Instance& instance;
	};
	struct OpenWeaponSheetTrigger : public Event
	{
		OpenWeaponSheetTrigger(bool isAddUnit, Instance& in, Entity selected = UNDEFINED) :
			addUnit{ isAddUnit }, instance{ in }, entity{ selected } {}
		bool addUnit = false;
		Instance& instance;
		Entity entity;
	};
	struct OpenTurnOrderOverlay : public Event
	{
		OpenTurnOrderOverlay(bool isNewInstance = true, tvector<Entity> exsitingEntities = tvector<Entity>{}) : newInstance{ isNewInstance }, entities{ exsitingEntities } {}
		bool newInstance;
		tvector<Entity> entities;
	};

	struct OpenPlaceUnitsOverlay : public Event 
	{
		OpenPlaceUnitsOverlay(const tvector<Entity> entity, Instance& in, bool openNewCombat = true) : entities{ entity }, instance{ in }, openNewCombat{ openNewCombat } {}
		tvector<Entity> entities;
		bool openNewCombat;
		Instance& instance;
	};
	struct OpenBuildModeOverlay : public Event
	{
		OpenBuildModeOverlay(Instance& in) : instance{ in } {}
		Instance& instance;
	};

	struct BuildModeTutorialIndexTrigger : public Event
	{
		BuildModeTutorialIndexTrigger(int indexNum) : index{ indexNum } {}
		int index;
	};

	struct SaveCurrentBeforeOpenTrigger : public Event 
	{
		SaveCurrentBeforeOpenTrigger(const tpath& path) : open_path{ path } {}
		tpath open_path;
	};


	struct GetRecentUtil : public Event
	{
		GetRecentUtil(std::vector<fs::path>& p) : paths{ p } {}
		std::vector<fs::path>& paths;
	};

	struct ShowRecentUtil : public Event {};
	struct AddRecentUtil : public Event {};
	struct ToggleMenuBar : public Event {};

	struct ChangeTurnOrder : public Event
	{
		ChangeTurnOrder(tvector<Entity> newTurnOrder) : entities { newTurnOrder } {}
		tvector<Entity> entities;
	};

	struct WipeTrigger : public Event
	{
		WipeTrigger(float _fade_in_time = 0.15f, float _fade_out_time = 0.15f, float _visible_time = 0.f, std::function<void(void)> _do_on_fade = []() {}) : fade_in_time(_fade_in_time), fade_out_time(_fade_out_time), visible_time(_visible_time), do_on_fade(_do_on_fade)
		{

		}

		bool force = false;
		float fade_in_time, fade_out_time, visible_time;
		std::function<void(void)> do_on_fade;
	};

	struct WipeColourTrigger : public Event
	{
		WipeColourTrigger(
			glm::vec3 _colour,
			float _fade_in_time = 0.15f, 
			float _fade_out_time = 0.15f, 
			float _visible_time = 0.f, 
			std::function<void(void)> _do_on_fade = []() {},
			std::function<void(void)> _do_on_end = []() {})
			: colour(_colour), fade_in_time(_fade_in_time), fade_out_time(_fade_out_time), visible_time(_visible_time), do_on_fade(_do_on_fade), do_on_end(_do_on_end)
		{

		}
		glm::vec3 colour;
		float fade_in_time, fade_out_time, visible_time;
		std::function<void(void)> do_on_fade;
		std::function<void(void)> do_on_end;
	};

	struct DelayTrigger : public Event
	{
		DelayTrigger(float _wait_time, std::function<void(void)> _do) : wait_time(_wait_time), do_fn(_do)
		{

		}
		float wait_time;
		std::function<void(void)> do_fn;
	};

	struct SettingsTrigger : public Event {};

	struct LoadTrigger : public Event
	{
		std::function<bool(void)> do_until_true_fn = []() { return true; };
		std::function<void(void)> do_at_end_fn = [](){};
	};

	// for ending screen
	struct UnitResult
	{
		unsigned short dmg_done{ 0 };
		unsigned short dmg_taken{ 0 };
		unsigned short defeated{ 0 };
		unsigned short total_attacks{ 0 };
		unsigned short successful_attacks{ 0 };
		unsigned short deaths{ 0 };
	};

	struct CombatResultsTrigger : public Event {};
	struct CreditsTrigger : public Event {};

	struct SendCombatResults : public Event
	{
		SendCombatResults(std::map<Entity, UnitResult> results) : units_results{ results } {}
		std::map<Entity, UnitResult>  units_results;
	};

	struct PlayPlaceUnitCinematic : public Event
	{
		PlayPlaceUnitCinematic(Instance& in, tvector<Entity> newTurnOrder) 
			: instance{ in }, entities{ newTurnOrder } {}
		Instance& instance;
		tvector<Entity> entities;
	};
}