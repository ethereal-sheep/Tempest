#pragma once

#include <PxConfig.h>
#include <PxPhysicsAPI.h>

namespace Tempest
{
	const static size_t			platform_dependent_alignment = 16;			// byte-alignment for all memory allocs

	static const char*			pvd_host_ip			= "127.0.0.1";			// IP of the PC which is running PVD
	const static int			port				= 5425;					// TCP port to connect to, where PVD is listening
	const static unsigned int	timeout				= 100;					// timeout in milliseconds to wait for PVD to respond,


}