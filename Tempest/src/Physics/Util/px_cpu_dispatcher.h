#pragma once
#include "px_include.h"
#include "px_make.h"
#include "Util/thread_pool.h"

namespace Tempest
{
	/**
	 * @brief Used for PhysX. Thread pool adapter for PxCpuDispatcher. Uses global
	 * thread_pool to schedule concurrent tasks.
	 */
	class px_cpu_dispatcher : public physx::PxCpuDispatcher
	{
	public:
		/**
		 * @brief Constructor. 
		 * @param tp Reference to global thread_pool.
		 */
		px_cpu_dispatcher(thread_pool& tp) : t_pool{ tp } {}

		/**
		 * @brief Explicitly called by PhysX. Schedules a task to thread_pool
		 * @param task physx BaseTask
		 */
		void submitTask(physx::PxBaseTask& task) override
		{
			auto f = std::bind(&physx::PxBaseTask::run, px_make(&task));
			t_pool.push_task(f);
		}
		/**
		 * @brief Gets number of available threads. Used by physx for optimizations.
		 */
		uint32_t getWorkerCount() const override
		{
			return t_pool.available_thread_count();
		}
	private:
		thread_pool& t_pool;
	};
	
}