/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
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
		SimulateConflict(
			id_t a, id_t d, id_t aa, id_t da, id_t c, uint32_t f,
			std::atomic_uint32_t& w, std::atomic_uint32_t& l,
			std::atomic_int32_t& ak, std::atomic_int32_t& dk,
			std::atomic_bool& fin) :
			atker(a), defer(d), atk_act(aa), def_act(da), conflict(c), freq(f),
			win(w), lose(l),
			attack(ak), defend(dk),
			finish(fin) {}

		id_t atker;
		id_t defer;
		id_t atk_act;
		id_t def_act;
		id_t conflict;
		uint32_t freq;
		std::atomic_uint32_t& win;
		std::atomic_uint32_t& lose;
		std::atomic_int32_t& attack;
		std::atomic_int32_t& defend;
		std::atomic_bool& finish;
	};


}