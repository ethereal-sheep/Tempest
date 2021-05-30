#pragma once

namespace Tempest
{
	struct IEvent
	{
		virtual ~IEvent() = 0 {};
	};
	using Event = IEvent;

	/**
	 * @brief sample event to copy from
	 */
	struct test_event : public Event
	{
		// members here
	};
	/**
	 * @brief sample event to copy from
	 */
	struct test_event_with_member : public Event
	{

		test_event_with_member(int a) : i(a) {}

		int i;
	};

	/**
	 * @brief Use DEFINE_EVENT macro to define simple
	 * events that have no members
	 */
#define DEFINE_EVENT(x)		\
struct x : public Event {}	\

	 /**
	  * @brief Casts the event to the correct type.
	  * Use only if guaranteed correct casting.
	  */
	template <typename T>
	inline const T& event_cast(const Event& e)
	{
		try
		{
			return dynamic_cast<const T&>(e);
		}
		catch (...)
		{
			LOG_CRITICAL("Bad Event Cast");
			//suppress warning
			return dynamic_cast<const T&>(e);
		}
	}
	/**
	 * @brief Casts the event to the correct type ptr.
	 * Returns nullptr if bad cast.
	 */
	template <typename T>
	inline const T* s_event_cast(const Event& e)
	{
		return dynamic_cast<const T*>(&e);
	}

}
