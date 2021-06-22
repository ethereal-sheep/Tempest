#pragma once
#include "Memory.h"
#include "ECS/ECS.h"
#include "Graphics/OpenGL/Camera.h"
#include "WindowManager.h"

namespace Tempest
{

	/**
	 * @brief SRM_exception.
	 */
	class instance_exception : public std::exception
	{
	public:
		instance_exception(const string& err_msg = "Instance exception thrown!") : msg(err_msg) {}
		const char* what() const noexcept override { return msg.c_str(); }
	private:
		string msg;
	};

	/**
	 * @brief Base abstract class of an instance. 
	 */
	class Instance
	{
	protected:
		Instance(MemoryStrategy strategy = {}) :
			memory_object(strategy),
			ecs(memory_object.get()),
			window_manager(memory_object.get()) {}

	public:
		virtual ~Instance() = 0 {}

		Instance(const tpath& root_directory, MemoryStrategy strategy = {}) :
			Instance(strategy)
		{
			if (!std::filesystem::exists(root_directory))
				throw instance_exception("Instance: Bad Directory!");

			ecs.load(root_directory);
		}

		[[nodiscard]] const debug_mr* get_debug() const
		{
			return memory_object.get_debug();
		}

		[[nodiscard]] bool has_debug() const
		{
			return memory_object.has_debug();
		}

		template <typename TWindow, typename... Args>
		void register_window(Args&&... args)
		{
			window_manager.register_window<TWindow>(std::forward<Args>(args)...);
		}

		template <typename TWindow, typename... Args>
		void register_always(Args&&... args)
		{
			window_manager.register_always<TWindow>(std::forward<Args>(args)...);
		}

		/**
		 * @brief Virtual hook for derived class. Add behaviour to the
		 * virtual function if the behavior desired is applied to all
		 * instances.
		 */
		void OnInit()
		{
			internal_init();
			_init();
		}

		/**
		 * @brief Virtual hook for derived class. Add behaviour to the
		 * virtual function if the behavior desired is applied to all
		 * instances.
		 */
		virtual void OnUpdate(float dt)
		{
			internal_update();
			_update(dt);
		}

		/**
		 * @brief Virtual hook for derived class. Add behaviour to the
		 * virtual function if the behavior desired is applied to all
		 * instances.
		 */
		virtual void OnRender()
		{
			internal_render();
			_render();
		}

		/**
		 * @brief Virtual hook for derived class. Add behaviour to the
		 * virtual function if the behavior desired is applied to all
		 * instances.
		 */
		virtual void OnExit()
		{
			internal_exit();
			_exit();
		}

	private:

		void internal_init();
		void internal_update();
		void internal_render();
		void internal_exit();

		virtual void _init() = 0;
		virtual void _update(float dt) = 0;
		virtual void _render() = 0;
		virtual void _exit() = 0;

	protected:
		tpath root;
		MemoryObject memory_object;

	public:
		ECS ecs;
		WindowManager window_manager;
		Camera cam;

		
		Entity selected = INVALID;
	};
}