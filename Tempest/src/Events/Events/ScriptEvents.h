#pragma once

#include "Events/Events/IEvent.h"

namespace Tempest
{
	struct Simulate : public Event {};

	struct Input : public Event 
	{
		Input(int i = 0) : input(i) {}
		int input;
	};
}