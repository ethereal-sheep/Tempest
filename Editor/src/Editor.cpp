
#include "Core.h"
#include "Tracy.hpp"

#include "Application/EntryPoint.h"

#include <iostream>
#include <thread>


namespace Tempest
{
	class Editor : public Application
	{
	public:

		Editor()
			: Application(1080, 960, L"Editor") {}

		void OnInit() override
		{
		}

		void OnUpdate() override
		{
			//do
			//{
			//
			//} while (true);
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
}

//int main()
//{
//	// always call this first
//	
//
//}