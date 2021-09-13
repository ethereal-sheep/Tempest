
#include "test_scripting.h"
#include "Scripting/SRM.h"

namespace Tempest
{
	void test_script()
	{
		auto test1 = CreateConstexprScript<std::tuple<int>(int, int)>(
			[](int x, int y) { return x + y; },
			std::placeholders::_1, std::placeholders::_2);

		auto test2 = CreateSequenceScript<3>();

		auto test3 = CreateDataScript<string>("string");

		auto test4 = CreateBranchScript<int(int), 2 + 1>(
			[](const auto& x) { return std::make_tuple(x); },
			std::placeholders::_1);

		auto test5 = CreateRuntimeScript<void(std::string)>(
			[](const auto& x) { LOG("{0}", x); },
			std::placeholders::_1);

		//auto test6 = CreateEventScript(
		//	[](const auto&) { LOG("Event Called"); });

		test5->set_input(0, test3->set_next(nullptr, 0), 0);
		(*test5)();
	}

	void testing_script_event_1()
	{
		SRM srm;
		Entity entity = 1;
		auto test = CreateEventScript(
			[entity](const Event& e)
			{
				auto a = event_cast<test_event_with_member>(e);
				LOG("Event Called: {0} called with {1}", entity, a.i);
			});
		srm.register_listener<test_event_with_member>(1, test.get());

		entity = 2;
		auto test2 = CreateEventScript(
			[entity](const Event& e)
			{
				if(auto* a = s_event_cast<test_event_with_member>(e))
					LOG("Event Called: {0} called with {1}", entity, a->i);
				else if(s_event_cast<test_event>(e))
					LOG("Event Called: ", entity);
			});

		srm.register_listener<test_event_with_member>(2, test2.get());
		srm.register_listener<test_event>(2, test2.get());

		try
		{
			srm.instant_dispatch_to_id<test_event>(1);
		}
		catch (const std::exception& a)
		{
			LOG_ERROR(a.what());
		}
		srm.instant_dispatch_to_id<test_event_with_member>(1, 2);

		srm.instant_dispatch_to_id<test_event>(2);
		srm.instant_dispatch_to_id<test_event_with_member>(2, 3);
		

		srm.instant_dispatch<test_event_with_member>(4);
	}

	void testing_script_event_2()
	{
	}

	void testing_script_event()
	{
		testing_script_event_1();
		//testing_script_event_2();
	}
}