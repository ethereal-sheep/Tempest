/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2020
* \note			Course: GAM200
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "Core.h"
#include "Util/range.h"
#include "AudioEngine.h"
#include "fmod/fmod.hpp"

namespace Tempest
{
	template <typename TVec>
	FMOD_VECTOR vecToFmodVec(const TVec& vector)
	{
		FMOD_VECTOR v;
		v.x = static_cast<float>(vector.x);
		v.y = static_cast<float>(vector.y);
		v.z = static_cast<float>(vector.z);

		return v;
	}

	float dBToVol(float dB)
	{
		return pow(10.f, 0.05f * dB);
	}

	float VolTodB(float volume)
	{
		return 20.f * log10f(volume);
	}

	struct AudioFader
	{
		void Update(float)
		{
		}

		void Start()
		{
		}

		bool IsFinish() const
		{
		}
	};


	// using pImpl idiom
	struct Implementation
	{
		struct SoundBus
		{
			Implementation& m_Impl;
			FMOD::ChannelGroup* m_ChannelGrp{ nullptr };

			SoundBus(Implementation& impl, const std::string& name)
				: m_Impl{ impl }
			{
				m_Impl.mp_System->createChannelGroup(name.c_str(), &m_ChannelGrp);
			}
			~SoundBus()
			{
				if (m_ChannelGrp)
					m_ChannelGrp->release();
			}
		};

		/*struct SoundObject
		{
			FMOD::Sound* m_Sound;
			SoundMeta m_Meta;
		};*/

		struct SoundChannel
		{
			enum class State : unsigned int
			{
				INITIALIZE,
				TOPLAY,
				LOADING,
				PLAYING,
				STOPPING,
				STOPPED,
				LOUSY
			};

			Implementation& m_Impl;
			tpath m_SoundPath;
			string m_SoundBus;
			SoundDefinition m_SoundDef;

			FMOD::Channel* m_Channel{ nullptr };
			State m_State{ State::INITIALIZE };
			bool m_StopRequested{ false };


			bool IsLooped()
			{
				return m_SoundDef.m_IsLooped;
			}

			bool IsOneShot()
			{
				return !IsLooped();
			}

			bool IsPlaying() const
			{
				bool playing;
				m_Channel->isPlaying(&playing);
				return playing;
			}

			float GetVolume() const
			{
				return m_SoundDef.m_Vol;
			}

			void UpdateChannelParams()
			{
				// set vol
				FMOD_VECTOR pos = vecToFmodVec(m_SoundDef.m_Pos);
				m_Channel->set3DAttributes(&pos, nullptr);
				m_Channel->setVolume(m_SoundDef.m_Vol);
				m_Channel->setPitch(m_SoundDef.m_Pitch);
				m_Channel->set3DMinMaxDistance(m_SoundDef.m_MinDistance, m_SoundDef.m_MaxDistance);
			}

			void Update()
			{
				switch (m_State)
				{
				case State::INITIALIZE:
					[[fallthrough]];
				case State::TOPLAY:
				{
					if (m_StopRequested)
					{
						m_State = State::STOPPING;
						return;
					}
					// sound should already be loaded
					/*if (!SoundIsLoaded())
					{
						if (LoadSound())
							m_State = State::LOADING;
						else
							m_State = State::LOUSY;
						return;
					}*/

					m_Channel = nullptr;
					auto it = m_Impl.m_Sounds.find(m_SoundPath);
					if (it != m_Impl.m_Sounds.end())
					{
						// check audio bus
						// if audio bus exist, use the correct bus
						FMOD::ChannelGroup* bus;
						if (m_Impl.m_Buses.count(m_SoundBus))
							bus = m_Impl.m_Buses.at(m_SoundBus);
						// else use the master bus
						else
							m_Impl.mp_System->getMasterChannelGroup(&bus);

						m_Impl.mp_System->playSound(it->second, bus, false, &m_Channel);

						// fade in
						unsigned long long dspclock;
						m_Channel->getDSPClock(0, &dspclock);
						m_Channel->addFadePoint(dspclock, 0.0f);
						m_Channel->addFadePoint(dspclock + static_cast<unsigned long long>(m_SoundDef.m_FadeIn) * 48000, m_SoundDef.m_Vol);
					}

					// if channel still alive
					if (m_Channel)
					{
						m_State = State::PLAYING;
						UpdateChannelParams();
					}
					else
					{
						m_State = State::STOPPING;
					}
				}
				break;
				case State::PLAYING:
					// virtualize fader update
					UpdateChannelParams();
					if (!IsPlaying())
					{
						if (m_SoundDef.m_IsLooped)
						{
							m_State = State::TOPLAY;
							return;
						}
						else
						{
							m_StopRequested = true;
						}
					}
					if (m_StopRequested)
					{
						unsigned long long dspclock;
						m_Channel->getDSPClock(0, &dspclock);
						m_Channel->addFadePoint(dspclock + static_cast<unsigned long long>(m_SoundDef.m_FadeOut) * 48000, 0.f);

						m_State = State::STOPPING;
						return;
					}
					break;
				case State::STOPPING:
				{
					// stop fade update
					//UpdateChannelParams();
					float curr_vol = 0.f;
					m_Channel->getVolume(&curr_vol);
					if (curr_vol < 0.01f) // stop fader is finished
					{
						m_Channel->stop();
					}
					if (!IsPlaying())
					{
						m_State = State::STOPPED;
					}
					break;
				}
				case State::STOPPED:
					break;
				default:
					break;
				}
			}
		};

