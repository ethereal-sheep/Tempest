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

		string get_filename_from_path(const tpath& path) const
		{
			if (!std::filesystem::exists(path))
				throw instance_exception("Instance: Bad Directory!");

			if (!path.has_stem())
				throw instance_exception("Instance: Bad Filename!");

			return path.stem().string();

		}
		tpath get_rootpath_from_path(const tpath& path) const
		{
			if (!std::filesystem::exists(path))
				throw instance_exception("Instance: Bad Directory!");

			return path.parent_path();
		}

		Instance(const string& _name, const tpath& path, MemoryStrategy strategy) :
			name{ _name },
			root{ path },
			memory_object(strategy),
			po(memory_object.get()),
			ecs(memory_object.get()),
			window_manager(memory_object.get()) {}

	public:
		virtual ~Instance() = 0 {}
		
		Instance(const tpath& project_path, MemoryStrategy strategy = {}) :
			Instance(
				get_filename_from_path(project_path),
				get_rootpath_from_path(project_path),
				strategy
			)
		{
			ecs.load(root);
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
		auto& register_window(Args&&... args)
		{
			return window_manager.register_window<TWindow>(std::forward<Args>(args)...);
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

		const string& get_name() const { return name; }
		const tpath& get_path() const { return root; }

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
		string name;
		tpath root;

		MemoryObject memory_object;
		PhysicsObject po;
	public:
		
		ECS ecs;
		WindowManager window_manager;
		Camera cam;

		
		Entity selected = INVALID;
	};
}