#include "AnimationManager.h"
#include "Logger/Log.h"
#include <cstring>


namespace Tempest
{
	AnimationManager::AnimationManager()
	{
		LoadAnimations();
	}

	void AnimationManager::LoadAnimations()
	{
		string path = "Animations/";

        for (const auto & file : std::filesystem::directory_iterator(path))
            LoadAnimation(file.path().string());
	}

	void AnimationManager::LoadAnimation(const std::string& path)
	{
		std::shared_ptr<ModelPBR> temp = std::make_shared<ModelPBR>();
        temp->loadModel(path);
        m_Animations.insert(std::make_pair(path, std::move(temp)));
	}

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

	void AnimationManager::ChangeAnimation(uint32_t id, std::string anim)
	{
		auto found = m_Animators.find(id);
		
		if (found == m_Animators.end())
		{
			LOG_WARN("Trying to access Invalid Animation");
		}

		else
		{
			if(m_Animators.at(id).IsAnimationEnded())
			{
				auto& animation = m_Animations.at(anim)->GetAnimation();
				m_Animators.at(id).ChangeAnimation(&animation);
			}
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

	void AnimationManager::PlayAnimation(uint32_t id, bool loop)
	{
		m_Animators.at(id).Play(loop);
	}

	void AnimationManager::PauseAnimation(uint32_t id)
	{
		m_Animators.at(id).Pause();
	}

	void AnimationManager::StopAnimation(uint32_t id)
	{
		m_Animators.at(id).Stop();
	}

	void AnimationManager::SetSpeed(uint32_t id, float speed)
	{
		m_Animators.at(id).SetSpeed(speed);
	}

	void AnimationManager::ForceChange(uint32_t id, std::string anim)
	{
		auto found = m_Animators.find(id);

		if (found == m_Animators.end())
		{
			LOG_WARN("Trying to access Invalid Animation");
		}

		else
		{
			auto& animation = m_Animations.at(anim)->GetAnimation();
			m_Animators.at(id).ChangeAnimation(&animation);
		}
	}

	bool AnimationManager::isPlaying(uint32_t id) const
	{
		return m_Animators.at(id).GetState() == ANIMATOR_STATE::PLAY;
	}

	bool AnimationManager::isPaused(uint32_t id) const
	{
		return m_Animators.at(id).GetState() == ANIMATOR_STATE::PAUSE;
	}

	bool AnimationManager::isStopped(uint32_t id) const
	{
		return m_Animators.at(id).GetState() == ANIMATOR_STATE::STOP;
	}

	bool AnimationManager::hasEnded(uint32_t id) const
	{
		return m_Animators.at(id).hasEnded();
	}
}