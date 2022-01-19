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
		std::vector<Frame> frames;
		std::vector<float> timestamps;
	};
}