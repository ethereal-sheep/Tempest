#pragma once
#include <string>
#include <memory>
#include <Windows.h>

namespace Tempest
{
	class Application
	{
	public:
		Application(uint32_t width, uint32_t height, std::wstring name);

		void OnEngineInit();
		void OnEngineUpdate();
		void OnEngineRender();
		void OnEngineExit();

		virtual void OnInit() {}
		virtual void OnUpdate() {}
		virtual void OnRender() {}
		virtual void OnExit() {}
		virtual LRESULT WndProcHandler(HWND, UINT, WPARAM, LPARAM) { return 0; }

		uint32_t GetWidth() const { return m_width; }
		uint32_t GetHeight() const { return m_height; }
		float GetAspectRatio() const { return static_cast<float>(m_width) / static_cast<float>(m_height); }
		const wchar_t* GetTitle() const { return m_title.c_str(); }
		void Resize(uint32_t width, uint32_t height) 
		{
			m_width = width;
			m_height = height;
		};

		/*
		*	I/O Stuff
		*/
		//virtual void OnLMouseDrag(POINT pos);
		//virtual void OnRMouseDrag(POINT pos);
		//virtual void OnLMousePress(uint8_t key, POINT pos);
		//virtual void OnLMouseRelease(uint8_t key, POINT pos);
		//virtual void OnRMousePress(uint8_t key, POINT pos);
		//virtual void OnRMouseRelease(uint8_t key, POINT pos);
		virtual void OnKeyPress(uint8_t key, uint8_t repeat);
		virtual void OnKeyRelease(uint8_t key);

	private:
		uint32_t m_width;
		uint32_t m_height;
		std::wstring m_title;

	};

	std::unique_ptr<Application> CreateApplication();
}
