#pragma once
#include <string>

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
	 * @brief sample event to copy from
	 */
	template <typename T>
	struct TEvent : public Event
	{
		TEvent(T a) : i(a) {}
		T i;
	};

	/**
	 * @brief Use DEFINE_EVENT macro to define simple
	 * events that have no members
	 */
#define DEFINE_EVENT(x)		\
struct x : public Event {}	\



	class bad_event_cast : public std::exception
	{
	public:
		bad_event_cast(const string& err_msg = "Bad Event Cast!") : msg(err_msg) {}
		const char* what() const noexcept override { return msg.c_str(); }
	private:
		string msg;
	};

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
		catch (const std::bad_cast)
		{
			throw bad_event_cast(string("Bad event_cast!"));
		}
		catch (...)
		{
			throw bad_event_cast(string("Unknown exception from event_cast!"));
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
