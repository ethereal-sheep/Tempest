/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "InstanceManager.h"

// windows
#include "Window/Diagnostics/DiagnosticsWindow.h"
#include "Window/Hierarchy/HierarchyWindow.h"
#include "Window/Hierarchy/PrefabList.h"
#include "Window/Inspector/InspectorWindow.h"
#include "Window/Menubar/EditTimeMenuBar.h"
#include "Window/Menubar/RunTimeMenuBar.h"
#include "Window/Viewport/ViewportWindow.h"
#include "Window/MainMenu/MainMenu.h"
#include "Window/AssetManager/AssetManagerWindow.h"

// test window
#include "Window/Test/test_window.h"
#include "Window/Test/test_window2.h"
#include "Window/Test/test_window3.h"
#include "Window/Test/test_window4.h"
#include "Window/Test/Test_UIWindow.h"


// filebrowsers (maybe dn)
#include "Window/FileBrowser/SaveBrowser.h"
#include "Window/FileBrowser/NewBrowser.h"

// popup utils
#include "Window/Popup/NewProjectPopup.h"
#include "Window/Popup/OpenProjectPopup.h"
#include "Window/Popup/SaveProjectPopup.h"
#include "Window/Popup/ExportProjectPopup.h"
#include "Window/Popup/CloseProjectPopup.h"
#include "Window/Popup/ImportAssetPopup.h"
#include "Window/Popup/ErrorMsgPopup.h"
#include "Window/Popup/SaveCurrentBeforeOpenPopup.h"
#include "Window/Popup/BottomRightOverlayPopup.h"
#include "Window/Popup/DefineStatsPopup.h"
#include "Window/Popup/SimulateSelectionPopup.h"
#include "Window/Popup/QuickMenuPopup.h" 
#include "Window/Popup/AddWeaponPopup.h"
#include "Window/Popup/AddUnitsPopup.h"
#include "Window/Popup/AddActionPopup.h"
#include "Window/Popup/SelectSequencePopup.h"
#include "Window/Popup/TutorialPopup.h"
#include "Window/Popup/MainMenuSequencePopup.h"

#include "Window/Util/SimulationBuilder.h"
#include "Window/Util/SimulationStart.h"

// Overlays
#include "Window/Overlay/UnitSheetOverlay.h"
#include "Window/Overlay/WeaponSheetOverlay.h"
#include "Window/Overlay/SimulateOverlay.h"
#include "Window/Overlay/CombatModeOverlay.h"
#include "Window/Overlay/ConflictResOverlay.h"
#include "Window/Overlay/AttackSystemOverlay.h"
#include "Window/Overlay/MainMenuOverlay.h"
#include "Window/Overlay/SimulateResultOverlay.h"
#include "Window/Overlay/TurnOrderOverlay.h"
#include "Window/Overlay/PlaceUnitsOverlay.h"
#include "Window/Overlay/BuildModeOverlay.h"
#include "Window/Overlay/PauseOverlay.h"

// show recent projects
#include "Window/Util/ShowRecent.h"

namespace Tempest
{
	void InstanceManager::register_nulltime_windows()
	{
		// assume instance is valid here
		instance->register_window<DiagnosticsWindow>()->visible = false;
		instance->register_always<MainMenuOverlay>();
		//instance->register_always<MainMenuWindow>();
		

		instance->register_always<NewProjectPopup>();
		instance->register_always<OpenProjectPopup>();
		instance->register_always<SaveCurrentBeforeOpenPopup>();
		instance->register_always<ErrorMsgPopup>();
		instance->register_always<BottomRightOverlayPopup>();

		instance->register_always<ShowRecent>();
	}

	void InstanceManager::register_physicstime_windows()
	{
		// assume instance is valid here
		instance->register_window<DiagnosticsWindow>()->visible = false;

		instance->register_always<MainMenuWindow>();

		instance->register_always<NewProjectPopup>();
		instance->register_always<OpenProjectPopup>();
		instance->register_always<SaveCurrentBeforeOpenPopup>();
		instance->register_always<ErrorMsgPopup>();
		instance->register_always<BottomRightOverlayPopup>();

		instance->register_always<ShowRecent>();
	}
	void InstanceManager::register_edittime_windows()
	{
		// assume instance is valid here
		instance->register_window<ViewportWindow>();
		
		instance->register_window<PrefabList>();
		
		//instance->register_window<AssetManagerWindow>();
		instance->register_window<DiagnosticsWindow>()->visible = false;
		//instance->register_window<HierarchyWindow>()->visible = false;
		//instance->register_window<InspectorWindow>()->visible = false;

		instance->register_window<test_window>()->visible = false;
		//instance->register_window<test_window2>()->visible = false;
		//instance->register_window<Test_UIWindow>();
		//instance->register_window<test_window3>()->visible = false;

		instance->register_always<EditTimeMenuBar>();
		instance->register_always<NewProjectPopup>();
		instance->register_always<OpenProjectPopup>();
		instance->register_always<SaveProjectPopup>();
		instance->register_always<CloseProjectPopup>();
		instance->register_always<ImportAssetPopup>();
		instance->register_always<SaveCurrentBeforeOpenPopup>();
		instance->register_always<DefineStatsPopup>();
		instance->register_always<SimulateSelectionPopup>();
		instance->register_always<QuickMenuPopup>();
		instance->register_always<AddWeaponPopup>();
		instance->register_always<AddUnitsPopup>();
		instance->register_always<AddActionPopup>();
		instance->register_always<SelectSequencePopup>();
		instance->register_always<TutorialPopup>();
		instance->register_always<MainMenuSequencePopup>();


		instance->register_always<SaveBrowser>();
		instance->register_always<ErrorMsgPopup>();
		instance->register_always<BottomRightOverlayPopup>();
		instance->register_always<UnitSheetOverlay>();
		instance->register_always<WeaponSheetOverlay>();
		instance->register_always<SimulateOverlay>();
		instance->register_always<SimulateResultOverlay>();
		instance->register_always<AttackSystemOverlay>();
		instance->register_always<MainMenuOverlay>();
		instance->register_always<BuildModeOverlay>();
		instance->register_always<PauseOverlay>();

		instance->register_always<SimulationStart>();
		

		instance->register_always<ShowRecent>();
	}
	void InstanceManager::register_runtime_windows()
	{
		// assume instance is valid here
		//instance->register_window<ViewportWindow>();
		instance->register_window<HierarchyWindow>()->visible = false;
		//instance->register_window<InspectorWindow>()->visible = false;
		instance->register_window<DiagnosticsWindow>()->visible = false;
		//instance->register_window<test_window>()->visible = false;
		//instance->register_window<test_window3>();
		instance->register_always<ConflictResOverlay>();
		instance->register_always<CombatModeOverlay>();
		instance->register_always<TurnOrderOverlay>();
		instance->register_always<PlaceUnitsOverlay>();
		instance->register_always<PauseOverlay>();
		//instance->register_always<TurnOrderOverlay>();
		instance->register_always<SimulationStart>();
		//instance->register_window<test_window4>();

		instance->register_always<RunTimeMenuBar>();
		instance->register_always<ErrorMsgPopup>();
		instance->register_always<BottomRightOverlayPopup>();
	}

}