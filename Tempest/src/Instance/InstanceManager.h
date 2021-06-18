#pragma once
#include "RuntimeInstance.h"
#include "EditTimeInstance.h"

namespace Tempest
{
	enum struct InstanceType
	{
		EDIT_TIME,
		RUN_TIME,
	};

	class window
	{
		virtual void show(Instance& ) = 0;
	};

	class test_window : public window
	{
		void show(Instance& a) override
		{

		}
	};


	class InstanceConfig
	{
		MemoryStrategy memory_strategy;
		InstanceType instance_type = InstanceType::EDIT_TIME;
	};

	class InstanceManager
	{

		void update()
		{

		}
		void render()
		{

		}

		void load_new_instance()
		{

		}


	private:
		tuptr<Instance> current_instance;
	};
}