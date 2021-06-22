#include "InstanceManager.h"
#include "Window/Test/test_window.h"
#include "Window/Diagnostics/DiagnosticsWindow.h"
#include "Window/Hierarchy/HierarchyWindow.h"
#include "Window/Inspector/InspectorWindow.h"
#include "Window/Menubar/EditTimeMenuBar.h"
#include "Window/Error/test_error.h"
#include "Window/Overlay/test_overlay_popup.h"
#include "Window/FileBrowser/SaveBrowser.h"

namespace Tempest
{
	void InstanceManager::register_edittime_windows()
	{
		// assume instance is valid here
		instance->register_window<DiagnosticsWindow>();
		instance->register_window<HierarchyWindow>();
		instance->register_window<InspectorWindow>();

		instance->register_always<EditTimeMenuBar>();
		instance->register_always<SaveBrowser>();
		instance->register_always<test_error>();
		instance->register_always<test_overlay_popup>();
		
	}
	void InstanceManager::register_runtime_windows()
	{
		// assume instance is valid here
		instance->register_window<DiagnosticsWindow>();
	}

}