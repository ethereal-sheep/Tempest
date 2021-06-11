
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
		unordered_var_set<string> v;
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
		unordered_var_set<string> v;
		for (int i = 0; i < 100; ++i)
		{
			v.create(std::to_string(i), i);
		}

		const unordered_var_set<string>& a = v;
		auto b = make_const_range(a);

		for (auto& [key, type, var] : b)
		{
			LOG("{0}, {1}, {2}", key, type, var.get<int>());
		}
	}
	void testing_data_6()
	{
		const char* s = R"(S:\Development\Projects)";
		tpath path(s);
		unordered_var_set<string> v;
		for (int i = 0; i < 100; ++i)
		{
			v.create(std::to_string(i), els::random::uniform_rand(0, 111));
		}
		v.serialize(path, "Unordered");
	}
	void testing_data_7()
	{
		const char* s = R"(S:\Development\Projects)";
		tpath path(s);
		ordered_var_set<string> v;
		for (int i = 0; i < 100; ++i)
		{
			v.create(std::to_string(i), els::random::uniform_rand(0, 111));
		}
		v.serialize(path, "Ordered");
	}
	void testing_data_8()
	{
		const char* s = R"(S:\Development\Projects)";
		string filename = "testing_data_8";
		tpath path(s);
		ordered_var_set<string> v;
		for (int i = 0; i < 100; ++i)
		{
			v.create(std::to_string(i), els::random::uniform_rand(0, 111));
		}

		v.serialize(path, filename);

		try
		{
			ordered_var_set<string> o{ path / (filename + ".json") };
			LOG_ASSERT(v.size());
			for (auto [key, type, var] : v)
			{
				LOG_ASSERT(o.count(key));
			}

		}
		catch (const std::exception& a)
		{
			LOG_ASSERT_V(false, "{}", a.what());
		}
		ordered_var_set<string> o;
		try
		{
			o.deserialize(path / (filename + ".json"));
		}
		catch (const std::exception& a)
		{
			LOG_ASSERT_V(false, "{}", a.what());
		}

		LOG_ASSERT(o.size());
		for (auto [key, type, var] : v)
		{
			LOG_ASSERT(o.count(key));
		}
	}

	void testing_data_9()
	{
		const char* s = R"(S:\Development\Projects)";
		string filename = "testing_data_9";
		tpath path(s);
		unordered_var_set<string> v;
		for (int i = 0; i < 100; ++i)
		{
			v.create(std::to_string(i), els::random::uniform_rand(0, 111));
		}

		v.serialize(path, filename);

		try
		{
			ordered_var_set<string> o{ path / (filename + ".json") };
			LOG_ASSERT(o.size());
			for (auto [key, type, var] : v)
			{
				LOG_ASSERT(o.count(key));
			}

		}
		catch (const std::exception& a)
		{
			LOG_ASSERT_V(false, "{}", a.what());
		}
		ordered_var_set<string> o;
		try
		{
			o.deserialize(path / (filename + ".json"));
		}
		catch (const std::exception& a)
		{
			LOG_ASSERT_V(false, "{}", a.what());
		}

		LOG_ASSERT(o.size());
		for (auto [key, type, var] : v)
		{
			LOG_ASSERT(o.count(key));
		}
	}
	void testing_data_10()
	{
		const char* s = R"(S:\Development\Projects)";
		string filename = "testing_data_10";
		tpath path(s);
		ordered_var_set<string> v;
		for (int i = 0; i < 100; ++i)
		{
			v.create(std::to_string(i), els::random::uniform_rand(0, 111));
		}

		v.serialize(path, filename);

		try
		{
			unordered_var_set<string> o{ path / (filename + ".json") };
			LOG_ASSERT(o.size());
			for (auto [key, type, var] : v)
			{
				LOG_ASSERT(o.count(key));
			}

		}
		catch (const std::exception& a)
		{
			LOG_ASSERT_V(false, "{}", a.what());
		}

		unordered_var_set<string> o;
		try
		{
			o.deserialize(path / (filename + ".json"));
		}
		catch (const std::exception& a)
		{
			LOG_ASSERT_V(false, "{}", a.what());
		}

		LOG_ASSERT(o.size());
		for (auto [key, type, var] : v)
		{
			LOG_ASSERT(o.count(key));
		}
	}

	void testing_data()
	{
		/*testing_data_1();
		testing_data_2();
		testing_data_3();
		testing_data_4();
		testing_data_5();
		testing_data_6();
		testing_data_7();*/
		testing_data_8();
		testing_data_9();
		testing_data_10();
	}
}