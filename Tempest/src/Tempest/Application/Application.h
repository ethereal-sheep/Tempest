#pragma once
#include <string>

namespace Tempest
{
	class Application
	{
	public:
		Application(uint32_t width, uint32_t height, std::string name);

		void OnEngineInit();
		void OnEngineUpdate();
		void OnEngineRender();
		void OnEngineExit();

		virtual void OnInit() {};
		virtual void OnUpdate() {};
		virtual void OnRender() {};
		virtual void OnExit() {};

		uint32_t GetWidth() const { return m_width; }
		uint32_t GetHeight() const { return m_height; }
		float GetAspectRatio() const { return static_cast<float>(m_width) / static_cast<float>(m_height); }
		const char* GetTitle() const { return m_title.c_str(); }
		void Resize(uint32_t width, uint32_t height) 
		{
			m_width = width;
			m_height = height;
		};

	private:
		uint32_t m_width;
		uint32_t m_height;
		std::string m_title;

		// linus add here
	};


}
