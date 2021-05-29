
#define _CRT_SECURE_NO_WARNINGS
#include "Application/EntryPoint.h"
#include "Util.h"
#include "Instance/Instances/RuntimeInstance.h"
#include "Events/Test/event_test.h"

#include <iostream>
#include <thread>

using namespace std;
using namespace Tempest;

class Editor : public Application
{
public:

	Editor()
		: Application(1080, 960, L"Editor") {}

	void OnInit() override
	{
		Logger::Init();
		LOG("Initializing Tempest");
		const char* s = R"(S:\Development\Projects)";
		tpath path(s);

		RuntimeInstance i(path, MemoryStrategy{ DebugFlag::NONE });
		LOG("{}", i.has_debug());
	}

	void OnUpdate() override
	{

	}

	void OnRender() override
	{

	}

	void OnExit() override
	{

	}
};

std::unique_ptr<Tempest::Application> CreateApplication()
{
	return std::make_unique<Editor>();
}
//int main()
//{
//	// always call this first
//	
//
//}