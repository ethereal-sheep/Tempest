#pragma once
#include "Events/Events/IEvent.h"

namespace Tempest
{
	struct BottomRightOverlayTrigger : public Event
	{
		BottomRightOverlayTrigger(std::string a) : msg(a) {}
		std::string msg;
	};

	struct ErrorTrigger : public Event
	{
		ErrorTrigger(std::string a) : msg(a) {}
		std::string msg;
	};

	struct OpenFileTrigger : public Event {};

}