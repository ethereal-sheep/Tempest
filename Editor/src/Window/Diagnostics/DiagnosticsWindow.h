/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include "Instance/Instance.h"
#include "Util/UIElements.h"
#include "Window/Util/AudioPlayer.h"

namespace Tempest
{
	class DiagnosticsWindow : public Window
	{

		const char* window_name() override
		{
			return ICON_FA_STETHOSCOPE " Diagnostics";
		}

		void init(Instance& instance) override;
		void show(Instance& instance) override;

		void Runtime(Instance& instance);
		void ShowFPSGraph();

		void ECSUsage(Instance& instance);

		void MemoryUsage(Instance& instance);
		void StatisticalData(Instance& instance);
		void AllocationHistory(Instance& instance);
		void Camera(Instance& instance);
		void Audio(Instance& instance);
		void Mouse(Instance& instance);
		void Light(Instance& instance);
		void Textures(Instance& instance);
		void Particles(Instance& instance);

		void Profiler();
		void ShowProfilingChart();

		std::vector<float> fps_values;
		std::vector<float> alloc_values;
		std::vector<float> dealloc_values;

		size_t m_CurrentFrame = 0;
		float m_BarHeight = 20.f;

		const int graphsize = 100;
		tsptr<Texture> curr_tex;
		AudioPlayer player;
	};
}