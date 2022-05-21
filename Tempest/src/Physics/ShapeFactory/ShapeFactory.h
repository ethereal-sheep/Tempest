/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu),
				Lim Ziyi Jean(ziyijean.lim@digipen.edu),
*				Huang Xurong(h.xurong@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#include "TMath.h"
#include "Core.h"
#include "Memory.h"
#include "Physics/Util/px_include.h"
#include "Physics/Util/px_allocator.h"
#include "Physics/Util/px_malloc_allocator.h"
#include "Physics/Util/px_cpu_dispatcher.h"


namespace Tempest
{	
	//enum struct PHYSX_SHAPES
	//{
	//	SPHERE = 0,
	//	BOX,
	//	CAPSULE
	//};


	//class PxShapeFactory
	//{
	//	typedef std::pair<unsigned, tsptr<physx::PxShape>> shape_pair; // id, pointer to shape
	//private: 
	//	std::vector<shape_pair> shapes; 
	//	//physx::PxShape newShape = factory.getshape(9).second;
	//public:
	//	shape_pair CreateSphere(float radius);
	//	shape_pair CreateBox(float halfx, float halfy, float halfz);
	//	shape_pair CreateCapsule(float radius, float halfHeight);

	//	physx::PxShape GetGenericShape(PHYSX_SHAPES shape, float scale = 1.0f);
	//	void ReleaseAllShapes();
	//};

}