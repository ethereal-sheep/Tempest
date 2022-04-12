/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include <chrono>

namespace Tempest
{
	/**
	 * @brief Timer implementation using clock.
	 */
	template <typename Clock = std::chrono::high_resolution_clock>
	class timer
	{
	public:
		using clock = Clock;
		using duration = typename Clock::duration;
		using rep = typename duration::rep;
		using time_point = typename clock::time_point;

		/**
		 * @brief Constructor.
		 */
		timer() :
			start_point(Clock::now())
		{}

		/**
		 * @brief Returns time elapsed since timer construction.
		 */
		template <typename Rep = rep, typename Units = duration>
		Rep elapsed() const
		{
			// fence to prevent compiler optimizations, i.e. make sure 
			// we observe the correct time when we call now.
			std::atomic_thread_fence(std::memory_order_relaxed);
			auto counted_time = std::chrono::duration_cast<Units>(Clock::now() - start_point).count();
			std::atomic_thread_fence(std::memory_order_relaxed);
			return static_cast<Rep>(counted_time);
		}

	private:
		const time_point start_point;
	};


	/**
	 * @brief Helper typedefs for chrono literals
	 */
	namespace time
	{
		using s = std::chrono::seconds;
		using ms = std::chrono::milliseconds;
		using mcs = std::chrono::microseconds;
		using ns = std::chrono::nanoseconds;
	}

}
