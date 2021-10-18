/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#define PX_PHYSX_STATIC_LIB

#include <PxPhysicsAPI.h>
#pragma warning (disable : 4099) //Disable No debug info Warning for PhysX
namespace Tempest
{
	const static size_t			platform_dependent_alignment = 16;			// byte-alignment for all memory allocs

	static const char*			pvd_host_ip			= "127.0.0.1";			// IP of the PC which is running PVD
	const static int			port				= 5425;					// TCP port to connect to, where PVD is listening
	const static unsigned int	timeout				= 100;					// timeout in milliseconds to wait for PVD to respond,

}