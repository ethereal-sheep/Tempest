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
#include "AudioEngine.h"
#include "fmod/fmod.hpp"

namespace Tempest
{
	namespace Audio
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
				FMOD::ChannelGroup* m_ChannelGrp{nullptr};

				SoundBus(Implementation& impl, const std::string& name)
					: m_Impl{ impl }
				{
					m_Impl.mp_System->createChannelGroup(name.c_str(), &m_ChannelGrp);
				}
				~SoundBus()
				{
					if(m_ChannelGrp)
						m_ChannelGrp->release();
				}
			};
			
			struct SoundObject
			{
				FMOD::Sound* m_Sound;
				SoundMeta m_Meta;
			};

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
					VIRTUAL,
					DEVIRTUALIZE,
					VIRTUALIZING,
					LOUSY
				};

				Implementation& m_Impl;
				SoundID m_SoundId;
				SoundDefinition m_SoundDef;

				FMOD::Channel* m_Channel{nullptr};
				State m_State{State::INITIALIZE};
				bool m_StopRequested{false};

				bool ShouldBeVirtual(bool)
				{
					// some metric to detect virtualness

					// check the vol
					// check priority etc.

					return false;
				}

				bool IsLooped()
				{
					auto it = m_Impl.m_Sounds.find(m_SoundId);
					// assert
					if (it == m_Impl.m_Sounds.end())
						return false;

					return it->second->m_Meta.m_IsLooped;
				}

				bool IsOneShot()
				{
					return !IsLooped();
				}

				bool SoundIsLoaded()
				{
					auto it = m_Impl.m_Sounds.find(m_SoundId);
					if (it == m_Impl.m_Sounds.end())
						return false;

					return it->second->m_Sound;
				}

				bool LoadSound()
				{
					return m_Impl.Load(m_SoundId);
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
					const SoundMeta& meta = m_Impl.m_Sounds[m_SoundId]->m_Meta;
					if (meta.m_Is3D)
					{
						// set pos
						FMOD_VECTOR pos = vecToFmodVec(m_SoundDef.m_Pos);
						m_Channel->set3DAttributes(&pos, nullptr);

						//m_Channel->set3DMinMaxDistance(m_SoundDef.m_MinDistance, m_SoundDef.m_MaxDistance);
					}

					// set vol
					m_Channel->setVolume(m_SoundDef.m_Vol);
					m_Channel->setPitch(m_SoundDef.m_Pitch);

					//
					//m_Channel->set3DMinMaxDistance(0.f,0.f);
				}
				void Update(float)
				{
					switch (m_State)
					{
					case State::INITIALIZE:
						[[fallthrough]];
					case State::DEVIRTUALIZE:
						[[fallthrough]];
					case State::TOPLAY:
						{
							if (m_StopRequested)
							{
								/*if (!els::is_zero(m_SoundDef.m_FadeOut))
								{
									unsigned long long dspclock;
									m_Channel->getDSPClock(0, &dspclock);
									m_Channel->addFadePoint(dspclock, m_SoundDef.m_Vol);
									m_Channel->addFadePoint(dspclock + static_cast<unsigned long long>(m_SoundDef.m_FadeOut) * 48000, 0.0f);
								}*/

								m_State = State::STOPPING;
								return;
							}
							if (ShouldBeVirtual(true))
							{
								if (IsOneShot())
									m_State = State::STOPPING;
								else
									m_State = State::VIRTUAL;

								return;
							}
							if (!SoundIsLoaded())
							{
								if (LoadSound())
									m_State = State::LOADING;
								else
									m_State = State::LOUSY;
								return;
							}
							m_Channel = nullptr;
							auto it = m_Impl.m_Sounds.find(m_SoundId);
							if (it != m_Impl.m_Sounds.end())
							{
								// check audio bus
								// if audio bus exist, use the correct bus
								FMOD::ChannelGroup* bus;
								if (m_Impl.m_Buses.count(it->second->m_Meta.m_SoundBus))
									bus = m_Impl.m_Buses[it->second->m_Meta.m_SoundBus]->m_ChannelGrp;
								// else use the master bus
								else
									m_Impl.mp_System->getMasterChannelGroup(&bus);
								
								m_Impl.mp_System->playSound(it->second->m_Sound, bus, true, &m_Channel);

								unsigned long long dspclock;
								m_Channel->getDSPClock(0, &dspclock);
								m_Channel->addFadePoint(dspclock, 0.0f);
								m_Channel->addFadePoint(dspclock + static_cast<unsigned long long>(m_SoundDef.m_FadeOut) * 48000, m_SoundDef.m_Vol);
							}
							if (m_Channel)
							{
								if (m_State == State::DEVIRTUALIZE)
								{
									// virtual fade start
								}
								m_State = State::PLAYING;
								FMOD_VECTOR position = vecToFmodVec(m_SoundDef.m_Pos);
								m_Channel->set3DAttributes(&position, nullptr);
								m_Channel->setVolume(GetVolume());
								m_Channel->setPaused(false);
							}
							else
							{
								m_State = State::STOPPING;
							}
						}
						break;
					case State::LOADING:
						if (SoundIsLoaded())
						{
							m_State = State::TOPLAY;
						}
						break;
					case State::PLAYING:
						// virtualize fader update
						UpdateChannelParams();
						if (!IsPlaying() || m_StopRequested)
						{
							m_State = State::STOPPING;
							return;
						}
						if (ShouldBeVirtual(false))
						{
							// virtualize fader start
							m_State = State::VIRTUALIZING;
						}
						break;
					case State::STOPPING:
					{
						// stop fade update
						/*if (!els::is_zero(m_SoundDef.m_FadeOut))
						{
							unsigned long long dspclock;
							m_Channel->getDSPClock(0, &dspclock);
							m_Channel->addFadePoint(dspclock, m_SoundDef.m_Vol);
							m_Channel->addFadePoint(dspclock + static_cast<unsigned long long>(m_SoundDef.m_FadeOut) * 48000, 0.0f);
						}*/
						UpdateChannelParams();

						if (true) // stop fader is finished
						{
							m_Channel->stop();
						}
						if (!IsPlaying())
						{
							m_State = State::STOPPED;
						}
						break;
					}
					case State::VIRTUALIZING:
						// virtualize fader update
						UpdateChannelParams();
						if (!ShouldBeVirtual(false))
						{
							// virtualize fader start
							m_State = State::PLAYING;
							break;
						}
						if constexpr (true) // virtualize fader finish
						{
							m_Channel->stop();
							m_State = State::VIRTUAL;
						}
						break;
					case State::VIRTUAL:
						if (m_StopRequested)
						{
							m_State = State::STOPPING;
						}
						else if (!ShouldBeVirtual(false))
						{
							m_State = State::DEVIRTUALIZE;
						}
						break;
					case State::STOPPED:
						break;
					default:
						break;
					}
				}
			};

			using SoundMap = std::unordered_map<SoundID, std::unique_ptr<SoundObject>>;
			using ChannelMap = std::unordered_map<ChannelID, std::unique_ptr<SoundChannel>>;
			using BusMap = std::unordered_map<std::string, std::unique_ptr<SoundBus>>;

			Implementation()
			{
				System_Create(&mp_System);
				mp_System->init(128, FMOD_INIT_NORMAL, nullptr);

				m_Buses.emplace("SFX", std::make_unique<SoundBus>(*this, "SFX"));
				m_Buses.emplace("BGM", std::make_unique<SoundBus>(*this, "BGM"));

				//mp_System->pl
			}

			~Implementation()
			{
				mp_System->release();
			}

			void Update(float dt)
			{
				std::vector<ChannelMap::iterator> stoppedChannels;
				for (auto it = m_Channels.begin(), itEnd = m_Channels.end(); it != itEnd; ++it)
				{
					it->second->Update(dt);
					if (it->second->m_State == SoundChannel::State::STOPPED)
					{
						stoppedChannels.push_back(it);
					}
				}

				for (auto& c : stoppedChannels)
					m_Channels.erase(c);

				mp_System->update();
			}

			bool Load(SoundID id)
			{
				// find the sound name in Sounds
				// if found, means sound already exist inside Sounds
				// update SD
				auto found = m_Sounds.find(id);
				if (found == m_Sounds.end())
				{
					// id missing 
					// sound not registered
					return false;
				}

				auto& obj = found->second;
				//check if sound is already loaded
				if (obj->m_Sound)
					return true;

				// set the mode of the player
				FMOD_MODE mode = FMOD_DEFAULT;

				mode |= obj->m_Meta.m_Is3D ? FMOD_3D : FMOD_2D;
				mode |= obj->m_Meta.m_IsLooped ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
				mode |= obj->m_Meta.m_IsStreaming ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;


				// create the sound
				mp_System->createSound(obj->m_Meta.m_Filename.string().c_str(), mode, nullptr, &obj->m_Sound);

				if (obj->m_Sound)
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

			SoundID m_NextSoundID{0};
			ChannelID m_NextChannelID{0};
		};

		static std::unique_ptr<Implementation> audioImpl;


		void AudioEngine::Init()
		{
			audioImpl = std::make_unique<Implementation>();
		}

		void AudioEngine::Update(float dt)
		{
			audioImpl->Update(dt);
		}

		void AudioEngine::Shutdown()
		{
			audioImpl.reset();
		}

		bool AudioEngine::IsOkay()
		{
			return audioImpl.get();
		}



		SoundID AudioEngine::RegisterSound(const SoundMeta& sd)
		{
			SoundID sid = audioImpl->m_NextSoundID++;
			audioImpl->m_Sounds.emplace(
				sid, std::make_unique<Implementation::SoundObject>(Implementation::SoundObject{nullptr, sd}));
			return sid;
		}

		SoundID AudioEngine::RegisterSound(SoundMeta&& sd)
		{
			SoundID sid = audioImpl->m_NextSoundID++;
			audioImpl->m_Sounds.emplace(
				sid, std::make_unique<Implementation::SoundObject>(Implementation::SoundObject{
					nullptr, std::move(sd)
				}));
			return sid;
		}

		void AudioEngine::UnregisterSound(SoundID id)
		{
			auto found = audioImpl->m_Sounds.find(id);
			if (found == audioImpl->m_Sounds.end())
				return;

			audioImpl->m_Sounds.erase(found);
		}

		bool AudioEngine::Load(SoundID id)
		{
			return audioImpl->Load(id);
		}

		bool AudioEngine::Unload(SoundID id)
		{
			// find the sound name in Sounds
			// if not found, means nothing to unload
			auto found = audioImpl->m_Sounds.find(id);
			if (found == audioImpl->m_Sounds.end()) return false;

			found->second->m_Sound->release();
			found->second->m_Sound = nullptr;

			return true;
		}

		void AudioEngine::UnloadAllSounds()
		{
			for (auto& obj : audioImpl->m_Sounds)
			{
				if (obj.second->m_Sound)
				{
					obj.second->m_Sound->release();
					obj.second->m_Sound = nullptr;
				}
			}
		}

		ChannelID AudioEngine::Play(SoundID id, const SoundDefinition& sd)
		{
			ChannelID cid = audioImpl->m_NextChannelID++;

			auto it = audioImpl->m_Sounds.find(id);
			if (it == audioImpl->m_Sounds.end()) // no sound found
				return cid;

			audioImpl->m_Channels.emplace(
				cid, std::make_unique<Implementation::SoundChannel>(Implementation::SoundChannel{*audioImpl, id, sd}));
			return cid;
		}

		SoundMeta& AudioEngine::GetSoundMeta(SoundID id)
		{
			return audioImpl->m_Sounds.at(id)->m_Meta;
		}

		bool AudioEngine::HasSoundMeta(SoundID id)
		{
			return (audioImpl->m_Sounds.find(id) != audioImpl->m_Sounds.end());
		}

		void AudioEngine::StopChannel(const size_t& channelID)
		{
			auto found = audioImpl->m_Channels.find(channelID);
			if (found == audioImpl->m_Channels.end()) return;

			found->second->m_StopRequested = true;
		}

		void AudioEngine::TogglePauseChannel(const size_t& channelID)
		{
			auto found = audioImpl->m_Channels.find(channelID);
			if (found == audioImpl->m_Channels.end()) return;

			bool isPaused = false;
			found->second->m_Channel->getPaused(&isPaused);


			if (isPaused)
				found->second->m_Channel->setPaused(false);
			else
				found->second->m_Channel->setPaused(true);
		}

		void AudioEngine::StopAllChannels()
		{
			for ([[maybe_unused]] auto& [id, c] : audioImpl->m_Channels)
			{
				c->m_StopRequested = true;
			}
		}

		void AudioEngine::PauseAllChannels()
		{
			for ([[maybe_unused]] auto& [id, c] : audioImpl->m_Channels)
			{
				c->m_Channel->setPaused(true);
			}
		}

		void AudioEngine::UnpauseAllChannels()
		{
			for ([[maybe_unused]] auto& [id, c] : audioImpl->m_Channels)
			{
				c->m_Channel->setPaused(false);
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

		void AudioEngine::SetChannel3dPosition(const size_t& channelID, const vec3& pos)
		{
			auto found = audioImpl->m_Channels.find(channelID);
			if (found == audioImpl->m_Channels.end()) return;

			found->second->m_SoundDef.m_Pos = pos;
		}

		bool AudioEngine::CheckBusExist(const std::string& bus)
		{
			return audioImpl->m_Buses.count(bus);
		}

		void AudioEngine::SetChannelVolume(const size_t& channelID, float vol)
		{
			if (!audioImpl)
				return;

			auto found = audioImpl->m_Channels.find(channelID);
			if (found == audioImpl->m_Channels.end()) return;

			if (vol < 0)
				vol = 0;

			found->second->m_SoundDef.m_Vol = vol;
		}

		float AudioEngine::GetChannelVolume(const size_t& channelID)
		{
			auto found = audioImpl->m_Channels.find(channelID);
			if (found == audioImpl->m_Channels.end()) return -1.f;
			
			return found->second->m_SoundDef.m_Vol;
		}

		void AudioEngine::SetBusVolume(const std::string& bus, float vol)
		{
			if (!audioImpl->m_Buses.count(bus))
				return;
			if (vol < 0)
				vol = 0;

			audioImpl->m_Buses[bus]->m_ChannelGrp->setVolume(vol);
		}

		float AudioEngine::GetBusVolume(const std::string& bus)
		{
			if (!audioImpl->m_Buses.count(bus))
				return 0.f;
			float vol;
			audioImpl->m_Buses[bus]->m_ChannelGrp->getVolume(&vol);
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

			for (auto& [name, ptr] : audioImpl->m_Buses)
			{
				ptr->m_ChannelGrp->setVolume(0.5);
			}
		}

		bool AudioEngine::IsPlaying(const size_t& channelID)
		{
			auto found = audioImpl->m_Channels.find(channelID);
			if (found == audioImpl->m_Channels.end()) return false;

			return found->second->m_State != Implementation::SoundChannel::State::STOPPED;
		}

		unsigned int AudioEngine::GetChannelLength(const size_t& channelID)
		{
			auto found = audioImpl->m_Channels.find(channelID);
			if (found == audioImpl->m_Channels.end()) return 0;

			FMOD::Sound* pSound;
			found->second->m_Channel->getCurrentSound(&pSound);

			if (pSound)
			{
				unsigned int length = 0;
				pSound->getLength(&length, FMOD_TIMEUNIT_MS);
				return length;
			}
			return 0;
		}
	}
}
