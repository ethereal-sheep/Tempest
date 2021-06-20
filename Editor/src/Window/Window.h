
#pragma once
#include <string>
#include "imgui/imgui.h"

namespace UI
{
	class Window
	{
	public:
		Window(const std::string& name = "nameless");

		virtual void Init() = 0;
		virtual void Show() = 0;
		virtual void Shutdown() = 0;

		std::string GetName() const;
		bool* GetToggle();

		void LockWindow();
		void UnlockWindow();

		virtual ~Window() = default;

	protected:
		std::string m_Name;
		bool m_toggle = true;
		ImGuiWindowFlags m_WindowFlags = 0;
	};
}