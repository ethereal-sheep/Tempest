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
#include "TMath.h"

namespace Tempest
{
	struct SoundDefinition
	{
		float m_Vol{ 0.5f };
		float m_Pitch{ 1.0f }; // between 0.5 to 2.0
		float m_FadeIn{ 0.f };
		float m_FadeOut{ 0.f };

		// 3d stuff
		vec3 m_Pos{ 0.f };
		float m_MinDistance{ 1.f };
		float m_MaxDistance{ 500.f };

		//booleans
		bool m_IsLooped{ false };


		/*template <typename Archive>
		friend Archive& operator& (Archive& ar, SoundDefinition& sound)
		{
			ar.StartObject();
			ar.Member("Volume", sound.m_Vol);
			ar.Member("Pitch", sound.m_Pitch);
			ar.Member("FadeIn", sound.m_FadeIn);
			ar.Member("FadeOut", sound.m_FadeOut);
			ar.Member("Position", sound.m_Pos);
			ar.Member("MinDistance", sound.m_MinDistance);
			ar.Member("MaxDistance", sound.m_MaxDistance);
			ar.Member("IsMute", sound.m_IsMute);
			ar.Member("IsLooped", sound.m_IsLooped);
			ar.Member("ByPassOnFilters", sound.m_BypassFilters);
			ar.Member("PlayOnAwake", sound.m_PlayOnAwake);
			return ar.EndObject();
		}*/
	};
}
