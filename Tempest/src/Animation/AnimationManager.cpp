#include "AnimationManager.h"
#include "Logger/Log.h"
#include <cstring>

namespace Tempest
{
	void AnimationManager::AddAnimator(uint32_t id, Animator anim)
	{

		m_Animators.insert(std::make_pair(id, std::move(anim)));
	}

	void AnimationManager::UpdateAnimations(float dt)
	{
		for (auto& i : m_Animators)
		{
			i.second.UpdateAnimation(dt);
		}
	}
	
	tvector<future_bool> AnimationManager::AsyncUpdateAnimations(float dt)
	{
		tvector<future_bool> v;
		for (auto& i : m_Animators)
		{
			auto fn = [&]()
			{
				i.second.UpdateAnimation(dt);
			};

			v.push_back(Service<thread_pool>::Get().submit_task(fn));
		}

		return v;
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
			m_Animators.at(id).ChangeAnimation(animation);
		}
	}

	bool AnimationManager::CheckAnimation(uint32_t id, std::string name)
	{
		auto& animator = m_Animators.at(id);
		return !strcmp(animator.GetName().c_str(), name.c_str());
	}

	bool AnimationManager::CheckAnimator(uint32_t id)
	{
		return m_Animators.contains(id);
	}

	void AnimationManager::ChangeDuration(uint32_t id, float duration)
	{
		m_Animators.at(id).ChangeDuration(duration);
	}
}