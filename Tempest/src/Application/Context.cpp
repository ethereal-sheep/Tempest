#include "Context.h"
#pragma warning( disable : 4706)

using wglprocCreateContextAttribsARB = HGLRC(*)(HDC, HGLRC, const int*);
using wglprocChoosePixelFormatARB = BOOL(*)(HDC, const int*, const FLOAT*, UINT, int*, UINT*);

namespace Tempest
{
	std::unique_ptr<Context> Context::Create()
	{
		return std::make_unique<Context>();
	}

	bool Context::OnInit(HINSTANCE hInstance, int nCmdShow, const std::unique_ptr<Application>& pApp)
	{
		if (!Create_Window(hInstance, pApp))	// Check for successful Window Creation
			return false;

		if (!wglGetCurrentContext())			// Check for Proper Context Creation
			return false;

		if (!glewInit())						// Check for GLEW Initialization
			return false;

		SetWindowLong(m_WND, GWL_STYLE, GetWindowLong(m_WND, GWL_STYLE));

		ShowWindow(m_WND, nCmdShow);

		return true;
	}

	void Context::OnShutdown()
	{
		wglMakeCurrent(nullptr, nullptr);
		wglDeleteContext(m_RC);
		ReleaseDC(m_WND, m_HDC);
		UnregisterClass(L"Tempest", GetModuleHandle(nullptr));
	}

	void Context::SwapBuffer()
	{
		SwapBuffers(m_HDC);
	}

	bool Context::Create_Window(HINSTANCE hInstance, const std::unique_ptr<Application>& pApp)
	{
		RECT windowRect = { 0, 0, static_cast<LONG>(pApp->GetWidth()), static_cast<LONG>(pApp->GetHeight()) };
		AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

		/* Create a temporary context to get address of wgl extensions. */
		const HWND hTempWnd = CreateWindowW(L"Tempest",
											pApp->GetTitle(),					// Title
											WS_OVERLAPPEDWINDOW,				// Style
											CW_USEDEFAULT,						// Default Config Position
											CW_USEDEFAULT,						// Default Config Position
											windowRect.right - windowRect.left,	// Window Width
											windowRect.bottom - windowRect.top,	// Window Height
											nullptr, 							// Parent Window
											nullptr, 							// Menu
											hInstance,							// Instance
											pApp.get());

		if (!hTempWnd)		// Check for successful window creation
			return false;

		const HDC hTempDC = GetDC(hTempWnd);
		if (!hTempDC)		// Check for temp device context
		{
			DestroyWindow(hTempWnd);
			return false;
		}

		PIXELFORMATDESCRIPTOR pfd;
		memset(&pfd, 0, sizeof(pfd));
		pfd.nSize = sizeof(pfd);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.cColorBits = 24;
		pfd.cDepthBits = 24;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.iLayerType = PFD_MAIN_PLANE;

		int iPF;
		HGLRC hTempRC;

		if ((!(iPF = ChoosePixelFormat(hTempDC, &pfd)) || !SetPixelFormat(hTempDC, iPF, &pfd)) ||
			(!(hTempRC = wglCreateContext(hTempDC)) || !wglMakeCurrent(hTempDC, hTempRC)))
		{
			ReleaseDC(hTempWnd, hTempDC);
			DestroyWindow(hTempWnd);
			return false;
		}

		const auto wglChoosePixelFormatARB = wglprocChoosePixelFormatARB(wglGetProcAddress("wglChoosePixelFormatARB"));
		const auto wglCreateContextAttribsARB = wglprocCreateContextAttribsARB(wglGetProcAddress("wglCreateContextAttribsARB"));

		if (wglChoosePixelFormatARB && wglCreateContextAttribsARB)
		{
			m_WND = CreateWindowW(L"Tempest",
									pApp->GetTitle(),
									WS_OVERLAPPEDWINDOW,
									CW_USEDEFAULT,
									CW_USEDEFAULT,
									windowRect.right - windowRect.left,
									windowRect.bottom - windowRect.top,
									nullptr, // no parent window.
									nullptr, // no menus.
									hInstance,
									pApp.get());

			if (!m_WND)
			{
				wglMakeCurrent(nullptr, nullptr);
				wglDeleteContext(hTempRC);
				ReleaseDC(hTempWnd, hTempDC);
				DestroyWindow(hTempWnd);

				return false;
			}

			m_HDC = GetDC(m_WND);
			if (!m_HDC)
			{
				DestroyWindow(m_WND);

				wglMakeCurrent(nullptr, nullptr);
				wglDeleteContext(hTempRC);
				ReleaseDC(hTempWnd, hTempDC);
				DestroyWindow(hTempWnd);

				return false;
			}

			int attribs[] = {
				WGL_DRAW_TO_WINDOW_ARB, TRUE,
				WGL_DOUBLE_BUFFER_ARB, TRUE,
				WGL_SUPPORT_OPENGL_ARB, TRUE,
				WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
				WGL_COLOR_BITS_ARB, 24,
				WGL_RED_BITS_ARB, 8,
				WGL_GREEN_BITS_ARB, 8,
				WGL_BLUE_BITS_ARB, 8,
				WGL_DEPTH_BITS_ARB, 24,
				WGL_STENCIL_BITS_ARB, 8,
				0, 0
			};
			uint32_t num_formats_choosen;
			const bool choose_pf_success = wglChoosePixelFormatARB(m_HDC,
				attribs,
				nullptr,
				1,
				&iPF,
				&num_formats_choosen);

			DescribePixelFormat(m_HDC, iPF, sizeof(pfd), &pfd);

			if (!(choose_pf_success &&
				num_formats_choosen >= 1 &&
				SetPixelFormat(m_HDC, iPF, &pfd)))
			{
				ReleaseDC(m_WND, m_HDC);
				DestroyWindow(m_WND);

				wglMakeCurrent(nullptr, nullptr);
				wglDeleteContext(hTempRC);
				ReleaseDC(hTempWnd, hTempDC);
				DestroyWindow(hTempWnd);

				return false;
			}

			/* request an OpenGL 4.6 profile */
			int context_attribs[] = {
				WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
				WGL_CONTEXT_MINOR_VERSION_ARB, 6,
				WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
				0
			};

			m_RC = wglCreateContextAttribsARB(m_HDC, nullptr, context_attribs);
			if (!m_RC)
			{
				ReleaseDC(m_WND, m_HDC);
				DestroyWindow(m_WND);

				wglMakeCurrent(nullptr, nullptr);
				wglDeleteContext(hTempRC);
				ReleaseDC(hTempWnd, hTempDC);
				DestroyWindow(hTempWnd);

				return false;
			}

			/* delete the temporaries */
			wglMakeCurrent(nullptr, nullptr);
			wglDeleteContext(hTempRC);
			ReleaseDC(hTempWnd, hTempDC);
			DestroyWindow(hTempWnd);

			wglMakeCurrent(m_HDC, m_RC);
		}
		else
		{
			//CDN_CORE_CRITICAL("extended pixel format and context aattributes not supported!");
			m_WND = hTempWnd;
			m_HDC = hTempDC;
			m_RC = hTempRC;
		}

		return true;
	}
}