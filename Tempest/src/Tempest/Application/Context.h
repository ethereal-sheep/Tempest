#pragma once

#include <memory>
#include <Windows.h>

namespace Tempest
{
	class Application;

	class Context
	{
	public:
		[[nodiscard]] static std::unique_ptr<Context> Create();
		virtual ~Context() = default;

		virtual bool OnInit(HINSTANCE hInstance, int nCmdShow, const std::unique_ptr<Application>& pApp) = 0;
		virtual void OnShutdown() = 0;

		virtual void SwapBuffer() = 0;

		virtual HWND GetHWND() const = 0;
		virtual HDC GetHDC() const = 0;
		virtual HGLRC GeHGLRC() const = 0;

	private:
		
	};
}