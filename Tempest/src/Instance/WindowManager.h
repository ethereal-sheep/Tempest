#pragma once
#include "Core.h"
#include "Window.h"
#include "Util/range.h"

namespace Tempest
{
	class WindowManager
	{
		using Windows = tvector<tuptr<Window>>;
	public:
		WindowManager(m_resource* mem_res = std::pmr::get_default_resource()) : 
			windows(mem_res) {}

		template <typename TWindow, typename... Args>
		void register_window(Args&&... args)
		{
			static_assert(std::is_base_of_v<Window, TWindow>, "Must inherit from IWindow!");
			static_assert(!std::is_same_v<Window, TWindow>, "Window type cannot be IWindow!");

			windows.emplace_back(make_uptr<TWindow>(std::forward<Args>(args)...));
			// not sure if exception will be thrown here
		}

		template <typename TWindow, typename... Args>
		void register_always(Args&&... args)
		{
			static_assert(std::is_base_of_v<Window, TWindow>, "Must inherit from IWindow!");
			static_assert(!std::is_same_v<Window, TWindow>, "Window type cannot be IWindow!");

			always.emplace_back(make_uptr<TWindow>(std::forward<Args>(args)...));
			// not sure if exception will be thrown here
		}

		void init()
		{
			for (auto& window : always)
				window->init();
			for (auto& window : windows)
				window->init();
		}

		void exit()
		{
			for (auto& window : always)
				window->exit();
			for (auto& window : windows)
				window->exit();
		}

		void show(Instance& instance)
		{

			for (auto& window : windows)
			{
				if(window->visible)
					window->show(instance);
			}
			for (auto& window : always)
				window->show(instance);
		}

		auto get_windows()
		{
			return make_range(windows);
		}

	private:
		Windows always;
		Windows windows;
	};
}