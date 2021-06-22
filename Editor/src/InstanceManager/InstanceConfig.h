#pragma once
#include "Instance/RuntimeInstance.h"
#include "Instance/EditTimeInstance.h"
#include "Instance/NullTimeInstance.h"

namespace Tempest
{

	enum struct InstanceType
	{
		NULL_TIME,
		EDIT_TIME,
		RUN_TIME,
	};

	enum struct InstanceState
	{
		LOAD,
		RUN,
	};

	struct InstanceConfig
	{
		InstanceConfig(const tpath& _path = tpath{}, MemoryStrategy strategy = MemoryStrategy{}, InstanceType type = InstanceType::NULL_TIME)
			: project_path(_path), memory_strategy(strategy), instance_type(type) {}

		tpath project_path;
		MemoryStrategy memory_strategy;
		InstanceType instance_type;
	};

	struct LoadNewInstance : public Event
	{
		LoadNewInstance(const tpath& _path, MemoryStrategy strategy, InstanceType type) :
			config(_path, strategy, type) {}

		LoadNewInstance() {}

		InstanceConfig config;
	};
}