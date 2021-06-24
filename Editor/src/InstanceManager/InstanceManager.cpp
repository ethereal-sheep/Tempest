#include "InstanceManager.h"
#include "Window/Test/test_window.h"
#include "Window/Diagnostics/DiagnosticsWindow.h"
#include "Window/Hierarchy/HierarchyWindow.h"
#include "Window/Inspector/InspectorWindow.h"
#include "Window/Menubar/EditTimeMenuBar.h"
#include "Window/Viewport/ViewportWindow.h"

#include "Window/Error/test_error.h"
#include "Window/Overlay/test_overlay_popup.h"

#include "Window/FileBrowser/SaveBrowser.h"
#include "Window/FileBrowser/NewBrowser.h"
#include "Window/MainMenu/MainMenu.h"

#include "Window/Popup/NewProjectPopup.h"
#include "Window/Popup/OpenProjectPopup.h"
#include "Window/Popup/SaveProjectPopup.h"
#include "Window/Popup/ExportProjectPopup.h"
#include "Window/Popup/CloseProjectPopup.h"
#include "Window/Popup/SaveCurrentBeforeOpenPopup.h"

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

		instance->register_always<test_error>();
		instance->register_always<test_overlay_popup>();

		instance->register_always<ShowRecent>();
	}
	void InstanceManager::register_edittime_windows()
	{
		// assume instance is valid here
		instance->register_window<DiagnosticsWindow>();
		instance->register_window<HierarchyWindow>();
		instance->register_window<InspectorWindow>();
		instance->register_window<ViewportWindow>();

		instance->register_always<EditTimeMenuBar>();
		instance->register_always<NewProjectPopup>();
		instance->register_always<OpenProjectPopup>();
		instance->register_always<SaveProjectPopup>();
		instance->register_always<CloseProjectPopup>();
		instance->register_always<SaveCurrentBeforeOpenPopup>();

		instance->register_always<SaveBrowser>();
		instance->register_always<test_error>();
		instance->register_always<test_overlay_popup>();

		instance->register_always<ShowRecent>();
		
	}
	void InstanceManager::register_runtime_windows()
	{
		// assume instance is valid here
		instance->register_window<DiagnosticsWindow>()->visible = false;;
		instance->register_always<test_error>();
		instance->register_always<test_overlay_popup>();
	}

}