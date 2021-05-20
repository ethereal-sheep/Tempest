#pragma once

#include "../Resource.h"
#include "lousy_string_1.h"
#include "lousy_string_2.h"

namespace Tempest
{
	void test1()
	{
		debug_resource mr("Test 1");
		std::cout << std::endl;
		std::cout << "1. Testing buffer overrun" << std::endl;
		lousy_string_1 hello{ "Hello", &mr };
	}

	void test2()
	{
		debug_resource mr("Test 1");
		std::cout << std::endl;
		std::cout << "1. Testing buffer overrun" << std::endl;
		lousy_string_1 hello{ "Hello", &mr };
	}

	void test3()
	{
		debug_resource mr("Test 3");
	}

	void test4()
	{
		debug_resource mr("Test 4");
	}
}