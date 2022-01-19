#pragma once
#include "Util.h"
#include "TMath.h""

namespace Tempest
{
	struct Frame
	{
		glm::vec3 pos;
		glm::vec4 rot;
		glm::vec3 scale;
	};

	struct Animation
	{
		std::vector<std::tuple<float, Frame>> mesh_frames;		// Vector of Different Meshes' Frames with their corresponding timestamps	
		float start_time = 0.f;
	};
}