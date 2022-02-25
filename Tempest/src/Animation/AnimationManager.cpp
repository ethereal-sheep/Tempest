#include "AnimationManager.h"
#include "Logger/Log.h"

namespace Tempest
{
	void AnimationManager::AddAnimator(uint32_t id, Animator* anim)
	{
		m_Animators.insert(std::make_pair(id, anim));
	}

	void AnimationManager::UpdateAnimations(float dt)
	{
		for (auto& i : m_Animators)
			i.second->UpdateAnimation(dt);
	}

	void AnimationManager::ChangeAnimation(uint32_t id, Animation* animation)
	{
		auto found = m_Animators.find(id);

		if (found == m_Animators.end())
		{
			LOG_WARN("Trying to access Invalid Animation");
		}

		else
		{
			m_Animators[id]->ChangeAnimation(animation);
		}
	}
}