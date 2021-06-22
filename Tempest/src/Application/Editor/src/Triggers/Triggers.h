#pragma once
#include "Events/Events/IEvent.h"

namespace Tempest
{
	struct OverlayTrigger : public Event
	{
		OverlayTrigger(std::string a) : msg(a) {}
		std::string msg;
	};

	struct ErrorTrigger : public Event
	{
		ErrorTrigger(std::string a) : msg(a) {}
		std::string msg;
	};


}