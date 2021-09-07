#pragma once
#include "Instance/Instance.h"
#include "Util/UIElements.h"

namespace Tempest
{
	class DiagnosticsWindow : public Window
	{

		const char* window_name() override
		{
			return ICON_FA_STETHOSCOPE " Diagnostics";
		}

		void show(Instance& instance) override;

		void Runtime(Instance& instance);
		void ShowFPSGraph();

		void ECSUsage(Instance& instance);

		void MemoryUsage(Instance& instance);
		void StatisticalData(Instance& instance);
		void AllocationHistory(Instance& instance);
		void Camera(Instance& instance);
		void Mouse(Instance& instance);
		void Light(Instance& instance);

		std::vector<float> fps_values;
		std::vector<float> alloc_values;
		std::vector<float> dealloc_values;


		const int graphsize = 100;
	};
}