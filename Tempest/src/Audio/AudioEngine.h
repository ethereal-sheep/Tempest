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
#include "TMath.h"
#include "Core.h"

namespace Tempest
{
	using ChannelID = id_t;

	class AudioEngine
	{
	public:
		static void Init();
		static void Update();
		static void Shutdown();
		static bool IsOkay();

		/**********************************************************************
		*
		* Sound functions
		*
		***********************************************************************/
		// play sound with highest level of specifics
		ChannelID Play(
			tpath path, string bus, const SoundDefinition &);

		// Play 
		ChannelID Play(tpath path, string bus, float vol = 0.5f, bool loop = false);

		// registers a sound and loads it
		bool RegisterSound(tpath file_path, bool is3D);
		bool Register2DSound(tpath file_path);
		bool Register3DSound(tpath file_path);

		// checks if a sound exists
		bool CheckSoundExist(tpath file_path);


		/**********************************************************************
		*
		* Channel functions
		*
		***********************************************************************/
		// stops given channel (fade time not implemented yet)
		void StopChannel(ChannelID channelID);

		// pause/unpause given channel
		void TogglePauseChannel(ChannelID channelID);

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


		// sets the channel 3D position
		// note: no effect for 2D sounds
		void SetChannel3dPosition(ChannelID channelID, vec3 pos);
		
		// check if a bus exist
		bool CheckBusExist(const std::string& bus);

		// getters/setter volume
		void SetChannelVolume(ChannelID channelID, float vol);
		float GetChannelVolume(ChannelID channelID);

		// set/get the bus volume
		void SetBusVolume(const std::string& bus, float vol);
		float GetBusVolume(const std::string& bus);

		// set/get master volume
		void SetMasterVolume(float vol);
		float GetMasterVolume();

		void ResetVolume();

		// returns true if given channgel is playing
		bool IsPlaying(ChannelID channelID);

		// returns the duration of the channel in ms
		unsigned int GetChannelLength(ChannelID channelID);

	};
}
