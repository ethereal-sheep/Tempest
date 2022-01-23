#pragma once

#include "Animation.h"

namespace Tempest
{
	class AnimationManager
	{
		std::unordered_map<uint32_t, Animation> animation_map;	// Map - id of entity to an Animation

	public:

		void PlayAnimation(uint32_t id, Animation& animation, float time, std::vector<glm::mat4>& anim_matrix);

	private:

		glm::mat4 GetAnimationMatrix(std::vector<std::pair<float, Frame>>& animation, const float time);
		glm::mat4 InterpolatePosition(std::vector<std::pair<float, Frame>>& animation, const float time);
		glm::mat4 InterpolateRoatation(std::vector<std::pair<float, Frame>>& animation, const float time);
		glm::mat4 InterpolateScale(std::vector<std::pair<float, Frame>>& animation, const float time);
		float GetLinearFactor(const float t0, const float t1, const float time);

	};
}