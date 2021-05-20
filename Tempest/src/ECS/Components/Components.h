#pragma once

#include "..\..\TMath.h"
#include "..\Entity.h"

namespace Tempest::Components
{
	/*
	ALL COMPONENTS MUST BE DEFAULT, COPY, AND MOVE CONSTRUCTABLE
	(therefore please use ids instead of pointers)
	*/

	struct Transform
	{
		els::vec3 position;
		els::quat rotation;
		els::vec3 scale;
	};

	struct Meta
	{
		string name{};
	};

	struct Relationship
	{
		Entity parent{INVALID};
		EnVector children;
	};


}

namespace tc = Tempest::Components;