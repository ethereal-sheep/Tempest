#pragma once

#include "Core.h"
#include "Events/Events/IEvent.h"

namespace Tempest
{
	struct Simulate : public Event {};

	struct Input : public Event 
	{
		Input(id_t entity, int i = 0) : owner(entity), input(i) {}
		
		id_t owner;
		int input;
	};
}