		using SoundMap = std::unordered_map<tpath, FMOD::Sound*, fs_hash>;
		using ChannelMap = std::unordered_map<ChannelID, SoundChannel>;
		using BusMap = std::unordered_map<std::string, FMOD::ChannelGroup*>;

		Implementation()
		{
			System_Create(&mp_System);
			mp_System->init(128, FMOD_INIT_3D_RIGHTHANDED, nullptr);

			FMOD::ChannelGroup* bus;
			bus = nullptr;
			mp_System->createChannelGroup("SFX", &bus);
			if(bus)
				m_Buses.emplace("SFX", bus);

			bus = nullptr;
			mp_System->createChannelGroup("BGM", &bus);
			if (bus)
				m_Buses.emplace("BGM", bus);

			bus = nullptr;
			mp_System->createChannelGroup("VL", &bus);
			if (bus)
				m_Buses.emplace("VL", bus);

		}

		~Implementation()
		{
			mp_System->release();
		}

		void Update()
		{
			std::vector<ChannelMap::iterator> stoppedChannels;
			for (auto it = m_Channels.begin(), itEnd = m_Channels.end(); it != itEnd; ++it)
			{
				it->second.Update();
				if (it->second.m_State == SoundChannel::State::STOPPED)
				{
					stoppedChannels.push_back(it);
				}
			}

			for (auto& c : stoppedChannels)
				m_Channels.erase(c);

			mp_System->update();
		}


		bool Load(tpath path, bool is3D)
		{
			// find the sound name in Sounds
			// if found, means sound already exist inside Sounds
			// update SD
			auto found = m_Sounds.find(path);
			if (found == m_Sounds.end())
			{
				//path missing 
				// sound not registered
				return false;
			}

			//check if sound is already loaded
			if (found->second)
				return true;

			// set the mode of the player
			FMOD_MODE mode = FMOD_DEFAULT;

			mode |= FMOD_LOOP_OFF;
			mode |= is3D ? FMOD_3D : FMOD_2D;

			// create the sound
			mp_System->createSound(path.string().c_str(), mode, nullptr, &found->second);

			if (found->second)
			{
				// sound successfully loaded
				return true;
			}
			// lousy file
			return false;
		}

		FMOD::System* mp_System;
		SoundMap m_Sounds;
		ChannelMap m_Channels;
		BusMap m_Buses;

		ChannelID m_NextChannelID{ 1 };
	};

	static std::unique_ptr<Implementation> audioImpl;


	void AudioEngine::Init()
	{
		audioImpl = std::make_unique<Implementation>();
	}

	void AudioEngine::Update()
	{
		audioImpl->Update();
	}

	void AudioEngine::Shutdown()
	{
		audioImpl.reset();
	}

	bool AudioEngine::IsOkay()
	{
		return audioImpl.get();
	}

	ChannelID AudioEngine::Play(tpath path, string bus, const SoundDefinition& sd)
	{
		auto it = audioImpl->m_Sounds.find(path);
		if (it == audioImpl->m_Sounds.end()) // no sound found
			return UNDEFINED;

		ChannelID cid = audioImpl->m_NextChannelID++;

		audioImpl->m_Channels.emplace(
			cid, Implementation::SoundChannel{ *audioImpl, path, bus, sd });
		return cid;
	}

	ChannelID AudioEngine::Play(tpath path, string bus, float vol, bool loop)
	{
		SoundDefinition sd;
		sd.m_IsLooped = loop;
		sd.m_Vol = vol;
		return Play(path, bus, sd);
	}

	bool AudioEngine::RegisterSound(tpath file_path, bool is3D)
	{
		if (audioImpl->m_Sounds.count(file_path))
			return true;

		audioImpl->m_Sounds.emplace(file_path, nullptr);
		return audioImpl->Load(file_path, is3D);
	}

	bool AudioEngine::Register2DSound(tpath file_path)
	{
		return RegisterSound(file_path, false);
	}

	bool AudioEngine::Register3DSound(tpath file_path)
	{
		return RegisterSound(file_path, true);
	}

	bool AudioEngine::CheckSoundExist(tpath file_path)
	{
		auto it = audioImpl->m_Sounds.find(file_path);
		return it != audioImpl->m_Sounds.end();
	}



