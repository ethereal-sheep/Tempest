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
#include "Core.h"
#include "SoundMeta.h"
#include "AudioEngine.h"

namespace Tempest
{
	namespace Audio
	{
		class AudioManager
		{
		public:
			using Audiofiles = std::vector<std::string>;
			using SoundMap = std::unordered_map<std::string, SoundID>;
			using SoundList = std::vector<std::string>;
			using SoundBusList = std::vector<std::string>;

			// construct buses
			AudioManager();

			bool AddAudioFile(const std::string&);
			bool DeleteAudioFile(const std::string&);
			const Audiofiles& GetAudiofiles() const;
			bool AudioFileExist(const std::string& filepath) const;

			// registers the sound to the manager and returns the id
			//SoundID RegisterSound(const SoundMeta& soundMeta);
			SoundID RegisterSound(SoundMeta&& soundMeta);

			// just use this for general case
			bool RegisterSound(const std::string& filename, const std::string& soundName);

			// deregister the sound, remove it from manager
			void UnregisterSound(const std::string& soundName);

			// functions to find sound def
			bool SoundExist(const std::string& soundName) const;
			SoundMeta& GetSoundMeta(const std::string& soundName);
			SoundID GetSoundID(const std::string& soundName) const;

			const SoundList& GetSounds() const;
			const SoundBusList& GetSoundBuses() const;

			bool RenameSound(const std::string& oldSoundName, const std::string& newSoundName);

		private:
			Audiofiles m_Audiofiles;
			SoundMap m_Sounds;
			SoundList m_SoundList;
			SoundBusList m_SoundBusList;

			AudioEngine ae;
		};
	}
}
