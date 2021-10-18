/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include "Context.h"
#include <iostream>

namespace Tempest
{
	class AppHandler
	{
	public:
		static int Run(std::unique_ptr<Application>&& pApp, HINSTANCE hInstance, LPWSTR lpCmdList, int nCmdShow);
		static std::unique_ptr<Application>& GetApp() { return s_pApp; }
		static std::unique_ptr<Context>& GetContext() { return s_pContext; }

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
