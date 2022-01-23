#pragma once
#include "Util.h"
#include "TMath.h"

namespace Tempest
{
	struct Frame
	{
		glm::vec3 pos;
		glm::quat rot;
		glm::vec3 scale;
	};

	struct Animation
	{
		std::vector<std::vector<std::pair<float, Frame>>> mesh_frames;		// Vector of Different Meshes' Frames with their corresponding timestamps	
		float current_time = 0.f;
		float ticks = 0.f;
		float duration = 0.f;
	};
}