/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

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
	/**
	 * @brief Alias for std::thread
	 */
	using thread = std::thread;

	/**
	 * @brief Alias for std::mutex
	 */
	using mutex = std::mutex;

	/**
	 * @brief Alias for std::future
	 */
	template <typename T>
	using future = std::future<T>;

	/**
	 * @brief Alias for std::future<bool>
	 */
	using future_bool = future<bool>;

	/**
	 * @brief Thread Pool implementation. Allows for multithread capabilities.
	 * Tasks can be pushed or submitted to the pool
	 */
	class thread_pool
	{
		// type defs for nicer code
		using task_queue = std::queue<std::function<void()>>;
		using atom_bool = std::atomic_bool;
		using atom_ui32 = std::atomic_uint32_t;
	public:
		/**
		 * @brief Constructor. Constructs thread pool with set number of
		 * threads. If none is specified, implementation defaults to system's hardware
		 * concurrency, which is the recommended amount anyway.
		 * @param num_threads Number of threads to initialize.
		 */
		thread_pool(uint32_t num_threads = std::thread::hardware_concurrency()) :
			thread_count(num_threads ? num_threads : std::thread::hardware_concurrency()),
			threads(make_uptr<thread[]>(thread_count))
		{
			create_threads();
		}
		/**
		 * @brief Destructor. Before we destruct, we wait for all tasks to complete first.
		 */
		~thread_pool()
		{
			wait_for_tasks();
			running = false;
			destroy_threads();
		}

		/**
		 * @brief Total number of threads.
		 */
		uint32_t total_thread_count() const
		{
			return thread_count;
		}

		/**
		 * @brief Number of available threads.
		 */
		uint32_t available_thread_count() const
		{
			return thread_count - running_task_count();
		}

		/**
		 * @brief Number of queued tasks.
		 */
		uint32_t queued_task_count() const
		{
			const std::scoped_lock<Tempest::mutex> lock(q_mutex);
			return static_cast<uint32_t>(tasks.size());
		}

		/**
		 * @brief Number of tasks currently running.
		 */
		uint32_t running_task_count() const
		{
			return task_count - queued_task_count();
		}

		/**
		 * @brief Total tasks currently in the thread pool, including
		 * running and queued tasks.
		 */
		uint32_t total_task_count() const
		{
			return task_count;
		}

		/**
		 * @brief Pushes a task to the thread pool. Use if you don't care
		 * when the task is done or the return type.
		 * @tparam Func function type
		 * @tparam Args... variadic type for arguments
		 * @param task function ptr
		 * @param args... variadic params for function
		 */
		template <typename Func, typename... Args>
		void push_task(Func&& task, Args&&...args)
		{
			push_task([&task, &args...]
				{ std::forward<Func>(task)(std::forward<Args>(args)...); });
		}

		/**
		 * @brief Pushes a task to the thread pool. Use if you don't care
		 * when the task is done or the return type.
		 * @tparam Func function type
		 * @param task function ptr
		 */
		template <typename Func>
		void push_task(Func&& task)
		{
			// increment task count
			task_count++;
			{
				const std::scoped_lock lock(q_mutex);
				tasks.push(std::function<void()>(std::forward<Func>(task)));
			}
		}

		/**
		 * @brief Submits a task to the thread pool. Returns a future.
		 * @tparam Func function type
		 * @tparam Args... variadic type for arguments
		 * @param task function ptr
		 * @param args... variadic params for function
		 * @return future_bool to task promise
		 */
		template <typename Func, typename... Args,
			typename = std::enable_if_t<std::is_void_v<
			std::invoke_result_t<std::decay_t<Func>, std::decay_t<Args>...>>>>
		future_bool submit_task(Func task, Args ...args)
		{
			// make a promise and set it to true when the task has finished
			tsptr<std::promise<bool>> promise(make_sptr<std::promise<bool>>());
			push_task([task, args..., promise]
				{
					task(args...);
					promise->set_value(true);
				});
			return promise->get_future();
		}

		/**
		 * @brief Submits a task to the thread pool. Returns a future to the
		 * return value.
		 * @tparam Func function type
		 * @param task function ptr
		 * @return future<Ret> to task promise
		 */
		template <typename Func, typename... Args,
			typename Ret = std::invoke_result_t<std::decay_t<Func>, std::decay_t<Args>...>,
			typename = std::enable_if_t<!std::is_void_v<Ret>>>
		future<Ret> submit_task(Func task, Args ...args)
		{
			// make a promise and set it to true when the task has finished
			tsptr<std::promise<Ret>> promise(make_sptr<std::promise<Ret>>());
			push_task([task, args..., promise]
				{
					promise->set_value(task(args...));
				});
			// returns the future produced by promise
			return promise->get_future();
		}

		/**
		 * @brief Makes the calling thread wait for all tasks to finish before
		 * continuing.
		 */
		void wait_for_tasks()
		{
			while (true)
			{
				if (running_task_count() == 0)
					break;
				sleep_or_yield();
			}
		}

	private:

		/**
		 * @brief Creates number of threads based on thread count.
		 */
		void create_threads()
		{
			for (uint32_t i = 0; i < thread_count; i++)
				threads[i] = thread(&thread_pool::worker, this);
		}

		/**
		 * @brief Destroy all threads by joining them with calling
		 * thread (Destructor).
		 */
		void destroy_threads()
		{
			for (uint32_t i = 0; i < thread_count; i++)
				threads[i].join();
		}

		/**
		 * @brief Pops a task to a calling thread if it exists.
		 */
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

		/**
		 * @brief Sleeps or yields the thread based on sleep_duration
		 */
		void sleep_or_yield()
		{
			if (sleep_duration)
				std::this_thread::sleep_for(std::chrono::microseconds(sleep_duration));
			else
				std::this_thread::yield();
		}

		/**
		 * @brief Worker function to invoke on threads.
		 */
		void worker()
		{
			while (running)
			{
				std::function<void()> task;
				if (pop_task(task))
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

	private:
		tuptr<thread[]> threads;
		task_queue tasks;

		atom_ui32 task_count = 0;
		atom_bool running = true;
		mutable mutex q_mutex;
	};
}