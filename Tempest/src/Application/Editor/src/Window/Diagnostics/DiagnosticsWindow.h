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

		void Runtime();
		void ShowFPSGraph();

		void ECSUsage(Instance& instance);

		void MemoryUsage(Instance& instance);
		void StatisticalData(Instance& instance);
		void AllocationHistory(Instance& instance);

		std::vector<float> fps_values;
		std::vector<float> alloc_values;
		std::vector<float> dealloc_values;


		const int graphsize = 100;
	};
}