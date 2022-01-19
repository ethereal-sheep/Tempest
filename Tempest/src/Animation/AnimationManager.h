#pragma once

#include "Animation.h"

namespace Tempest
{
	class AnimationManager
	{
		std::unordered_map<uint32_t, Animation> animation_map;	// Map - id of entity to an Animation

	public:

		void PlayAnimation(uint32_t id, Animation& animation, float time, glm::mat4& anim_matrix);

	private:

		glm::mat4 InterpolatePosition(const Animation& animation, const float time);
		glm::mat4 InterpolateRoatation(const Animation& animation, const float time);
		glm::mat4 InterpolateScale(const Animation& animation, const float scale);


	};
}