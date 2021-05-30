#include "AppHandler.h"
#include "Application.h"

namespace Tempest
{
	int AppHandler::Run(std::unique_ptr<Application>&& pApp, HINSTANCE hInstance, [[maybe_unused]] LPWSTR lpCmdList, int nCmdShow)
	{
		s_pApp = std::move(pApp);

		// Initialize application
		RegisterWindow(hInstance);
		if (!InitInstance(hInstance, nCmdShow))
		{
			return false;
		}

		// Main loop.
		MSG msg = {};
		bool running = true;
		while (running)
		{
			while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
				{
					running = false;
				}
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			if (s_pApp)
			{
				s_pApp->OnEngineUpdate();
				s_pApp->OnEngineRender();
				s_pContext->SwapBuffer();
			}
		}

		s_pApp->OnEngineExit();
		s_pContext->OnShutdown();

#ifdef _DEBUG
		FreeConsole();
#endif

		// Return this part of the WM_QUIT message to Windows.
		return static_cast<char>(msg.wParam);
	}

	void AppHandler::ToggleFullscreen()
	{
		SetFullscreen(!s_IsFullscreen);
	}

	void AppHandler::SetFullscreen(bool val)
	{
		if (val)
		{
			s_IsFullscreen = true;

			GetWindowPlacement(s_pContext->GetHWND(), &s_WPC);
			if (s_HWNDStyle == 0)
				s_HWNDStyle = GetWindowLong(s_pContext->GetHWND(), GWL_STYLE);
			if (s_HWNDStyleEx == 0)
				s_HWNDStyleEx = GetWindowLong(s_pContext->GetHWND(), GWL_EXSTYLE);

			LONG NewHWNDStyle = s_HWNDStyle;
			NewHWNDStyle &= ~WS_BORDER;
			NewHWNDStyle &= ~WS_DLGFRAME;
			NewHWNDStyle &= ~WS_THICKFRAME;

			LONG NewHWNDStyleEx = s_HWNDStyleEx;
			NewHWNDStyleEx &= ~WS_EX_WINDOWEDGE;

			SetWindowLong(s_pContext->GetHWND(), GWL_STYLE, NewHWNDStyle | WS_POPUP);
			SetWindowLong(s_pContext->GetHWND(), GWL_EXSTYLE, NewHWNDStyleEx | WS_EX_TOPMOST);
			ShowWindow(s_pContext->GetHWND(), SW_SHOWMAXIMIZED);
		}
		else
		{
			s_IsFullscreen = false;

			SetWindowLong(s_pContext->GetHWND(), GWL_STYLE, s_HWNDStyle);
			SetWindowLong(s_pContext->GetHWND(), GWL_EXSTYLE, s_HWNDStyleEx);
			ShowWindow(s_pContext->GetHWND(), SW_SHOWNORMAL);
			SetWindowPlacement(s_pContext->GetHWND(), &s_WPC);
		}
	}

	LRESULT CALLBACK AppHandler::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	ATOM AppHandler::RegisterWindow(HINSTANCE hInstance)
	{
		WNDCLASSEX wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = nullptr; // icon LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32DEMO));
		wcex.hCursor = LoadCursor(hInstance, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
		wcex.lpszMenuName = nullptr;
		wcex.lpszClassName = L"Tempest";
		wcex.hIconSm = nullptr; // smol icon LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

		return RegisterClassExW(&wcex);
	}

	bool AppHandler::InitInstance(HINSTANCE hInstance, int nCmdShow)
	{
		s_pContext = Context::Create();

		if (!s_pContext->OnInit(hInstance, nCmdShow, s_pApp))
			return false;

		// Initialize the application. OnInit is defined in each child-implementation of application.
		s_pApp->OnEngineInit();

		return true;
	}
}