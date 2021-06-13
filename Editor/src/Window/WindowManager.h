#include <unordered_map>
#include <string>
#include <memory>
#include "Window.h"
#include "MenuBar.h"
namespace UI
{
	class WindowManager
	{
	public:
		void Initialize();
		void StartupWindows();
		void RegisterWindow(std::string name, std::unique_ptr<Window> window);
		void InitMenuBar(MenuBar& menuBar);
		void DisplayWindows();
		void ShowAllWindow();
		void ShutdownWindows();
		std::unordered_map<std::string, std::unique_ptr<Window>>& GetWindow();

		bool m_LockWindow = false;
	private:
		//DockingWindow m_DockingWindow;
		std::unordered_map<std::string, std::unique_ptr<Window>> m_WindowItems;
	};
}