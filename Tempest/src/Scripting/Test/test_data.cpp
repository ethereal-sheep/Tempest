
#include "test_scripting.h"
#include "../Util/var_data.h"
#include "../Util/var_set.h"
#include "Util.h"

namespace Tempest
{
	void testing_data_1()
	{
		var_data v(1);
		LOG_ASSERT(v.get<int>() == 1);
		LOG_ASSERT(v.get_type() == pin_type::Int);
		LOG_ASSERT(v.get_type() != pin_type::Float);
		LOG_ASSERT(v.try_get<int>() == true);
		LOG_ASSERT(v.try_get<float>() == false);
		LOG_ASSERT(*(v.get_if<int>()) == 1);
		LOG_ASSERT(v.get_if<float>() == nullptr);
		
	}
	void testing_data_2()
	{
		var_data v(pin_type::Int);
		LOG_ASSERT(v.get<int>() == 0);
		LOG_ASSERT(v.get_type() == pin_type::Int);
		LOG_ASSERT(v.get_type() != pin_type::Float);
		LOG_ASSERT(v.try_get<int>() == true);
		LOG_ASSERT(v.try_get<float>() == false);
		LOG_ASSERT(*(v.get_if<int>()) == 0);
		LOG_ASSERT(v.get_if<float>() == nullptr);

	}
	void testing_data_3()
	{
		var_data v(1.);
		LOG_ASSERT(v.get_type() != pin_type::Int);
		LOG_ASSERT(v.get_type() == pin_type::Float);
		LOG_ASSERT(v.try_get<int>() != true);
		LOG_ASSERT(v.try_get<float>() != false);
		LOG_ASSERT(v.get_if<int>() == nullptr);
		LOG_ASSERT(v.get_if<float>() != nullptr);
	}
	void testing_data_4()
	{
		var_set<string> v;
		v.create("hello", 1);
		LOG_ASSERT(v.count("hello") == 1);
		LOG_ASSERT(v.count("bye") == 0);
		LOG_ASSERT(v.empty() == false);
		LOG_ASSERT(v.type_at("hello") == pin_type::Int);
		LOG_ASSERT(v.get_at<int>("hello") == 1);

		try
		{
			auto temp [[maybe_unused]] = v.get_at<float>("hello");
			LOG_ASSERT(false);
		}
		catch (const std::exception& s)
		{
			LOG(s.what());
		}

		try
		{
			auto temp [[maybe_unused]] = v.get_at<float>("bye");
			LOG_ASSERT(false);
		}
		catch (const std::exception& s)
		{
			LOG(s.what());
		}

		LOG_ASSERT(v.get_if_at<int>("hello") != nullptr);
		LOG_ASSERT(v.get_if_at<float>("hello") == nullptr);

		
	}
	void testing_data_5()
	{
		var_set<string> v;
		for (int i = 0; i < 100; ++i)
		{
			v.create(std::to_string(i), i);
		}

		const var_set<string>& a = v;
		auto b = make_const_range(a);

		for (auto& [key, type, var] : b)
		{
			LOG("{0}, {1}, {2}", key, type, var.get<int>());
		}
	}

	void testing_data()
	{
		testing_data_1();
		testing_data_2();
		testing_data_3();
		testing_data_4();
		testing_data_5();
	}
}