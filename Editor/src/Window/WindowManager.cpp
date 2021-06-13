#include "WindowManager.h"
#include "AssetManager/AssetManagerWindow.h"

namespace UI
{
	void WindowManager::Initialize()
	{
		// Register windows here (MUST BE BEFORE INITIALIZING MENU BAR)
		RegisterWindow("AssetManagerWindow", std::make_unique<AssetManagerWindow>());
	}

	void WindowManager::StartupWindows()
	{
		for (auto& window : m_WindowItems)
		{
			window.second->Init();
		}
	}

	void WindowManager::RegisterWindow(std::string name, std::unique_ptr<Window> window)
	{
		m_WindowItems.emplace(std::move(name), std::move(window));
	}

	void WindowManager::InitMenuBar(MenuBar& menuBar)
	{
		for (auto& [name, window] : m_WindowItems)
		{
			menuBar.UpdateWindowItems(window->GetName(), window->GetToggle(), &m_LockWindow);
		}
	}

	void WindowManager::DisplayWindows()
	{
		// Display windows
		for (auto& [name, window] : m_WindowItems)
		{
			if (m_LockWindow)
			{
				window->LockWindow();
			}
			else
			{
				window->UnlockWindow();
			}

			window->Show();
		}

	}

	void WindowManager::ShowAllWindow()
	{
		for (auto& [name, window] : m_WindowItems)
		{
			*window->GetToggle() = true;
		}
	}

	void WindowManager::ShutdownWindows()
	{
		for (auto& window : m_WindowItems)
		{
			window.second->Shutdown();
		}
	}

	std::unordered_map<std::string, std::unique_ptr<Window>>& WindowManager::GetWindow()
	{
		return m_WindowItems;
	}
}
