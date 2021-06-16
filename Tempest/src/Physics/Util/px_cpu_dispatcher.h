#pragma once
#include "px_include.h"
#include "px_make.h"
#include "Util//thread_pool.h"

namespace Tempest
{
	class px_cpu_dispatcher : public physx::PxCpuDispatcher
	{
	public:
		px_cpu_dispatcher(thread_pool& tp) : t_pool{ tp } {}

		void submitTask(physx::PxBaseTask& task) override
		{
			auto f = std::bind(&physx::PxBaseTask::run, px_make(&task));
			t_pool.push_task(f);
		}
		uint32_t getWorkerCount() const override
		{
			return t_pool.available_thread_count();
		}
	private:
		thread_pool& t_pool;
	};
	
}