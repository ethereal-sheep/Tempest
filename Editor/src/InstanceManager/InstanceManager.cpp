#include "InstanceManager.h"

// windows
#include "Window/Diagnostics/DiagnosticsWindow.h"
#include "Window/Hierarchy/HierarchyWindow.h"
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
#include "Window/Overlay/UnitSheetOverlay.h"
#include "Window/Overlay/SimulateOverlay.h"

// show recent projects
#include "Window/Util/ShowRecent.h"

namespace Tempest
{
	void InstanceManager::register_nulltime_windows()
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
		instance->register_window<DiagnosticsWindow>();
		instance->register_window<HierarchyWindow>();
		instance->register_window<InspectorWindow>();
		instance->register_window<AssetManagerWindow>();

		instance->register_always<EditTimeMenuBar>();
		instance->register_always<NewProjectPopup>();
		instance->register_always<OpenProjectPopup>();
		instance->register_always<SaveProjectPopup>();
		instance->register_always<CloseProjectPopup>();
		instance->register_always<ImportAssetPopup>();
		instance->register_always<SaveCurrentBeforeOpenPopup>();

		instance->register_always<SaveBrowser>();
		instance->register_always<ErrorMsgPopup>();
		instance->register_always<BottomRightOverlayPopup>();
		instance->register_always<UnitSheetOverlay>();
		instance->register_always<SimulateOverlay>();

		instance->register_always<ShowRecent>();
		instance->register_window<test_window>();
		instance->register_window<test_window2>();
		instance->register_window<test_window3>();
	}
	void InstanceManager::register_runtime_windows()
	{
		// assume instance is valid here
		instance->register_window<ViewportWindow>();
		instance->register_window<HierarchyWindow>();
		instance->register_window<InspectorWindow>();
		instance->register_window<DiagnosticsWindow>()->visible = false;
		instance->register_window<test_window>();
		instance->register_window<test_window3>();

		instance->register_always<RunTimeMenuBar>();
		instance->register_always<ErrorMsgPopup>();
		instance->register_always<BottomRightOverlayPopup>();
	}

}