/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include "Core.h"
#include "Audio/AudioEngine.h"
#include "Font.h"

namespace Tempest
{
	class AudioPlayer
	{
		enum struct PlayerState
		{
			RELOADED,
			LOUSY,
			STOP,
			PLAY,
			PAUSE,
		};

		tpath selected{ "" };
		tpath prev{ "" };

		PlayerState state{ PlayerState::RELOADED };
		ChannelID current{ UNDEFINED };
		float elapsed{ 0.f };
		float vol{ 0.5f };

	public:
		void SelectNewTrack(tpath path)
		{
			selected = path;
		}

		const tpath& GetCurrentTrack() const
		{
			return selected;
		}

		void Draw()
		{

			if (selected != prev)
			{
				state = PlayerState::RELOADED;
				prev = selected;
			}

			AudioEngine ae;


			ImGui::GetColumnWidth();
			ImGui::BeginChild("", ImVec2(ImGui::GetColumnWidth() - 15.f, 70));


			float length = static_cast<float>(ae.GetChannelLength(current)) / 1000.f;
			float inPercent = (length == 0) ? 0.f : elapsed / length;

			int elapsed_minutes = static_cast<int>(elapsed / 60);
			int elapsed_seconds = static_cast<int>(elapsed) % 60;

			int total_minutes = static_cast<int>(length / 60);
			int total_seconds = static_cast<int>(length) % 60;

			char buffer[32];
			sprintf_s(buffer, 32, "%i:%02i / %i:%02i",
				elapsed_minutes, elapsed_seconds,
				total_minutes, total_seconds);

			ImGui::ProgressBar(inPercent, ImVec2(-1.f, 0.0f), buffer);


			ImGui::Button("", ImVec2(0.01f, 0.f));
			ImGui::SameLine();
			switch (state)
			{
			case PlayerState::RELOADED:
				if (ae.CheckSoundExist(selected))
				{
					ae.StopChannel(current);
					vol = 0.5f;
					elapsed = 0.f;
					state = PlayerState::STOP;
				}
				else
				{
					state = PlayerState::LOUSY;
				}

				break;
			case PlayerState::LOUSY:
				ImGui::TextDisabled("Lousy file.");
				break;
			case PlayerState::STOP:

				elapsed = 0.f;
				if (ImGui::Button(ICON_FA_PLAY))
				{
					current = ae.Play(selected, "", vol);
					state = PlayerState::PLAY;
				}
				ImGui::SameLine();
				ImGui::TextDisabled(ICON_FA_PAUSE);
				ImGui::SameLine();
				ImGui::TextDisabled(ICON_FA_STOP);
				break;
			case PlayerState::PLAY:

				elapsed += ImGui::GetIO().DeltaTime;

				ImGui::TextDisabled(ICON_FA_PLAY);

				ImGui::SameLine();

				if (ImGui::Button(ICON_FA_PAUSE))
				{
					ae.TogglePauseChannel(current);
					state = PlayerState::PAUSE;
				}

				ImGui::SameLine();

				if (ImGui::Button(ICON_FA_STOP))
				{
					ae.StopChannel(current);
					current = UNDEFINED;
					state = PlayerState::STOP;
				}

				// check if still playing
				if (!ae.IsPlaying(current))
				{
					state = PlayerState::STOP;
				}
				break;
			case PlayerState::PAUSE:
				if (ImGui::Button(ICON_FA_PLAY))
				{
					ae.TogglePauseChannel(current);
					state = PlayerState::PLAY;
				}
				ImGui::SameLine();
				ImGui::TextDisabled(ICON_FA_PAUSE);
				ImGui::SameLine();
				if (ImGui::Button(ICON_FA_STOP))
				{
					ae.StopChannel(current);
					state = PlayerState::STOP;
				}
				break;
			default:
				break;
			}

			ImGui::EndChild();
			ImGui::SameLine();

			if (ImGui::VSliderFloat("##VolSlider", ImVec2(10.f, 45.0f), &vol, 0.f, 1.f, ""))
			{
				ae.SetChannelVolume(current, vol);
			}
		}
	};
}