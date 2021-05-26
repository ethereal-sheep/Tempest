
#define _CRT_SECURE_NO_WARNINGS

#include "Util.h"
#include "Instance/Instance.h"

#include <iostream>
#include <thread>




using namespace std;
using namespace Tempest;




int main()
{
	// always call this first
	Logger::Init();
	LOG("Initializing Tempest");

	
	Instance i(MemoryStrategy{DebugFlag::DEBUG_STRICT});


}