/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#include "Events/Events/IEvent.h"


namespace Tempest
{
	class Instance;

	struct Clean : public Event 
	{
		Clean(Instance& in) : instance{ in } {}
		Instance& instance;
	};
	struct BuildAll : public Event 
	{
		BuildAll(Instance& in) : instance{ in } {}
		Instance& instance;
	};

	struct SimulateConflict : public Event 
	{
		SimulateConflict(id_t a, id_t d, id_t c, uint32_t f, uint32_t& w, uint32_t& l) :
			atk(a), def(d), conflict(c), freq(f), win(w), lose(l) {}
		id_t atk;
		id_t def;
		id_t conflict;
		uint32_t freq;
		uint32_t& win;
		uint32_t& lose;
	};


}