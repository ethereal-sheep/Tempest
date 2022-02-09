/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include "Memory.h"
#include "ECS/ECS.h"
#include "Graphics/OpenGL/Camera.h"
#include "WindowManager.h"
#include "Actions/ActionHistory.h"
#include "Scripting/SRM.h"
#include "ECS/Scene.h"

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

		Instance(
			const tpath& project_path, 
			const string& _name, 
			const tpath& path, 
			MemoryStrategy strategy
		) :
			full_path{ project_path },
			name{ _name },
			root{ path },
			memory_object(strategy),

			//po(memory_object.get()),
			scene(),
			ecs(memory_object.get()),
			srm(memory_object.get()),

			action_history(memory_object.get()),
			window_manager(memory_object.get())
		{}

	public:
		virtual ~Instance() = 0 {}
		
		Instance(const tpath& project_path, MemoryStrategy strategy = {}) :
			Instance(
				project_path,
				get_filename_from_path(project_path),
				get_rootpath_from_path(project_path),
				strategy
			)
		{
			//scene.load(root);
			//ecs.load(root);
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
			internal_update(dt);
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
		const tpath& get_full_path() const { return full_path; }

		bool load_new_scene_by_path(const tpath& path);
		bool load_new_scene_by_name(const string& name);
		bool load_new_conflict_resolution_by_path(const tpath& path);
		bool load_new_conflict_resolution_by_int(int i);

		bool unload_current_conflict_resolution();
		bool unload_current_scene();

		tvector<tpair<int, tpath>> get_scene_paths();
		tvector<tpair<bool, tpath>> get_conflict_resolution_paths();


		const string& get_current_scene_name() const {
			return current_scene_name;
		}
		const string& get_current_res_name() const {
			return current_res_name;
		}

		auto get_current_res_index() const {
			return current_res_index;
		}

	private:

		void internal_init();
		void internal_update(float dt);
		void internal_render();
		void internal_exit();

		virtual void _init() = 0;
		virtual void _update(float dt) = 0;
		virtual void _render() = 0;
		virtual void _exit() = 0;

	protected:
		tpath full_path;
		string name;
		tpath root;

		MemoryObject memory_object;

		int current_res_index = 0;
		string current_scene_name = "";
		string current_res_name = "";
		
	public:
		//PhysicsObject po;
		Scene scene;
		ECS ecs;
		SRM srm;
		Camera cam;

		ActionHistory action_history;
		WindowManager window_manager;

		bool tutorial_enable = false;
		bool tutorial_slide = false;
		bool tutorial_temp_enable = true;
		bool tutorial_temp_exit = false;
		short tutorial_level = 1;
		Entity selected = INVALID;
	};
}