/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2020
* \note			Course: GAM200
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#include "SoundDefinition.h"
#include "SoundMeta.h"
#include "TMath.h"

namespace Tempest
{
	namespace Audio
	{
		using SoundID = size_t;
		using ChannelID = size_t;

		constexpr ChannelID NO_SOUND = static_cast<ChannelID>(-1);
		constexpr ChannelID NO_CHANNEL = static_cast<ChannelID>(-1);

		class AudioEngine
		{
		public:
			static void Init();
			static void Update(float dt);
			static void Shutdown();
			static bool IsOkay();

			/**********************************************************************
			*
			* Sound functions
			*
			***********************************************************************/
			SoundID RegisterSound(const SoundMeta& sd);
			SoundID RegisterSound(SoundMeta&& sd);
			void UnregisterSound(SoundID id);

			// load a sound given by the id, meta should already be specified
			bool Load(SoundID id);

			// unloads a sound
			bool Unload(SoundID id);
			void UnloadAllSounds();

			// plays a sound and returns the channel id
			ChannelID Play(
				SoundID id, const SoundDefinition& = SoundDefinition{});

			// gets the sound def
			SoundMeta& GetSoundMeta(SoundID id);

			bool HasSoundMeta(SoundID id);


			/**********************************************************************
			*
			* Channel functions
			*
			***********************************************************************/
			// stops given channel (fade time not implemented yet)
			void StopChannel(const size_t& channelID);

			// pause/unpause given channel
			void TogglePauseChannel(const size_t& channelID);

			// stops all channels
			void StopAllChannels();

			// pause all channels
			void PauseAllChannels();
			void UnpauseAllChannels();

			// sets the listener position and orientation
			// can add velocity for doppler
			void Set3DListener(
				const vec3& pos,
				const vec3& look,
				const vec3& up);

			/*void Set3DListenerOrientation(
				const quat&);*/

			// sets the channel 3D position
			// note: no effect for 2D sounds
			void SetChannel3dPosition(const size_t& channelID, const vec3& pos);

			bool CheckBusExist(const std::string& bus);

			// getters/setter volume
			void SetChannelVolume(const size_t& channelID, float vol);
			float GetChannelVolume(const size_t& channelID);

			void SetBusVolume(const std::string& bus, float vol);
			float GetBusVolume(const std::string& bus);

			void SetMasterVolume(float vol);
			float GetMasterVolume();

			void ResetVolume();

			// returns true if given channgel is playing
			bool IsPlaying(const size_t& channelID);

			// returns the duration of the channel in ms
			unsigned int GetChannelLength(const size_t& channelID);
		};
	}
}
