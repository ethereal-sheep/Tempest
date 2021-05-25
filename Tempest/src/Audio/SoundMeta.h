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
#include <string>
#include <filesystem>

namespace Tempest
{
	namespace Audio
	{
		struct SoundMeta
		{
			enum class Compression : uint8_t
			{
				DECOMPRESSED,
				COMPRESSED,
				STREAM
			};

			std::filesystem::path m_Filename; // should not be changed
			std::string m_SoundName;
			std::string m_SoundBus;

			//Compression m_Compression{Compression::DECOMPRESSED};

			
			bool m_Is3D{false};
			bool m_IsLooped{false};
			bool m_IsStreaming{false};
			bool m_IsPreloaded{true};


			SoundMeta() = default;
			SoundMeta(
				const std::string & filename,
				const std::string & soundname)
				: m_Filename(filename)
				, m_SoundName(soundname)
			{
			}

		};
	}
}
