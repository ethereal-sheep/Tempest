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