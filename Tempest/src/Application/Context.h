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
#include <glew.h>
#include <memory>
#include <Windows.h>

namespace Tempest
{
	class Application;

	class Context
	{
	public:
		[[nodiscard]] static std::unique_ptr<Context> Create();
		~Context() = default;
		
		bool OnInit(HINSTANCE hInstance, int nCmdShow, const std::unique_ptr<Application>& pApp);
		void OnShutdown();
		
		void SwapBuffer();
		
		HWND GetHWND() const 	{ return m_WND; }
		HDC GetHDC() const 	{ return m_HDC; }
		HGLRC GetHGLRC() const { return m_RC; }

	private:
		
		HWND  m_WND;	// Window
		HDC   m_HDC;	// Device Context
		HGLRC m_RC;		// Render Context

		bool Create_Window(HINSTANCE hInstance, const std::unique_ptr<Application>& pApp);
	};
}