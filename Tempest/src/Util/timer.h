#pragma once
#include <chrono>

namespace Tempest
{
	template <typename Clock = std::chrono::high_resolution_clock>
	class timer
	{
	public:
		using clock = Clock;
		using duration = typename Clock::duration;
		using rep = typename duration::rep;
		using time_point = typename clock::time_point;



		timer() :
			start_point(Clock::now())
		{}

		template <typename Rep = rep, typename Units = duration>
		Rep elapsed() const
		{
			std::atomic_thread_fence(std::memory_order_relaxed);
			auto counted_time = std::chrono::duration_cast<Units>(Clock::now() - start_point).count();
			std::atomic_thread_fence(std::memory_order_relaxed);
			return static_cast<Rep>(counted_time);
		}

	private:
		const time_point start_point;
	};

	namespace time
	{
		using s = std::chrono::seconds;
		using ms = std::chrono::milliseconds;
		using mcs = std::chrono::microseconds;
		using ns = std::chrono::nanoseconds;
	}

}
