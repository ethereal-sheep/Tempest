#pragma once
#include "Context.h"

namespace Tempest
{
	class AppHandler
	{
	public:
		static int Run(std::unique_ptr<Application>&& pApp, HINSTANCE hInstance, LPWSTR lpCmdList, int nCmdShow);
		static std::unique_ptr<Application>& GetApp() { return s_pApp; }

		static void ToggleFullscreen();
		static void SetFullscreen(bool val);

	protected:
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		static ATOM RegisterWindow(HINSTANCE hInstance);
		static bool InitInstance(HINSTANCE hInstance, int nCmdShow);

		inline static std::unique_ptr<Application> s_pApp = nullptr;
		inline static std::unique_ptr<Context> s_pContext = nullptr;

		inline static bool s_IsFullscreen = false;
		inline static WINDOWPLACEMENT s_WPC;
		inline static LONG s_HWNDStyle = 0;
		inline static LONG s_HWNDStyleEx = 0;

	};
}
