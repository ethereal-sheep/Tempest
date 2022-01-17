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
#include "Instance/RuntimeInstance.h"
#include "Instance/EditTimeInstance.h"
#include "Instance/NullTimeInstance.h"
#include "Instance/PhysicsInstance.h"

namespace Tempest
{

	enum struct InstanceType
	{
		NULL_TIME,
		EDIT_TIME,
		RUN_TIME,
		PHYSICS_TIME
	};

	enum struct InstanceState
	{
		LOAD,
		RUN,
	};

	struct InstanceConfig
	{
		InstanceConfig(
			const tpath& _path = tpath{}, 
			MemoryStrategy strategy = MemoryStrategy{ DebugFlag::NONE },
			InstanceType type = InstanceType::NULL_TIME,
			string mname = "",
			int conres = -1,
			std::vector<Entity> seqs = std::vector<Entity>())
			: 
			project_path(_path), 
			memory_strategy(strategy), 
			instance_type(type),
			map_name(mname),
			conflict_resolution(conres),
			sequences(seqs)
		{}

		tpath project_path;
		MemoryStrategy memory_strategy;
		InstanceType instance_type;
		string map_name;
		int conflict_resolution;
		std::vector<Entity> sequences;
	};

	struct LoadNewInstance : public Event
	{
		template<typename... TArgs>
		LoadNewInstance(TArgs... args) :
			config(std::forward<TArgs>(args)...) {}

		LoadNewInstance() {}

		InstanceConfig config;
	};

	struct LoadPrevInstance : public Event
	{
		LoadPrevInstance(MemoryStrategy strategy) :
			memory_strategy(strategy) {}


		MemoryStrategy memory_strategy;
	};

	struct CreateNewInstance : public Event
	{
		CreateNewInstance(const tpath& _path, const string& _name, bool _load) :
			path(_path), name(_name), load(_load) {}

		tpath path;
		string name;
		bool load;
	};
}