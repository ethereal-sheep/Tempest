#pragma once
#include "Window.h"

namespace UI
{
	Window::Window(const std::string& name)
		: m_Name{ name }
	{
	}

	std::string Window::GetName() const
	{
		return m_Name;
	}

	bool* Window::GetToggle()
	{
		return &m_toggle;
	}

	void Window::LockWindow()
	{
		m_WindowFlags |= ImGuiWindowFlags_NoMove;
	}

	void Window::UnlockWindow()
	{
		m_WindowFlags &= ~ImGuiWindowFlags_NoMove;
	}
}
