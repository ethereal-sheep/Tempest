#pragma once
#include "Core.h"
#include "Events.h"
#include "Memory.h"

namespace Tempest
{
	using EventCallback = std::function<void(const Event&)>;
	class EventManager
	{
		using Listeners = tmap<size_t, tvector<EventCallback>>;
	public:
		template <typename TEvent, typename TFunc, typename... TArgs>
		void register_listener(TFunc&& func, TArgs&&... args)
		{
			static_assert(std::is_base_of_v<Event, TEvent>, "Must inherit from Event");
			static_assert(!std::is_same_v<Event, TEvent>, "Event type cannot be IEvent");

			// if error C2440 is thrown here, your callback function is bad
			// see IEvent.h
			EventCallback callback = std::bind(func, args..., std::placeholders::_1);

			LOG("Registered listener of Event {}", typeid(TEvent).name());

			listeners[t_hash<TEvent>()].push_back(std::move(callback));
		}

		template <typename TEvent, typename... TArgs>
		void instant_dispatch(TArgs&&... args)
		{
			TEvent new_event{ std::forward<TArgs>(args)... };

			for (auto& fn : listeners[t_hash<TEvent>()])
				fn(new_event);
		}

		template <typename TEvent, typename... TArgs>
		void queue_dispatch(TArgs&&... args)
		{

		}

		void dispatch_all_in_queue()
		{

		}

		void clear()
		{
			listeners.clear();
		}

	private:
		Listeners listeners;
	};
}