#pragma once

#include "..\..\Math.h"
#include "..\Entity.h"

namespace Tempest::Components
{
	struct Transform
	{
		els::vec3 position;
		els::quat rotation;
		els::vec3 scale;
	};

	struct Meta
	{
		string name;
	};

	struct Relationship
	{
		Entity parent;
		EnVector children;
	};


}

namespace tc = Tempest::Components;