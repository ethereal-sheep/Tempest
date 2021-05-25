/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2020
* \note			Course: GAM200
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "AudioManager.h"
#include "fmod/fmod.hpp"

namespace Tempest
{
	namespace Audio
	{
		AudioManager::AudioManager()
		{
			m_SoundBusList.emplace_back("NONE");
			m_SoundBusList.emplace_back("SFX");
			m_SoundBusList.emplace_back("BGM");
		}
		bool AudioManager::AddAudioFile(const std::string& filepath)
		{
			// check if the filepath exist
			if (std::find(m_Audiofiles.begin(), m_Audiofiles.end(), filepath) != m_Audiofiles.end())
				return false;

			m_Audiofiles.push_back(filepath);
			return true;
		}
		bool AudioManager::DeleteAudioFile(const std::string&)
		{
			return false;
		}
		const AudioManager::Audiofiles& AudioManager::GetAudiofiles() const
		{
			return m_Audiofiles;
		}

		bool AudioManager::AudioFileExist(const std::string& filepath) const
		{
			return std::find(m_Audiofiles.begin(), m_Audiofiles.end(), filepath) != m_Audiofiles.end();
		}

		// registers the sound to the manager and returns the id
		//SoundID AudioManager::RegisterSound(const SoundMeta& soundMeta)
		//{
		//	// check if filename exist
		//	if (SoundExist(soundMeta.m_Filename))
		//		return GetSoundID(soundMeta.m_Filename);

		//	// doesnt do any checking cos mainly used for reloading
		//	SoundID newID = ae.RegisterSound(soundMeta);
		//	m_Sounds.emplace(std::make_pair(soundMeta.m_Filename, newID));
		//	m_SoundList.emplace_back(soundMeta.m_Filename);

		//	return newID;
		//}

		SoundID AudioManager::RegisterSound(SoundMeta&& soundMeta)
		{
			if (SoundExist(soundMeta.m_SoundName))
				return GetSoundID(soundMeta.m_SoundName);

			// doesnt do any checking cos mainly used for reloading
			std::string soundName = soundMeta.m_SoundName;
			SoundID newID = ae.RegisterSound(std::move(soundMeta));
			m_Sounds.emplace(std::make_pair(soundName, newID));
			m_SoundList.emplace_back(std::move(soundName));

			return newID;
		}

		bool AudioManager::RegisterSound(const std::string& filename, const std::string& soundName)
		{
			// check if sound exist
			if (SoundExist(soundName))
				return false;

			//else
			SoundID newID = ae.RegisterSound(SoundMeta{filename, soundName });
			m_Sounds.emplace(std::make_pair(soundName, newID));

			m_SoundList.push_back(soundName);
			return true;
		}

		// deregister the sound, remove it from manager
		void AudioManager::UnregisterSound(const std::string& soundName)
		{
			// check if filename exist
			if (!SoundExist(soundName))
				return;

			auto it = m_Sounds.find(soundName);

			ae.UnregisterSound(it->second);
			m_Sounds.erase(it);
			m_SoundList.erase(std::find(m_SoundList.begin(), m_SoundList.end(), soundName));
		}

		//overloads to find sound def
		bool AudioManager::SoundExist(const std::string& soundName) const
		{
			return m_Sounds.count(soundName);
		}

		SoundMeta& AudioManager::GetSoundMeta(const std::string& soundName)
		{
			//assuming exist
			return ae.GetSoundMeta(m_Sounds.at(soundName));
		}

		SoundID AudioManager::GetSoundID(const std::string& soundName) const
		{
			// assuming sound exist
			return m_Sounds.at(soundName);
		}

		const AudioManager::SoundList& AudioManager::GetSounds() const
		{
			return m_SoundList;
		}
		const AudioManager::SoundBusList& AudioManager::GetSoundBuses() const
		{
			return m_SoundBusList;
		}
		bool AudioManager::RenameSound(const std::string& oldSoundName, const std::string& newSoundName)
		{
			// check if sound exist
			if (!SoundExist(oldSoundName) || SoundExist(newSoundName))
				return false;

			//change meta
			auto& meta = GetSoundMeta(oldSoundName);
			meta.m_SoundName = newSoundName;

			// remove the pair from the map
			auto handle = m_Sounds.extract(oldSoundName);
			handle.key() = newSoundName;
			m_Sounds.insert(std::move(handle));

			// rename in the list
			auto it = std::find(m_SoundList.begin(), m_SoundList.end(), oldSoundName);
			*it = newSoundName;

			return true;

		}
	}
}
