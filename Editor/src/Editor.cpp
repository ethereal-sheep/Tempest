
#define _CRT_SECURE_NO_WARNINGS

#include "Util.h"
#include "Instance/Instances/RuntimeInstance.h"

#include <iostream>
#include <thread>

using namespace std;
using namespace Tempest;


int main()
{
	// always call this first
	Logger::Init();
	LOG("Initializing Tempest");
	const char* s = R"(S:\Development\Projects)";
	tpath path(s);
	
	RuntimeInstance i(path, MemoryStrategy{DebugFlag::DEBUG_VERBOSE});
	LOG("{}", i.has_debug());


}