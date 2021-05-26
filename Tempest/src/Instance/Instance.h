#pragma once
#include "Memory.h"
#include "ECS/ECS.h"



namespace Tempest
{
	/**
	 * @brief An instance to be run. 
	 */
	class Instance
	{

	public:
		Instance(MemoryStrategy strategy = {}) : 
			memory_object( strategy ), 
			ecs(memory_object.get())
		{

		}
		Instance(const tpath& root_directory, MemoryStrategy strategy = {}) :
			Instance(strategy)
		{
			ecs.load(root_directory);
		}
		~Instance()
		{

		}

		virtual void OnInit() {}
		virtual void OnUpdate() {}
		virtual void OnRender() {}
		virtual void OnExit() {}

		[[nodiscard]] const m_resource* get_debug() const
		{
			return memory_object.get_debug();
		}
		[[nodiscard]] bool has_debug() const
		{
			return memory_object.has_debug();
		}


	private:
		MemoryObject memory_object;
		ECS ecs;
	};
}