#include "AnimationManager.h"

namespace Tempest
{
	void AnimationManager::PlayAnimation(uint32_t id, Animation& animation, float time, std::vector<glm::mat4>& anim_matrix)
	{
		if (animation_map.contains(id))
		{
			// Look up animation key frames, interpolate pos, rot, scale
			// Update anim_matrix
			animation.current_time += animation.ticks * time;
			animation.current_time = fmod(animation.current_time, animation.duration);	// Modulo the time to figure out which time frame to lookup

			for (auto& i : animation.mesh_frames)
				anim_matrix.emplace_back(GetAnimationMatrix(i, animation.current_time));	// Loop through all the meshes and calculate the animation Matrix
		}

		else
		{
			// Add animation into the map
			animation.current_time += animation.ticks * time;
			animation_map.insert(std::make_pair(id, animation));

			for (auto& i : animation.mesh_frames)
				anim_matrix.emplace_back(GetAnimationMatrix(i, animation.current_time));
		}
	}

	glm::mat4 AnimationManager::GetAnimationMatrix(std::vector<std::pair<float, Frame>>& animation, const float time)
	{
		glm::mat4 trans = InterpolatePosition(animation, time);
		glm::mat4 rot = InterpolateRoatation(animation, time);
		glm::mat4 scale = InterpolateScale(animation, time);

		return trans * rot * scale;
	}

	glm::mat4 AnimationManager::InterpolatePosition(std::vector<std::pair<float, Frame>>& animation, const float time)
	{
		if (animation.size() == 1)
			return glm::translate(glm::mat4(1.f), animation[0].second.pos);
		
		uint32_t index0 = 0;
		uint32_t index1 = 0;
		for (uint32_t i = 0; i < static_cast<uint32_t>(animation.size()); ++i)
		{
			if (time < animation[i + 1].first)
				index0 = i;
		}

		index1 = index0 + 1;
		float factor = GetLinearFactor(animation[index0].first, animation[index1].first, time);

		glm::vec3 finalPos = glm::mix(animation[index0].second.pos, animation[index1].second.pos, factor);

		return glm::translate(glm::mat4(1.f), finalPos);
	}

	glm::mat4 AnimationManager::InterpolateRoatation(std::vector<std::pair<float, Frame>>& animation, const float time)
	{
		if (animation.size() == 1)
			return glm::toMat4(animation[0].second.rot);

		uint32_t index0 = 0;
		uint32_t index1 = 0;
		for (uint32_t i = 0; i < static_cast<uint32_t>(animation.size()); ++i)
		{
			if (time < animation[i + 1].first)
				index0 = i;
		}

		index1 = index0 + 1;
		float factor = GetLinearFactor(animation[index0].first, animation[index1].first, time);

		glm::quat finalRot = glm::slerp(animation[index0].second.rot, animation[index1].second.rot, factor);
		finalRot = glm::normalize(finalRot);

		return glm::toMat4(finalRot);
	}

	glm::mat4 AnimationManager::InterpolateScale(std::vector<std::pair<float, Frame>>& animation, const float time)
	{
		if (animation.size() == 1)
			return glm::scale(glm::mat4(1.f), animation[0].second.scale);

		uint32_t index0 = 0;
		uint32_t index1 = 0;
		for (uint32_t i = 0; i < static_cast<uint32_t>(animation.size()); ++i)
		{
			if (time < animation[i + 1].first)
				index0 = i;
		}

		index1 = index0 + 1;

		float factor = GetLinearFactor(animation[index0].first, animation[index1].first, time);

		glm::vec3 finalPos = glm::mix(animation[index0].second.scale, animation[index1].second.scale, factor);

		return glm::scale(glm::mat4(1.f), finalPos);
	}

	float AnimationManager::GetLinearFactor(const float t0, const float t1, const float time)
	{
		float factor = 0.f;
		float mid = time - t0;
		float diff = t1 - t0;
		factor = mid / diff;

		return factor;
	}
}