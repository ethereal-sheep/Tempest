#pragma once
#include <unordered_map>
#include <string>
#include <filesystem>
namespace UI
{
	enum class ButtonState
	{
		PLAY = 0,
		PAUSE,
		STOP
	};

	class MenuBar
	{
	public:
		void Show();
		void UpdateWindowItems(
			std::string name,
			bool* Toggle,
			bool* Lock);
		bool GetIsPaused() const { return m_isPaused; }

	private:
		void ShowProject();
		void ShowEdit();
		void ShowWindow();
		void ShowHelp();
		void ShowInvalidFilePopup();
		void GameLoopButton();
		void SaveEntityState();
		// Mapping Item name to bool variables
		std::unordered_map<std::string, bool*> m_WindowItems;

		// For storing file paths
		std::filesystem::path m_FilePath = "";

		// Bool variables
		bool* m_LockWindow = nullptr;
		bool m_ShowDemo = false;
		bool m_ShowImPlotDemo = false;
		bool m_ShowInvalidFilePopup = false;
		bool m_ShowDirectories = false;

		// Game Loop Buttons
		bool m_isPaused = true;

		ButtonState m_state = ButtonState::STOP;
	};
}