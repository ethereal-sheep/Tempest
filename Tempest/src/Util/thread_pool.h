#pragma once

#include "Core.h"
#include "type_traits.h"

#include <atomic>      
#include <chrono>      
#include <future>      
#include <mutex>         
#include <thread>    
#include <queue>

namespace Tempest
{
	using thread = std::thread;
	using mutex = std::mutex;

	template <typename T>
	using future = std::future<T>;


	class thread_pool
	{
		// type defs for nicer code
		using task_queue = std::queue<std::function<void()>>;
		using atom_bool = std::atomic_bool;
		using atom_ui32 = std::atomic_uint32_t;
	public:
		thread_pool(uint32_t num_threads = std::thread::hardware_concurrency()) :
			thread_count(num_threads ? num_threads : std::thread::hardware_concurrency()),
			threads(make_uptr<thread[]>(thread_count))
		{
			create_threads();
		}
		~thread_pool()
		{
			wait_for_tasks();
			running = false;
			destroy_threads();
		}

		uint32_t queued_task_count() const
		{
			const std::scoped_lock lock(q_mutex);
			return static_cast<uint32_t>(tasks.size());
		}

		uint32_t running_task_count() const
		{
			return task_count - queued_task_count();
		}

		uint32_t total_task_count() const
		{
			return task_count;
		}

		uint32_t total_thread_count() const
		{
			return thread_count;
		}

		template <typename Func, typename... Args>
		void push_task(Func task, Args&&...args)
		{
			push_task([task, &args...]
				{ task(std::forward<Args>(args)...); });
		}
		template <typename Func>
		void push_task(Func task)
		{
			task_count++;
			{
				const std::scoped_lock lock(q_mutex);
				tasks.push(std::function<void()>(task));
			}
		}

		template <typename Func, typename... Args, 
			typename = std::enable_if_t<std::is_void_v<std::invoke_result_t<std::decay_t<Func>, std::decay_t<Args>...>>>>
		std::future<bool> submit(Func&& task, Args &&...args)
		{
			tsptr<std::promise<bool>> promise(make_sptr<std::promise<bool>>());
			future<bool> future = promise->get_future();
			push_task([task, &args..., promise]
				{
					task(std::forward<Args>(args)...);
					promise->set_value(true);
				});
			return future;
		}
		template <typename Func, typename... Args,
			typename Ret = std::invoke_result_t<std::decay_t<Func>, std::decay_t<Args>...>, typename = std::enable_if_t<!std::is_void_v<Ret>>>
		future<Ret> submit(Func&& task, Args &&...args)
		{
			tsptr<std::promise<Ret>> promise(make_sptr<std::promise<Ret>>());
			future<Ret> future = promise->get_future();
			push_task([task, &args..., promise]
				{ promise->set_value(task(std::forward<Args>(args)...)); });
			return future;
		}

		void wait_for_tasks()
		{
			while (true)
			{
				if (!paused)
				{
					if (task_count == 0)
						break;
				}
				else
				{
					if (running_task_count() == 0)
						break;
				}
				sleep_or_yield();
			}
		}

	private:
		void create_threads()
		{
			for (uint32_t i = 0; i < thread_count; i++)
				threads[i] = thread(&thread_pool::worker, this);
		}

		void destroy_threads()
		{
			for (uint32_t i = 0; i < thread_count; i++)
				threads[i].join();
		}

		bool pop_task(std::function<void()>& task)
		{
			const std::scoped_lock lock(q_mutex);
			if (tasks.empty())
				return false;
			else
			{
				task = std::move(tasks.front());
				tasks.pop();
				return true;
			}
		}

		void sleep_or_yield()
		{
			if (sleep_duration)
				std::this_thread::sleep_for(std::chrono::microseconds(sleep_duration));
			else
				std::this_thread::yield();
		}

		void worker()
		{
			while (running)
			{
				std::function<void()> task;
				if (!paused && pop_task(task))
				{
					task();
					task_count--;
				}
				else
				{
					sleep_or_yield();
				}
			}
		}

	public:
		const uint32_t thread_count;
		uint32_t sleep_duration = 0;
		atom_bool paused = false;

	private:
		tuptr<thread[]> threads;
		task_queue tasks;

		atom_ui32 task_count = 0;
		atom_bool running = true;
		mutable mutex q_mutex;
	};
}