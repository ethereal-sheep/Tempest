#include "AnimationManager.h"

namespace Tempest
{
	void AnimationManager::PlayAnimation(uint32_t id, Animation& animation, float time, glm::mat4& anim_matrix)
	{
		if (animation_map.contains(id))
		{
			// Look up animation key frames, interpolate pos, rot, scale
			// Update anim_matrix

		}

		else
		{
			// Add animation into the map
			animation.start_time = time;
			animation_map.insert(std::make_pair(id, animation));
		}
	}

	glm::mat4 AnimationManager::InterpolatePosition(const Animation& animation, const float time)
	{

	}

	glm::mat4 AnimationManager::InterpolateRoatation(const Animation& animation, const float time)
	{

	}

	glm::mat4 AnimationManager::InterpolateScale(const Animation& animation, const float scale)
	{

	}
}