	void AudioEngine::StopChannel(ChannelID channelID)
	{
		auto found = audioImpl->m_Channels.find(channelID);
		if (found == audioImpl->m_Channels.end()) return;

		found->second.m_StopRequested = true;
	}

	void AudioEngine::TogglePauseChannel(ChannelID channelID)
	{
		auto found = audioImpl->m_Channels.find(channelID);
		if (found == audioImpl->m_Channels.end()) return;

		bool isPaused = false;
		found->second.m_Channel->getPaused(&isPaused);


		if (isPaused)
			found->second.m_Channel->setPaused(false);
		else
			found->second.m_Channel->setPaused(true);
	}

	void AudioEngine::StopAllChannels()
	{
		for ([[maybe_unused]] auto& [id, c] : audioImpl->m_Channels)
		{
			c.m_StopRequested = true;
		}
	}

	void AudioEngine::PauseAllChannels()
	{
		for ([[maybe_unused]] auto& [id, c] : audioImpl->m_Channels)
		{
			c.m_Channel->setPaused(true);
		}
	}

	void AudioEngine::UnpauseAllChannels()
	{
		for ([[maybe_unused]] auto& [id, c] : audioImpl->m_Channels)
		{
			c.m_Channel->setPaused(false);
		}
	}

	void AudioEngine::Set3DListener(
		const vec3& pos,
		const vec3& look,
		const vec3& up)
	{
		FMOD_VECTOR position = vecToFmodVec(pos);
		FMOD_VECTOR lookat = vecToFmodVec(look);
		FMOD_VECTOR uphigh = vecToFmodVec(up);


		audioImpl->mp_System->set3DListenerAttributes(0, &position, nullptr, &lookat, &uphigh);
	}


	void AudioEngine::SetChannel3dPosition(ChannelID channelID, vec3 pos)
	{
		auto found = audioImpl->m_Channels.find(channelID);
		if (found == audioImpl->m_Channels.end()) return;

		found->second.m_SoundDef.m_Pos = pos;
	}

	bool AudioEngine::CheckBusExist(const std::string& bus)
	{
		return audioImpl->m_Buses.count(bus);
	}

	void AudioEngine::SetChannelVolume(ChannelID channelID, float vol)
	{
		if (!audioImpl)
			return;

		auto found = audioImpl->m_Channels.find(channelID);
		if (found == audioImpl->m_Channels.end()) return;

		if (vol < 0)
			vol = 0;

		found->second.m_SoundDef.m_Vol = vol;
	}

	float AudioEngine::GetChannelVolume(ChannelID channelID)
	{
		auto found = audioImpl->m_Channels.find(channelID);
		if (found == audioImpl->m_Channels.end()) return -1.f;

		return found->second.m_SoundDef.m_Vol;
	}

	void AudioEngine::SetBusVolume(const std::string& bus, float vol)
	{
		if (!audioImpl->m_Buses.count(bus))
			return;

		if (vol < 0)
			vol = 0;

		audioImpl->m_Buses.at(bus)->setVolume(vol);
	}

	float AudioEngine::GetBusVolume(const std::string& bus)
	{
		if (!audioImpl->m_Buses.count(bus))
			return 0.f;
		float vol;
		audioImpl->m_Buses.at(bus)->getVolume(&vol);
		return vol;
	}

	void AudioEngine::SetMasterVolume(float vol)
	{
		if (vol < 0)
			vol = 0;
		FMOD::ChannelGroup* master;
		audioImpl->mp_System->getMasterChannelGroup(&master);
		master->setVolume(vol);
	}

	float AudioEngine::GetMasterVolume()
	{
		FMOD::ChannelGroup* master;
		audioImpl->mp_System->getMasterChannelGroup(&master);

		float vol;
		master->getVolume(&vol);
		return vol;
	}

	void AudioEngine::ResetVolume()
	{
		SetMasterVolume(0.5f);

		for (auto& [name, bus] : audioImpl->m_Buses)
		{
			bus->setVolume(0.5);
		}
	}

	bool AudioEngine::IsPlaying(ChannelID channelID)
	{
		auto found = audioImpl->m_Channels.find(channelID);
		if (found == audioImpl->m_Channels.end()) return false;

		return found->second.m_State != Implementation::SoundChannel::State::STOPPED;
	}

	unsigned int AudioEngine::GetChannelLength(ChannelID channelID)
	{
		auto found = audioImpl->m_Channels.find(channelID);
		if (found == audioImpl->m_Channels.end()) return 0;

		FMOD::Sound* pSound;
		found->second.m_Channel->getCurrentSound(&pSound);

		if (pSound)
		{
			unsigned int length = 0;
			pSound->getLength(&length, FMOD_TIMEUNIT_MS);
			return length;
		}
		return 0;
	}
}
