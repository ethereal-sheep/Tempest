#pragma once
#include "Util.h"
#include "Animator.h"
#include "Core.h"
#include "TMath.h"

namespace Tempest
{
	class AnimationManager
	{
		tmap<uint32_t, Animator*> m_Animators;

	public:

		void AddAnimator(uint32_t id, Animator* anim);					// Add Entity's Animator into Animation Manager
		void UpdateAnimations(float dt);								// Updates all the Animations 
		void ChangeAnimation(uint32_t id, Animation* animation);		// Changes 
		bool CheckAnimation(uint32_t id, std::string name);
		bool CheckAnimator(uint32_t id);
	};
}

