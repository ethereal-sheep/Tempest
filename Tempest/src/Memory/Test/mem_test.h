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
#include "../Resource.h"
#include "lousy_string_1.h"
#include "lousy_string_2.h"

namespace Tempest
{
	void output_debug_mr(const debug_resource* mr)
	{

		std::cout << "" << std::endl;
		std::cout << "-------------------------------------------" << std::endl;
		std::cout << "                " << mr->get_name() << std::endl;
		std::cout << "-------------------------------------------" << std::endl;
		std::cout << " Total bytes allocated:               " << mr->bytes_allocated() << std::endl;
		std::cout << " Total bytes deallocated:             " << mr->bytes_deallocated() << std::endl;
		std::cout << " Highest memory usage:                " << mr->bytes_highwater() << std::endl;
		std::cout << " Outstanding Bytes:                   " << mr->bytes_outstanding() << std::endl;
		std::cout << " Outstanding Blocks:                  " << mr->blocks_outstanding() << std::endl;
		std::cout << "-------------------------------------------" << std::endl;
		std::cout << " Last allocated address:              0x" << mr->last_allocated_address() << std::endl;
		std::cout << " Last allocated bytes:                "	<< mr->last_allocated_num_bytes() << std::endl;
		std::cout << " Last allocated alignment:            "	<< mr->last_allocated_alignment() << std::endl;
		std::cout << " Last deallocated address:            0x" << mr->last_deallocated_address() << std::endl;
		std::cout << " Last deallocated bytes:              "	<< mr->last_deallocated_num_bytes() << std::endl;
		std::cout << " Last deallocated alignment:          "	<< mr->last_deallocated_alignment() << std::endl;
		std::cout << "-------------------------------------------" << std::endl;
		std::cout << " Misc Errors:                         "	<< mr->misc_errors() << std::endl;
		std::cout << " Bad Deallocations:                   "	<< mr->bad_deallocations() << std::endl;
		std::cout << "-------------------------------------------" << std::endl;
	}

	void mem_test1()
	{
		debug_resource mr("Test 1");
		mr.set_verbose(true);
		LOG("1. Testing buffer overrun");
		lousy_string_1 hello{ "Hello", &mr };
	}

	void mem_test2()
	{
		debug_resource mr("Test 2");
		mr.set_verbose(true);
		LOG("2. Testing memory leak");
		lousy_string_2 hello{ "Hello", &mr };
	}

	void mem_test3()
	{
		debug_resource mr("Test 3");
	}

	void mem_test4()
	{
		debug_resource mr("Test 4");
	}


}