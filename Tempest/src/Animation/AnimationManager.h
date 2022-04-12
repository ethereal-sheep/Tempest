/**********************************************************************************
* \author		Linus Ng Hao Xuan (haoxuanlinus.ng@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#include "Util.h"
#include "Animator.h"
#include "Core.h"
#include "TMath.h"
#include "Util/thread_pool.h"
#include "Graphics/PBR/ModelPBR.h"

namespace Tempest
{
	class AnimationManager
	{
		tmap<uint32_t, Animator> m_Animators;
		tmap<std::string, std::shared_ptr<ModelPBR>> m_Animations;

	public:

		AnimationManager();
		void LoadAnimations();
		void LoadAnimation(const std::string& path);
		void AddAnimator(uint32_t id, Animator anim);					// Add Entity's Animator into Animation Manager
		void UpdateAnimations(float dt);								// Updates all the Animations 
		tvector<future_bool> AsyncUpdateAnimations(float dt);
		void ChangeAnimation(uint32_t id, std::string anim);		// Changes 
		bool CheckAnimation(uint32_t id, std::string name);
		bool CheckAnimator(uint32_t id);
		void ChangeDuration(uint32_t id, float duration);

		void PlayAnimation(uint32_t id, bool loop);
		void PauseAnimation(uint32_t id);
		void StopAnimation(uint32_t id);
		void SetSpeed(uint32_t id, float speed);
		void ForceChange(uint32_t id, std::string anim);

		bool isPaused(uint32_t id) const;
		bool isPlaying(uint32_t id) const;
		bool isStopped(uint32_t id) const;
		bool hasEnded(uint32_t id) const;

		tvector<glm::mat4> GetBoneMatrix(uint32_t id) { return m_Animators.at(id).GetFinalBoneMatrix(); }
	};
}

