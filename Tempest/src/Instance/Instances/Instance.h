#pragma once
#include "Memory.h"
#include "ECS/ECS.h"



namespace Tempest
{
	/**
	 * @brief Base abstract class of an instance. 
	 */
	class Instance
	{
	public:
		virtual ~Instance() = 0 {}
		Instance(MemoryStrategy strategy = {}) : 
			memory_object( strategy ), 
			ecs(memory_object.get()) {}

		Instance(const tpath& root_directory, MemoryStrategy strategy = {}) :
			Instance(strategy)
		{
			ecs.load(root_directory);
		}


		/**
		 * @brief Virtual hook for derived class. Add behaviour to the
		 * virtual function if the behavior desired is applied to all
		 * instances.
		 * 
		 * @warning Must always be called by derived class
		 * 
		 */
		virtual void OnInit();

		/**
		 * @brief Virtual hook for derived class. Add behaviour to the
		 * virtual function if the behavior desired is applied to all
		 * instances.
		 * 
		 * @warning Must always be called by derived class
		 * 
		 */
		virtual void OnUpdate(float dt);

		/**
		 * @brief Virtual hook for derived class. Add behaviour to the
		 * virtual function if the behavior desired is applied to all
		 * instances.
		 * 
		 * @warning Must always be called by derived class
		 * 
		 */
		virtual void OnRender();

		/**
		 * @brief Virtual hook for derived class. Add behaviour to the
		 * virtual function if the behavior desired is applied to all
		 * instances.
		 * 
		 * @warning Must always be called by derived class
		 * 
		 */
		virtual void OnExit();


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

	protected:
		ECS ecs;
	};
}