
#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <forward_list>
#include <cstring>
#include <memory>

#include "MemoryManager.h"
#include "test_mem_res.h"


//
//
//void abc()
//{
//	wrapper::Init();
//	//std::pmr::set_default_resource(std::pmr::new_delete_resource());
//    //mr.set_verbose(true);
//
//    // testing one allocation of lousystring1
//    try
//    {
//		std::cout << std::endl;
//		std::cout << "1. Testing buffer overrun" << std::endl;
//		lousyString1 hello{ "Hello", wrapper::mr};
//    }
//    catch(...)
//    {
//
//    }
//
//	// testing one allocation of lousystring2
//	try
//	{
//		std::cout << std::endl;
//		std::cout << "2. Testing no destructor" << std::endl;
//		lousyString2 hello{ "Hello", wrapper::mr };
//	}
//	catch (...)
//	{
//
//	}
//
//	// testing one allocation
//	try
//	{
//		std::cout << std::endl;
//		std::cout << "3. Testing pmr::vector: 1 allocation" << std::endl;
//		std::pmr::vector<std::string> hello{ "Hello" };
//	}
//	catch (...)
//	{
//
//	}
//
//	// testing three allocations
//	try
//	{
//		std::cout << std::endl;
//		std::cout << "4. Testing pmr::vector: 3 allocation" << std::endl;
//		std::pmr::vector<std::string> hello{ "Hello", "", "Hi" };
//	}
//	catch (...)
//	{
//
//	}
//
//	// unique pointer
//	try
//	{
//		std::cout << std::endl;
//		std::cout << "5. Testing unique ptr" << std::endl;
//		std::unique_ptr<std::string> hello = std::make_unique<std::string>("Hello");
//	}
//	catch (...)
//	{
//
//	}
//	// vector of unique ptr
//	try
//	{
//		std::cout << std::endl;
//		std::cout << "6. Testing unique ptr" << std::endl;
//		std::vector<std::unique_ptr<int>> hello{  };
//		hello.emplace_back(std::move(std::make_unique<int>(5)));
//
//		//hello[0] = nullptr;
//	}
//	catch (...)
//	{
//
//	}
//
//	//// testing static scoped omr vectors before and after mr
//	//try
//	//{
//	//	std::cout << std::endl;
//	//	std::cout << "*** Testing statically scoped pmr::vectors ***" << std::endl;
//	//	std::cout << "5. Declared before MR" << std::endl;
//	//	test1.push_back(1);
//	//	std::cout << "6. Declared after MR but not initialized with" << std::endl;
//	//	test2.push_back(2);
//	//	std::cout << "7. Declared after MR and initialized with" << std::endl;
//	//	test3.push_back(3);
//	//}
//	//catch (...)
//	//{
//
//	//}
//
//	// stress test
//	try
//	{
//		std::cout << std::endl;
//		std::cout << "5. Stress Testing std::vector: 10000 pushback" << std::endl;
//		std::vector<size_t> test;
//		const size_t BUFFER_SIZE = 10000;
//		for (size_t i = 0; i < BUFFER_SIZE; ++i)
//		{
//			test.push_back(i);
//		}
//	}
//	catch (...)
//	{
//
//	}
//
//    std::cout << "" << std::endl;
//    std::cout << "-------------------------------------------" << std::endl;
//    std::cout << "                SAMPLE DATA                " << std::endl;
//    std::cout << "-------------------------------------------" << std::endl;
//    std::cout << "Total bytes allocated:                " << wrapper::mr->bytes_allocated() << std::endl;
//    std::cout << "Total bytes deallocated:              " << wrapper::mr->bytes_deallocated() << std::endl;
//    std::cout << "Highest memory usage:                 " << wrapper::mr->bytes_highwater() << std::endl;
//    std::cout << "Outstanding Bytes:                    " << wrapper::mr->bytes_outstanding() << std::endl;
//    std::cout << "Outstanding Blocks:                   " << wrapper::mr->blocks_outstanding() << std::endl;
//    std::cout << "-------------------------------------------" << std::endl;
//    std::cout << "Last allocated address:               0x" << wrapper::mr->last_allocated_address() << std::endl;
//    std::cout << "Last allocated bytes:                 " << wrapper::mr->last_allocated_num_bytes() << std::endl;
//    std::cout << "Last allocated alignment:             " << wrapper::mr->last_allocated_alignment() << std::endl;
//    std::cout << "Last deallocated address:             0x" << wrapper::mr->last_deallocated_address() << std::endl;
//	std::cout << "Last deallocated bytes:               " << wrapper::mr->last_deallocated_num_bytes() << std::endl;
//	std::cout << "Last deallocated alignment:           " << wrapper::mr->last_deallocated_alignment() << std::endl;
//	std::cout << "-------------------------------------------" << std::endl;
//	std::cout << "Misc Errors:                          " << wrapper::mr->misc_errors() << std::endl;
//	std::cout << "Bad Deallocations:                    " << wrapper::mr->bad_deallocations() << std::endl;
//	std::cout << "-------------------------------------------" << std::endl;
//    std::cout << "" << std::endl;
//
//	wrapper::Destroy();
//}
//
//void test()
//{
//}
//
