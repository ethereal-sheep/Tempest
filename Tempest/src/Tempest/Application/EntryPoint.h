#pragma once
#include <cstdlib>
#include <crtdbg.h>
#include <Windows.h>

#include "Application.h"
#include "AppHandler.h"

extern std::unique_ptr<Tempest::Application> Tempest::CreateApplication();

_Use_decl_annotations_
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	return Tempest::AppHandler::Run(Tempest::CreateApplication(), hInstance, lpCmdLine, nCmdShow);
}
