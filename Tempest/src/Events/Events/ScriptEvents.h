#pragma once

#include "Core.h"
#include "Events/Events/IEvent.h"

namespace Tempest
{
	struct Simulate : public Event 
	{

		Simulate(
			id_t a,
			id_t d,
			id_t as,
			id_t ds,
			id_t rs
		) :
			attacking(a),
			defending(d),
			atk_sys(as),
			def_sys(ds),
			res_sys(rs)
		{}


		id_t attacking;
		id_t defending;
		id_t atk_sys;
		id_t def_sys;
		id_t res_sys;
	};

	struct Input : public Event 
	{
		Input(id_t entity, int i = 0) : owner(entity), input(i) {}
		
		id_t owner;
		int input;
	};
}