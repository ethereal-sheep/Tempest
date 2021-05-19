#pragma once

#include "..\..\TMath.h"
#include "..\Entity.h"

namespace Tempest::Components
{
	/*
	ALL COMPONENTS MUST BE DEFAULT AND COPY CONSTRUCTABLE
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
		string name;

		//Meta() = default;
		//Meta(string s) : name{s}{}
	};

	struct Relationship
	{
		Entity parent;
		EnVector children;
	};


}

namespace tc = Tempest::Components;