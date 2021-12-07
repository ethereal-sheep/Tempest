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
#include "Util.h"
#include "../EventManager.h"

namespace Tempest
{
	static void doo(const Event&);
	static void loo(const Event&);
	static void koo(int, const Event&);

	struct testing_event_callable
	{
		testing_event_callable(EventManager& em)
		{
			// set i = 1

			// using lambda
			em.register_listener<test_event>(
				[=](const Event&)
				{
					LOG("lambda callable: {}", i);
				}
			);

			// using std bind
			auto binded1 = std::bind(&testing_event_callable::foo, this, std::placeholders::_1);
			em.register_listener<test_event>(binded1);

			// member function pointer
			em.register_listener<test_event>(&testing_event_callable::goo, this);

			// i is captured by copy
			em.register_listener<test_event>(&testing_event_callable::boo, this, i);

			// cannot be initialized as no Event param
			//em.register_listener<test_event>(&testing_event_callable::too);

			// static function
			em.register_listener<test_event>(doo);
			// global function
			em.register_listener<test_event>(loo);
			// global fn with param
			em.register_listener<test_event>(koo, i);

			i = 1;

			em.instant_dispatch<test_event>();
		}

		void foo([[maybe_unused]] const Event& e)
		{
			LOG("binded member fn callable: {}", i);
		}
		void goo([[maybe_unused]] const Event& e)
		{
			LOG("member fn callable: {}", i);
		}
		void boo(int a, [[maybe_unused]] const Event& e)
		{
			LOG("member fn with param callable: {}", a);
		}
		void too()
		{
			LOG("member with no Event param cannot be called!");
		}

		int i = 0;
	};

	static void doo([[maybe_unused]] const Event& e)
	{
		LOG("static fn callable");
	}
	void loo([[maybe_unused]] const Event& e)
	{
		LOG("global fn callable");
	}
	void koo(int i, [[maybe_unused]] const Event& e)
	{
		LOG("fn with param callable: {}", i);
	}

	void event_test_1()
	{
		EventManager em;
		testing_event_callable testing(em);

		em.instant_dispatch<test_event>();
	}

	void hoo([[maybe_unused]] const Event& e)
	{
		auto a = event_cast<test_event_with_member>(e);
		LOG("event with param: {}", a.i);
	}

	void event_test_2()
	{
		EventManager em;
		em.register_listener<test_event>(hoo);

		em.instant_dispatch<test_event>();
	}
}