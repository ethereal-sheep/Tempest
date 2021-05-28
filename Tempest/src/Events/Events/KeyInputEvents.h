#pragma once

#include "Events/Events/IEvent.h"

namespace Tempest
{
	template<size_t CharCode>
	struct OnKeyDownEvent : public Event {};

	template<size_t CharCode>
	struct OnKeyReleaseEvent : public Event {};

	template<size_t CharCode>
	struct OnKeyHoldEvent : public Event {};

	struct OnAnyKeyDownEvent : public Event {};
	struct OnAnyKeyReleaseEvent : public Event {};
	struct OnAnyKeyHoldEvent : public Event {};
}