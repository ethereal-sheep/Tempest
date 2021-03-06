/**********************************************************************************
* \author		Lim Ziyi Jean(ziyijean.lim@digipen.edu),
*				Huang Xurong(h.xurong@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "SimulateOverlay.h"
#include "Tempest/src/Graphics/OpenGL/Texture.h"
#include "Triggers/SimulationTriggers.h"
#include "Triggers/Triggers.h"
#include "Instance/EditTimeInstance.h"
#include "InstanceManager/InstanceConfig.h"

#include "../../Tempest/src/Particles/Particles_2D/EmitterSystem_2D.h"
#include "Particles/Particles_2D/WaypointEmitter_2D.h"
#include "Particles/Particles_2D/CircularMotionEmitter_2D.h"
#include "Particles/Particles_2D/ExplosionEmitter_2D.h"

namespace Tempest
{
	void SimulateOverlay::open_popup(const Event& e)
	{
		auto a = event_cast<OpenSimulateTrigger>(e);
		OverlayOpen = true;
		window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoFocusOnAppearing;;
		attacker.Reset(a.instance);
		defender.Reset(a.instance);

		if (!a.instance.ecs.view<Components::Graph>(exclude_t<tc::Destroyed>()).contains(sequence))
			sequence = UNDEFINED;

		padding = ImGui::GetMainViewport()->Size.y * 0.02f;
		win = 0;
		lose = 0;
		enter_button = tex_map["Assets/EnterButton.dds"];

		for (int i = 0; i < inter_nest.size(); ++i)
		{
			inter_nest[i].start(-.18f * ImGui::GetMainViewport()->Size.x, .0f, .4f, i * .05f, [](float x) { return glm::cubicEaseOut(x); });
		}
		inter.start(-0.1f, 0.02f, .25f, 0, [](float x) { return glm::cubicEaseOut(x); });

		tutorial_index = 0;
		tutorial_p2 = false;
		tut_openSlide = true;

		// Task Complete VFX Triggers
		b_unit_select_defender_task_VFX = false;
		b_unit_select_weapon_task_VFX = false;
		b_unit_select_action_task_VFX = false;

		emitter_1_0 = false;
		emitter_1_1 = false;
		emitter_1_2 = false;

		emitter_2_0 = false;
		emitter_2_1 = false;
		emitter_2_2 = false;
		emitter_2_3 = false;
		emitter_2_4 = false;
		emitter_2_5 = false;
		emitter_2_6 = false;
		emitter_2_7 = false;

		emitter_C_0 = false;
	}

	void SimulateOverlay::close_popup(const Event& e)
	{
		auto a = event_cast<CloseOverlayTrigger>(e);
		if (a.current == QUICKMENU_POPUP_TYPE::SIMULATE)
		{
			OverlayOpen = false;
			AudioEngine ae;
			ae.StopChannel(voice_line);
			voice_line = 0;
		}
	}

	void SimulateOverlay::force_close(const Event&)
	{
		OverlayOpen = false;

		win = 0;
		lose = 0;
		attack = UNDEFINED;
		defend = UNDEFINED;
		finish = false;

	}

	void SimulateOverlay::confirm_data(const Event& e)
	{
		auto a = event_cast<SimulateSelectionConfirm>(e);

		if (a.for_unitpage)
			return;

		UnitData& owner = a.is_attacker ? attacker : defender;
		switch (a.type)
		{
		case SIMULATE_POPUP_TYPE::UNIT:
			owner.unit_id = a.data;
			break;
		case SIMULATE_POPUP_TYPE::WEAPON:
			owner.weapon = a.data;
			break;
		case SIMULATE_POPUP_TYPE::ACTION:
			owner.action = a.data;
			break;
		case SIMULATE_POPUP_TYPE::SEQUENCE:
			sequence = a.data;
			break;
		default:
			break;
		}
	}

	void SimulateOverlay::simulate_tutorial_p2(const Event&)
	{
		tutorial_p2 = true;
		AudioEngine ae;
		voice_line = ae.Play("Sounds2D/Cr_Simulation_1.wav", "VL", 1.0f);
	}

	void SimulateOverlay::show(Instance& instance)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);

		{
			float dt = ImGui::GetIO().DeltaTime;
			for (auto& i : inter_nest)
				i.update(dt);

			inter.update(dt);
		}

		if (OverlayOpen)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.f });

			if (ImGui::Begin("Simulate Page Configure", nullptr, window_flags))
			{
				// tutorial progrss
				if (instance.tutorial_enable && !instance.tutorial_temp_exit)
				{
					auto drawlist = ImGui::GetForegroundDrawList();
					if (instance.tutorial_level != 1) //set Slide to false if not tut level 1
						instance.tutorial_slide = false;
					if (instance.tutorial_level == 1)
					{
						if (!tutorial_p2)
						{
							switch (tutorial_index)
							{
								// Click to quick menu

							case 0:
							{
								ImVec2 pos = { viewport->Size.x * 0.1f, viewport->Size.y * 0.025f };
								ImVec2 size = { 200.f, 50.f };
								UI::TutArea(pos, size);
								block_input_if_mouse_not_in_bounds(pos, size);
								string str = string(ICON_FK_EXCLAMATION_CIRCLE) + "Click here to access the quick menu.";
								drawlist->AddText({ pos.x + size.x + 10.f, pos.y + size.y - 10.f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());
								/*if (UI::MouseIsWithin(pos, { pos.x + size.x, pos.y + size.y }) && ImGui::IsMouseDown(0))
									ImGui::GetIO().MouseClicked[0] = true;*/
							}
							break;
							case 1:
							{
								ImVec2 pos = { 0.f, 0.f };
								ImVec2 size = { viewport->Size.x, viewport->Size.y * 0.25f };
								UI::TutArea(pos, size);
								block_input_if_mouse_not_in_bounds({ 0,0 }, { 0,0 });
								string str = "";
								str = "Quick Menu";
								ImGui::PushFont(FONT_BTN);
								drawlist->AddText({ pos.x + size.x * 0.1f, pos.y + viewport->Size.y * 0.4f }, ImGui::GetColorU32({ 0.98f,0.768f,0.51f,1 }), str.c_str());
								ImGui::PopFont();

								drawlist->AddLine({ pos.x + size.x * 0.5f, size.y }, { pos.x + size.x * 0.5f, viewport->Size.y * 0.4f + 20.f }, ImGui::GetColorU32({ 1,1,1,1 }), 2.f);
								drawlist->AddLine({ pos.x + size.x * 0.5f, viewport->Size.y * 0.4f + 20.f }, { pos.x + size.x * 0.1f, viewport->Size.y * 0.4f + 20.f }, ImGui::GetColorU32({ 1,1,1,1 }), 2.f);

								str = "The quick menu allows you to quickly access all";
								drawlist->AddText({ pos.x + size.x * 0.1f, pos.y + viewport->Size.y * 0.4f + 25.f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());
								str = "the pages in the conflict resolution screen.";
								drawlist->AddText({ pos.x + size.x * 0.1f, pos.y + viewport->Size.y * 0.4f + 40.f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());
								str = string(ICON_FK_EXCLAMATION_CIRCLE) + "Click anywhere to continue.";
								drawlist->AddText({ pos.x + size.x * 0.1f, pos.y + viewport->Size.y * 0.4f + 70.f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());

								if (!m_waypointEmitter.expired())
									m_waypointEmitter.lock()->m_GM.m_active = false;

								if (NonImGuiMouseClick())
								{
									tutorial_index = 2;

									AudioEngine ae;
									if (!ae.IsPlaying(voice_line))
										voice_line = ae.Play("Sounds2D/Cr_Units_1.wav", "VL", 1.0f);
								}

							}
							break;
							case 2:
							{
								ImVec2 pos = { viewport->Size.x * 0.225f, viewport->Size.y * 0.085f };
								ImVec2 size = { 340.f, 140.f };
								UI::TutArea(pos, size);
								block_input_if_mouse_not_in_bounds(pos, size);
								string str = string(ICON_FK_EXCLAMATION_CIRCLE) + "Click here to access units page.";
								drawlist->AddText({ pos.x + size.x + 10.f, pos.y + size.y - 10.f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());
								/*if (UI::MouseIsWithin(pos, { pos.x + size.x, pos.y + size.y }) && ImGui::IsMouseDown(0))
									ImGui::GetIO().MouseClicked[0] = true;*/

								if (emitter_1_1 == false)
								{
									EmitterSystem_2D::GetInstance().CreateButtonEmitter(m_waypointEmitter, pos, size);
									emitter_1_1 = true;
								}
							}
							break;
							}

							UI::TutProgressBar(drawlist, ImVec2{ viewport->Size }, 1);
						}
						else if (instance.tutorial_slide && tut_openSlide)
						{
							tut_openSlide = false;
							Service<EventManager>::Get().instant_dispatch<TutorialPopupTrigger>(TUTORIAL_POPUP_TYPES::SIMULATE_TUT);
						}
						else if (instance.tutorial_slide == false)// Tutorial 1 Part 2
						{

							switch (tutorial_index)
							{
							case 0:
							{
								ImVec2 pos = { viewport->Size.x * 0.44f, viewport->Size.y * 0.18f };
								ImVec2 size = { 220.f, 150.f };
								UI::TutArea(pos, size);
								block_input_if_mouse_not_in_bounds(pos, size);
								string str = string(ICON_FK_EXCLAMATION_CIRCLE) + "Click here to select the sequence.";
								drawlist->AddText({ pos.x + size.x + 10.f, pos.y + size.y - 10.f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());

								if (emitter_2_0 == false)
								{
									EmitterSystem_2D::GetInstance().CreateButtonEmitter(m_waypointEmitter, pos, size);
									emitter_2_0 = true;
								}
							}
							break;

							case 1:
							{
								ImVec2 pos = { viewport->Size.x * 0.387f, viewport->Size.y * 0.275f };
								ImVec2 size = { 200.f, 80.f };
								UI::TutArea(pos, size);
								block_input_if_mouse_not_in_bounds(pos, size);
								string str = string(ICON_FK_EXCLAMATION_CIRCLE) + "Click here to select the sequence.";
								drawlist->AddText({ pos.x + size.x + 10.f, pos.y + size.y - 10.f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());

								if (sequence != UNDEFINED)
									tutorial_index = 2;

								if (emitter_2_1 == false)
								{
									EmitterSystem_2D::GetInstance().CreateButtonEmitter(m_waypointEmitter, pos, size);
									emitter_2_1 = true;
								}
							}
							break;

							case 2:
							{
								ImVec2 pos = { viewport->Size.x * 0.14f, viewport->Size.y * 0.41f };
								ImVec2 size = { 150.f, 150.f };
								UI::TutArea(pos, size);
								block_input_if_mouse_not_in_bounds(pos, size);
								string str = string(ICON_FK_EXCLAMATION_CIRCLE) + "Click here to select a unit.";
								drawlist->AddText({ pos.x + size.x + 10.f, pos.y + size.y - 10.f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());

								if (emitter_2_2 == false)
								{
									EmitterSystem_2D::GetInstance().CreateButtonEmitter(m_waypointEmitter, pos, size);
									emitter_2_2 = true;
								}
							}
							break;

							case 3:
							{
								ImVec2 pos = { viewport->Size.x * 0.1f, viewport->Size.y * 0.305f };
								ImVec2 size = { 150.f, 150.f };
								UI::TutArea(pos, size);
								block_input_if_mouse_not_in_bounds(pos, size);
								string str = string(ICON_FK_EXCLAMATION_CIRCLE) + "Click here to select a unit.";
								drawlist->AddText({ pos.x + size.x + 10.f, pos.y + size.y - 10.f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());

								if (attacker.unit_id != UNDEFINED)
									tutorial_index = 4;

								if (emitter_2_3 == false)
								{
									EmitterSystem_2D::GetInstance().CreateButtonEmitter(m_waypointEmitter, pos, size);
									emitter_2_3 = true;
								}
							}
							break;

							case 4:
							{
								ImVec2 pos = { viewport->Size.x * 0.133f, viewport->Size.y * 0.555f };
								ImVec2 size = { 200.f, 50.f };
								UI::TutArea(pos, size);
								block_input_if_mouse_not_in_bounds(pos, size);
								string str = string(ICON_FK_EXCLAMATION_CIRCLE) + "Click here to select a weapon.";
								drawlist->AddText({ pos.x + size.x + 10.f, pos.y + size.y - 10.f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());

								if (emitter_2_4 == false)
								{
									EmitterSystem_2D::GetInstance().CreateButtonEmitter(m_waypointEmitter, pos, size);
									emitter_2_4 = true;
								}
							}
							break;

							case 5:
							{
								ImVec2 pos = { viewport->Size.x * 0.085f, viewport->Size.y * 0.276f };
								ImVec2 size = { 200.f, 80.f };
								UI::TutArea(pos, size);
								block_input_if_mouse_not_in_bounds(pos, size);
								string str = string(ICON_FK_EXCLAMATION_CIRCLE) + "Click here to select a weapon.";
								drawlist->AddText({ pos.x + size.x + 10.f, pos.y + size.y - 10.f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());

								if (attacker.weapon != UNDEFINED)
									tutorial_index = 6;

								if (emitter_2_5 == false)
								{
									EmitterSystem_2D::GetInstance().CreateButtonEmitter(m_waypointEmitter, pos, size);
									emitter_2_5 = true;
								}
							}
							break;

							case 6:
							{
								ImVec2 pos = { viewport->Size.x * 0.133f, viewport->Size.y * 0.615f };
								ImVec2 size = { 200.f, 50.f };
								UI::TutArea(pos, size);
								block_input_if_mouse_not_in_bounds(pos, size);
								string str = string(ICON_FK_EXCLAMATION_CIRCLE) + "Click here to select an action.";
								drawlist->AddText({ pos.x + size.x + 10.f, pos.y + size.y - 10.f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());

								if (emitter_2_6 == false)
								{
									EmitterSystem_2D::GetInstance().CreateButtonEmitter(m_waypointEmitter, pos, size);
									emitter_2_6 = true;
								}
							}
							break;

							case 7:
							{
								ImVec2 pos = { viewport->Size.x * 0.085f, viewport->Size.y * 0.276f };
								ImVec2 size = { 200.f, 80.f };
								UI::TutArea(pos, size);
								block_input_if_mouse_not_in_bounds(pos, size);
								string str = string(ICON_FK_EXCLAMATION_CIRCLE) + "Click here to select an action.";
								drawlist->AddText({ pos.x + size.x + 10.f, pos.y + size.y - 10.f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());

								if (attacker.action != UNDEFINED)
									tutorial_index = 8;

								if (emitter_2_7 == false)
								{
									EmitterSystem_2D::GetInstance().CreateButtonEmitter(m_waypointEmitter, pos, size);
									emitter_2_7 = true;
								}
							}
							break;

							// moar tasks
							case 8:
							{
								if (!m_waypointEmitter.expired())
									m_waypointEmitter.lock()->m_GM.m_active = false;

								auto selected = tex_map["Assets/Selected.dds"];
								auto unselected = tex_map["Assets/Unselected.dds"];
								bool taskCompleted = true;
								ImVec2 pos = { viewport->Size.x * 0.3f, 0 };
								ImVec2 size = { viewport->Size.x, viewport->Size.y };
								UI::TutArea(pos, size);
								block_input_if_mouse_not_in_bounds(pos, size);
								//Task List
								string str = "";
								str = string(ICON_FK_EXCLAMATION_CIRCLE);
								ImGui::PushFont(FONT_HEAD);
								drawlist->AddText({ viewport->Size.x * 0.12f, viewport->Size.y * 0.4f }, ImGui::GetColorU32({ 1.f,1.f,1.f,1 }), str.c_str());
								str = " Tasks";
								drawlist->AddText({ viewport->Size.x * 0.12f + ImGui::GetFontSize(), viewport->Size.y * 0.4f }, ImGui::GetColorU32({ 0.98f,0.768f,0.51f,1 }), str.c_str());
								drawlist->AddLine({ 0.0f, viewport->Size.y * 0.4f + ImGui::GetFontSize() }, { viewport->Size.x * 0.2f, viewport->Size.y * 0.4f + ImGui::GetFontSize() }, ImGui::GetColorU32({ 1,1,1,1 }), 2.f);
								ImGui::PopFont();

								ImGui::PushFont(FONT_BODY);
								ImVec2 min = { viewport->Size.x * 0.17f, viewport->Size.y * 0.45f };
								str = "Select a unit for Defender";

								drawlist->AddText({ viewport->Size.x * 0.04f, min.y + (float)unselected->GetHeight() * 0.2f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());

								if (defender.unit_id != UNDEFINED)
								{
									drawlist->AddImage((void*)static_cast<size_t>(selected->GetID()), min, { min.x + (float)selected->GetWidth() * 0.6f, min.y + (float)selected->GetHeight() * 0.6f });
									taskCompleted &= true;

									if (!b_unit_select_defender_task_VFX)
									{
										b_unit_select_defender_task_VFX = true;
										ImVec2 max_VFX{ min.x + (float)selected->GetWidth() * 0.6f, min.y + (float)selected->GetHeight() * 0.6f };
										EmitterSystem_2D::GetInstance().CreateExplosionEmitter(m_explosion_VFX, (min + max_VFX) * 0.5f);
										m_explosion_VFX.lock()->m_PAM.m_colourBegin = glm::vec4{ 250.f / 255.f, 250.f / 255.f, 210.f / 255.f, 1.0f };
										m_explosion_VFX.lock()->m_PAM.m_colourEnd = glm::vec4{ 250.f / 255.f, 250.f / 255.f, 210.f / 255.f, 1.0f };
									}
								}
								else
								{
									drawlist->AddImage((void*)static_cast<size_t>(unselected->GetID()), min, { min.x + (float)unselected->GetWidth() * 0.6f, min.y + (float)unselected->GetHeight() * 0.6f });
									taskCompleted &= false;
									b_unit_select_defender_task_VFX = false;
								}

								min = { min.x, min.y + unselected->GetWidth() * 0.9f };
								str = "Select a weapon for Defender";
								drawlist->AddText({ viewport->Size.x * 0.04f,  min.y + (float)unselected->GetHeight() * 0.2f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());

								if (defender.weapon != UNDEFINED)
								{
									drawlist->AddImage((void*)static_cast<size_t>(selected->GetID()), min, { min.x + (float)selected->GetWidth() * 0.6f, min.y + (float)selected->GetHeight() * 0.6f });
									taskCompleted &= true;

									if (!b_unit_select_weapon_task_VFX)
									{
										b_unit_select_weapon_task_VFX = true;
										ImVec2 max_VFX{ min.x + (float)selected->GetWidth() * 0.6f, min.y + (float)selected->GetHeight() * 0.6f };
										EmitterSystem_2D::GetInstance().CreateExplosionEmitter(m_explosion_VFX, (min + max_VFX) * 0.5f);
									}
								}
								else
								{
									drawlist->AddImage((void*)static_cast<size_t>(unselected->GetID()), min, { min.x + (float)unselected->GetWidth() * 0.6f, min.y + (float)unselected->GetHeight() * 0.6f });
									taskCompleted &= false;
									b_unit_select_weapon_task_VFX = false;
								}

								min = { min.x, min.y + unselected->GetWidth() * 0.9f };
								str = "Select an action for Defender";
								drawlist->AddText({ viewport->Size.x * 0.04f,  min.y + (float)unselected->GetHeight() * 0.2f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());

								if (defender.action != UNDEFINED)
								{
									drawlist->AddImage((void*)static_cast<size_t>(selected->GetID()), min, { min.x + (float)selected->GetWidth() * 0.6f, min.y + (float)selected->GetHeight() * 0.6f });
									taskCompleted &= true;

									if (!b_unit_select_action_task_VFX)
									{
										b_unit_select_action_task_VFX = true;
										ImVec2 max_VFX{ min.x + (float)selected->GetWidth() * 0.6f, min.y + (float)selected->GetHeight() * 0.6f };
										EmitterSystem_2D::GetInstance().CreateExplosionEmitter(m_explosion_VFX, (min + max_VFX) * 0.5f);
									}
								}
								else
								{
									drawlist->AddImage((void*)static_cast<size_t>(unselected->GetID()), min, { min.x + (float)unselected->GetWidth() * 0.6f, min.y + (float)unselected->GetHeight() * 0.6f });
									taskCompleted &= false;
									b_unit_select_action_task_VFX = false;
								}
								ImGui::PopFont();

								auto nextBtn = tex_map["Assets/NextBtn.dds"];
								ImVec2 tut_min = { viewport->Size.x * 0.1f, min.y + unselected->GetWidth() * 2.f };
								ImVec2 tut_max = { tut_min.x + nextBtn->GetWidth() * 1.f, tut_min.y + nextBtn->GetHeight() * 1.f };

								if (taskCompleted)
								{
									drawlist->AddImage((void*)static_cast<size_t>(nextBtn->GetID()), tut_min, tut_max);
									override_mouse_blocking(tut_min, tut_max - tut_min);
									if (UI::MouseIsWithin(tut_min, tut_max))
									{
										ImGui::SetMouseCursor(7);
										if (ImGui::IsMouseClicked(0))
											tutorial_index = 9;
									}
								}
								else
									drawlist->AddImage((void*)static_cast<size_t>(nextBtn->GetID()), tut_min, tut_max, { 0,0 }, { 1,1 }, ImGui::GetColorU32({ 1,1,1,0.4f }));
							}
							break;

							case 9:
							{
								ImVec2 pos = { viewport->Size.x * 0.445f, viewport->Size.y * 0.63f };
								ImVec2 size = { 220.f, 120.f };
								UI::TutArea(pos, size);
								block_input_if_mouse_not_in_bounds(pos, size);
								string str = string(ICON_FK_EXCLAMATION_CIRCLE) + "Click here to simulate the results.";
								drawlist->AddText({ pos.x + size.x + 10.f, pos.y + size.y - 10.f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());
								ImGui::PushFont(FONT_BTN);
								str = "Set Frequency";
								drawlist->AddText({ viewport->Size.x * 0.8f - ImGui::CalcTextSize(str.c_str()).x, pos.y + size.y * 0.3f - 20.f }, ImGui::GetColorU32({ 0.98f,0.768f,0.51f,1 }), str.c_str());
								ImGui::PopFont();
								drawlist->AddLine({ pos.x + size.x, pos.y + size.y * 0.3f }, { viewport->Size.x * 0.8f, pos.y + size.y * 0.3f }, ImGui::GetColorU32({ 1,1,1,1 }), 2.f);
								str = "Set the frequency of the simulations here.";
								ImVec2 strPos = { viewport->Size.x * 0.8f - ImGui::CalcTextSize(str.c_str()).x, pos.y + size.y * 0.3f + ImGui::CalcTextSize(str.c_str()).y - 10.f };
								drawlist->AddText(strPos, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());
								str = "A higher number will give a more accurate result.";
								strPos.x = viewport->Size.x * 0.8f - ImGui::CalcTextSize(str.c_str()).x;
								strPos.y += ImGui::CalcTextSize(str.c_str()).y;
								drawlist->AddText(strPos, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());
							}
							break;

							case 10:
							{
								ImVec2 pos = { viewport->Size.x * 0.3655f, viewport->Size.y * 0.35f };
								ImVec2 size = { 500.f, 280.f };
								UI::TutArea(pos, size);
								block_input_if_mouse_not_in_bounds(pos, size);
								drawlist->AddLine({ pos.x + size.x, pos.y + size.y * 0.4f }, { viewport->Size.x * 0.8f, pos.y + size.y * 0.4f }, ImGui::GetColorU32({ 1,1,1,1 }), 2.f);
								ImGui::PushFont(FONT_BTN);
								string str = "Chance of Success";
								drawlist->AddText({ viewport->Size.x * 0.8f - ImGui::CalcTextSize(str.c_str()).x, pos.y + size.y * 0.4f - 20.f }, ImGui::GetColorU32({ 0.98f,0.768f,0.51f,1 }), str.c_str());
								ImGui::PopFont();
								drawlist->AddLine({ pos.x + size.x, pos.y + size.y * 0.4f }, { viewport->Size.x * 0.8f, pos.y + size.y * 0.4f }, ImGui::GetColorU32({ 1,1,1,1 }), 2.f);
								str = "Success rate of the simulation resuls will";
								ImVec2 strPos = { viewport->Size.x * 0.8f - ImGui::CalcTextSize(str.c_str()).x, pos.y + size.y * 0.4f + ImGui::CalcTextSize(str.c_str()).y - 10.f };
								drawlist->AddText(strPos, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());
								str = "be shown here.";
								strPos.y += ImGui::CalcTextSize(str.c_str()).y;
								drawlist->AddText(strPos, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());

								auto nextBtn = tex_map["Assets/NextBtn.dds"];
								ImVec2 tut_min = { viewport->Size.x * 0.7f,viewport->Size.y * 0.55f };
								ImVec2 tut_max = { tut_min.x + nextBtn->GetWidth() * 1.f, tut_min.y + nextBtn->GetHeight() * 1.f };

								drawlist->AddImage((void*)static_cast<size_t>(nextBtn->GetID()), tut_min, tut_max);
								override_mouse_blocking(tut_min, tut_max - tut_min);
								if (UI::MouseIsWithin(tut_min, tut_max))
								{
									ImGui::SetMouseCursor(7);
									if (ImGui::IsMouseClicked(0))
										tutorial_index = 11;
								}

								UI::TutProgressBar(drawlist, ImVec2{ viewport->Size }, 6);
							}
							break;

							case 11:
							{
								UI::TutProgressBar(drawlist, ImVec2{ viewport->Size }, 6);

								UI::TutArea({ 0,0 }, { 0,0 }, false);

								// render the complete level here
								ImGui::PushFont(FONT_CONTAX_144);
								string str = "Level 1";
								drawlist->AddText({ viewport->Size.x * 0.4f,viewport->Size.y * 0.3f }, ImGui::GetColorU32({ 0.98f,0.768f,0.51f,1 }), str.c_str());
								drawlist->AddLine({ viewport->Size.x * 0.2f,viewport->Size.y * 0.42f }, { viewport->Size.x * 0.8f,viewport->Size.y * 0.42f }, ImGui::GetColorU32({ 1,1,1,1 }));
								str = "COMPLETE!";
								drawlist->AddText({ viewport->Size.x * 0.34f,viewport->Size.y * 0.45f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());
								ImGui::PopFont();

								auto nextBtn = tex_map["Assets/NextBtn.dds"];
								ImVec2 tut_min = { viewport->Size.x * 0.85f,viewport->Size.y * 0.85f };
								ImVec2 tut_max = { tut_min.x + nextBtn->GetWidth() * 1.f, tut_min.y + nextBtn->GetHeight() * 1.f };

								//Text Above Next button
								ImGui::PushFont(FONT_BTN);
								str = "Next up - ";
								drawlist->AddText({ tut_min.x, tut_min.y - 20.f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());
								auto strLen = ImGui::CalcTextSize(str.c_str());
								str = "Level 2";
								drawlist->AddText({ tut_min.x + strLen.x, tut_min.y - 20.f }, ImGui::GetColorU32({ 0.98f,0.768f,0.51f,1 }), str.c_str());
								ImGui::PopFont();

								drawlist->AddImage((void*)static_cast<size_t>(nextBtn->GetID()), tut_min, tut_max);
								override_mouse_blocking(tut_min, tut_max - tut_min);
								if (UI::MouseIsWithin(tut_min, tut_max))
								{
									ImGui::SetMouseCursor(7);
									if (ImGui::IsMouseClicked(0))
									{
										tutorial_index = 0;
										tutorial_p2 = false;
										instance.tutorial_level = 2;

										if (!m_circularMotionEmitter.expired())
											m_circularMotionEmitter.lock()->m_GM.m_active = false;
									}
								}

								if (emitter_C_0 == false)
								{
									emitter_C_0 = true;
									EmitterSystem_2D::GetInstance().CreateCircularMotionEmitter_2D(m_circularMotionEmitter, glm::vec2{ 980.0f, 450.0f }, 300.0f);
									auto sm = m_circularMotionEmitter.lock();
									sm->m_PAM.m_scaleBegin = 30.0f;
									sm->m_PAM.m_scaleEnd = 30.0f;

								}
							}
							break;

							default:
								break;
							}

							if (tutorial_index < 10)
								UI::TutProgressBar(drawlist, ImVec2{ viewport->Size }, 5);
						}
					}

					else if (instance.tutorial_level == 2)
					{
						switch (tutorial_index)
						{
						case 0:
							UI::TutProgressBar2(drawlist, ImVec2{ viewport->Size }, instance);
							break;

						case 1:
						{
							UI::TutProgressBar2(drawlist, ImVec2{ viewport->Size }, instance, true);
							UI::TutArea({ 0,0 }, { 0,0 }, false);

							// render the complete level here
							ImGui::PushFont(FONT_CONTAX_144);
							string str = "Level 2";
							drawlist->AddText({ viewport->Size.x * 0.4f,viewport->Size.y * 0.3f }, ImGui::GetColorU32({ 0.98f,0.768f,0.51f,1 }), str.c_str());
							drawlist->AddLine({ viewport->Size.x * 0.2f,viewport->Size.y * 0.42f }, { viewport->Size.x * 0.8f,viewport->Size.y * 0.42f }, ImGui::GetColorU32({ 1,1,1,1 }));
							str = "COMPLETE!";
							drawlist->AddText({ viewport->Size.x * 0.34f,viewport->Size.y * 0.45f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());
							ImGui::PopFont();

							auto nextBtn = tex_map["Assets/NextBtn.dds"];
							ImVec2 tut_min = { viewport->Size.x * 0.85f,viewport->Size.y * 0.85f };
							ImVec2 tut_max = { tut_min.x + nextBtn->GetWidth() * 1.f, tut_min.y + nextBtn->GetHeight() * 1.f };

							ImGui::PushFont(FONT_BTN);
							str = "Next up - ";
							drawlist->AddText({ tut_min.x, tut_min.y - 20.f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());
							auto strLen = ImGui::CalcTextSize(str.c_str());
							str = "Level 3";
							drawlist->AddText({ tut_min.x + strLen.x, tut_min.y - 20.f }, ImGui::GetColorU32({ 0.98f,0.768f,0.51f,1 }), str.c_str());
							ImGui::PopFont();

							drawlist->AddImage((void*)static_cast<size_t>(nextBtn->GetID()), tut_min, tut_max);
							override_mouse_blocking(tut_min, tut_max - tut_min);
							if (UI::MouseIsWithin(tut_min, tut_max))
							{
								ImGui::SetMouseCursor(7);
								if (ImGui::IsMouseClicked(0))
								{
									tutorial_index = 0;
									instance.tutorial_level = 3;
									if (!m_circularMotionEmitter.expired())
										m_circularMotionEmitter.lock()->m_GM.m_active = false;
								}
							}

							if (emitter_C_0 == false)
							{
								emitter_C_0 = true;
								EmitterSystem_2D::GetInstance().CreateCircularMotionEmitter_2D(m_circularMotionEmitter, glm::vec2{ 980.0f, 450.0f }, 300.0f);
							}
						}
						break;

						default:
							break;
						}
					}

					else if (instance.tutorial_level == 3)
					{
						switch (tutorial_index)
						{
						case 0:
						{
							UI::TutProgressBar3(drawlist, ImVec2{ viewport->Size }, 1);

							if (win + lose)
							{
								float result = 0.0f;
								result = 100.f * win / (win + lose);

								if (result >= 50.f && result <= 60.f)
									tutorial_index = 1;
							}
						}
						break;

						case 1:
						{
							UI::TutProgressBar3(drawlist, ImVec2{ viewport->Size }, 2);
							UI::TutArea({ 0,0 }, { 0,0 }, false);

							// render the complete level here
							ImGui::PushFont(FONT_CONTAX_144);
							string str = "Level 3";
							drawlist->AddText({ viewport->Size.x * 0.4f,viewport->Size.y * 0.3f }, ImGui::GetColorU32({ 0.98f,0.768f,0.51f,1 }), str.c_str());
							drawlist->AddLine({ viewport->Size.x * 0.2f,viewport->Size.y * 0.42f }, { viewport->Size.x * 0.8f,viewport->Size.y * 0.42f }, ImGui::GetColorU32({ 1,1,1,1 }));
							str = "COMPLETE!";
							drawlist->AddText({ viewport->Size.x * 0.34f,viewport->Size.y * 0.45f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());
							ImGui::PopFont();

							auto nextBtn = tex_map["Assets/NextBtn.dds"];
							ImVec2 tut_min = { viewport->Size.x * 0.85f,viewport->Size.y * 0.85f };
							ImVec2 tut_max = { tut_min.x + nextBtn->GetWidth() * 1.f, tut_min.y + nextBtn->GetHeight() * 1.f };

							drawlist->AddImage((void*)static_cast<size_t>(nextBtn->GetID()), tut_min, tut_max);
							override_mouse_blocking(tut_min, tut_max - tut_min);
							if (UI::MouseIsWithin(tut_min, tut_max))
							{
								ImGui::SetMouseCursor(7);
								if (ImGui::IsMouseClicked(0))
								{
									tutorial_index = 0;
									//	instance.tutorial_level = 1;
									//	instance.tutorial_enable = false;
									instance.tutorial_level = 4;
									if (!m_circularMotionEmitter.expired())
										m_circularMotionEmitter.lock()->m_GM.m_active = false;
								}
							}

							if (emitter_C_0 == false)
							{
								emitter_C_0 = true;
								EmitterSystem_2D::GetInstance().CreateCircularMotionEmitter_2D(m_circularMotionEmitter, glm::vec2{ 980.0f, 450.0f }, 300.0f);
							}
						}
						break;
						default:
							break;
						}

					}

					//Tutorial Exit Button
					if (instance.tutorial_slide == false)
					{
						auto exitBtn = tex_map["Assets/Tutorial_exit.dds"];
						ImVec2 tut_min = { viewport->Size.x * 0.85f, viewport->Size.y * 0.05f };
						ImVec2 tut_max = { tut_min.x + exitBtn->GetWidth() * 1.f, tut_min.y + exitBtn->GetHeight() * 1.f };
						drawlist->AddImage((void*)static_cast<size_t>(exitBtn->GetID()), tut_min, tut_max);

						override_mouse_blocking(tut_min, tut_max - tut_min);
						if (UI::MouseIsWithin(tut_min, tut_max))
						{
							ImGui::SetMouseCursor(7);
							if (ImGui::IsMouseClicked(0))
							{
								instance.tutorial_temp_exit = true;
								ImGui::OpenPopup("TutorialExitPopupConfirm");
							}
						}
					}

				}

				// exit tutorial
				if (UI::ConfirmTutorialPopup("TutorialExitPopupConfirm", "Do you want to exit the tutorial?", true, [&]() {instance.tutorial_temp_exit = false; }))
				{
					AudioEngine ae;
					ae.StopChannel(voice_line);
					voice_line = 0;

					instance.tutorial_temp_exit = false;
					instance.tutorial_enable = false;

					if (!m_circularMotionEmitter.expired())
						m_circularMotionEmitter.lock()->m_GM.m_active = false;

					if (!m_waypointEmitter.expired())
						m_waypointEmitter.lock()->m_GM.m_active = false;
				}
				auto tex = tex_map["Assets/SimulationBG.dds"];
				{
					ImVec2 Min{ 0,0 };
					ImVec2 Max{ Min.x + viewport->Size.x, Min.y + viewport->Size.y };
					ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(tex->GetID()), Min, Max);
				}

				// title
				ImGui::SetCursorPos(ImVec2{ 0,0 });
				ImGui::Dummy(ImVec2{ 0.f, ImGui::GetContentRegionAvail().y * 0.05f });
				UI::SubHeader("Simulate");
				ImGui::Dummy(ImVec2{ 0.f, ImGui::GetContentRegionAvail().y * 0.05f });

				// sequence display
				ImGui::PushFont(FONT_SHEAD);
				const std::string seq_title{ "Sequence" };
				ImGui::SetCursorPos({ viewport->Size.x * 0.5f - (ImGui::CalcTextSize(seq_title.c_str()).x + ImGui::GetFontSize() * 0.5f) * 0.5f, viewport->Size.y * 0.2f });
				ImGui::Text(seq_title.c_str());
				ImGui::PopFont();

				// display chosen sequence
				std::string seq_name{ "SELECT SEQUENCE" };
				if (sequence != UNDEFINED)
					seq_name = instance.ecs.get<tc::Graph>(sequence).g.name;

				if (UI::UIButton_Sequence(instance, sequence, seq_name.c_str(), seq_name.c_str(), { viewport->Size.x * 0.5f, viewport->Size.x * 0.15f }, { 0,0 }, FONT_PARA))
				{
					if (instance.tutorial_enable && tutorial_p2)
						tutorial_index = 1;

					Service<EventManager>::Get().instant_dispatch<SimulatePopupTrigger>(
						SIMULATE_POPUP_TYPE::SEQUENCE, false, sequence);
				}
				push_button_style();
				ImGui::SameLine();
				ImGui::SetCursorPos(ImVec2{ ImGui::GetCursorPosX() + 115.f, ImGui::GetCursorPosY() - 25.0f });

				//	ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.55f, viewport->Size.y * 0.25f });
				ImGui::PushID("seq");
				if (sequence != UNDEFINED && UI::UIImageButton((void*)static_cast<size_t>(enter_button->GetID()), ImVec2{ enter_button->GetWidth() * 1.0f, enter_button->GetHeight() * 1.0f }))
				{
					Service<EventManager>::Get().instant_dispatch<CloseOverlayTrigger>(QUICKMENU_POPUP_TYPE::SIMULATE);
					Service<EventManager>::Get().instant_dispatch<OpenGraphTrigger>(sequence, instance, OPEN_GRAPH_TYPE::GRAPH_SEQUENCE);
				}
				ImGui::PopID();
				pop_button_style();

				// success title
				tex = tex_map["Assets/CoSTitle.dds"];
				ImGui::SetCursorPos({ viewport->Size.x * 0.5f - tex->GetWidth() * 0.5f, viewport->Size.y * 0.4f - tex->GetHeight() * 0.5f });
				ImGui::Image((void*)static_cast<size_t>(tex->GetID()), ImVec2((float)tex->GetWidth(), (float)tex->GetHeight()));

				// display number percentage
				float result = 0.0f;
				ImU32 color = IM_COL32(232, 137, 64, 255); // orange
				// display number percentage
				if (win + lose)
				{
					result = 100.f * win / (win + lose);

					if (result >= 75.f)
						color = IM_COL32(141, 218, 50, 255); // green
					else if (result <= 25.f)
						color = IM_COL32(202, 56, 56, 255); // red

					ImGui::SetCursorPos({ viewport->Size.x * 0.465f, viewport->Size.y * 0.55f });
					ImGui::PushFont(FONT_HEAD);
					ImGui::PushStyleColor(ImGuiCol_Text, color);
					ImGui::Text("%.1f %%", result);
					ImGui::PopStyleColor();
					ImGui::PopFont();
				}

				// render progress bar
				ImGui::SetCursorPos({ viewport->Size.x * 0.5f - 350.0f * 0.5f, viewport->Size.y * 0.45f });
				ImGui::PushStyleColor(ImGuiCol_PlotHistogram, color);
				ImGui::ProgressBar(result * 0.01f, { 350.0f, 50.0f });
				ImGui::PopStyleColor();

				ImGui::SetCursorPos({ viewport->Size.x * 0.45f, viewport->Size.y * 0.65f });
				ImGui::PushFont(FONT_PARA);
				ImGui::Text("Frequency ");
				ImGui::PopFont();
				ImGui::SameLine();
				ImGui::PushItemWidth(100.f);
				ImGui::InputScalar("", ImGuiDataType_U32, &freq, 0);
				freq = std::clamp(freq, 100u, 50000u);
				ImGui::PopItemWidth();

				// attack section
				display_unit_section(instance, { viewport->Size.x * 0.18f,viewport->Size.y * 0.5f }, true);
				display_unit_section(instance, { viewport->Size.x * (1.f - 0.18f),viewport->Size.y * 0.5f }, false);

				if (UI::UIButton_Simulate("Simulate", "Simulate", { viewport->Size.x * 0.5f, viewport->Size.y * 0.72f }, { -15.f, 6.f }, FONT_BODY, true))
				{
					if (instance.tutorial_enable && tutorial_p2 && tutorial_index == 9)
						tutorial_index = 10;

					// TODO: check for invalid inputs
					freq = std::clamp(freq, 1u, 100000u);

					// equip your stuff here
					attacker.unit_id;
					defender.unit_id;

					if (auto character = instance.ecs.get_if<tc::Character>(attacker.unit_id))
					{
						character->chosen_weapon = attacker.weapon;
					}
					if (auto character = instance.ecs.get_if<tc::Character>(defender.unit_id))
					{
						character->chosen_weapon = defender.weapon;
					}

					Service<EventManager>::Get().instant_dispatch<SimulateConflict>(attacker.unit_id, defender.unit_id, attacker.action, defender.action, sequence, freq, win, lose, attack, defend, finish);

					if (instance.tutorial_enable && instance.tutorial_level == 2 &&
						sequence != UNDEFINED && attacker.unit_id != UNDEFINED && defender.unit_id != UNDEFINED)
					{
						Service<EventManager>::Get().instant_dispatch<DelayTrigger>(3.0f, [&]() {tutorial_index = 1; });
					}
				}


				// if (UI::UIButton_CustomMap("Custom Map", "Custom Map", { viewport->Size.x * 0.57f, viewport->Size.y * 0.72f }, { -15.f, 6.f }, FONT_BODY,true))
				// {
				// 	// open testing combat in map
				// 	auto& edit = dynamic_cast<EditTimeInstance&>(instance);
				// 	edit.save();

				// 	if (instance.ecs.view_first<tc::Character>() && instance.ecs.view_first<tc::ConflictGraph>())
				// 	{
				// 		Service<EventManager>::Get().instant_dispatch<OpenMainMenuTrigger>(5);
				// 		OverlayOpen = false;
				// 	}
				// 	else if (!instance.ecs.view_first<tc::Character>() && !instance.ecs.view_first<tc::ConflictGraph>())
				// 	{
				// 		Service<EventManager>::Get().instant_dispatch<ErrorTrigger>("No existing Unit or Sequence found!");
				// 	}
				// 	else if (!instance.ecs.view_first<tc::ConflictGraph>())
				// 	{
				// 		Service<EventManager>::Get().instant_dispatch<ErrorTrigger>("No existing Sequence found!");
				// 	}
				// 	else
				// 	{
				// 		Service<EventManager>::Get().instant_dispatch<ErrorTrigger>("No existing Unit found!");
				// 	}

				// 	/*Service<EventManager>::Get().instant_dispatch<LoadNewInstance>(
				// 		edit.get_full_path(),
				// 		MemoryStrategy{},
				// 		InstanceType::RUN_TIME);*/
				// }

				// display top buttons
				{
					ImGui::SetCursorPos(ImVec2{ viewport->Size.x * inter.get(),viewport->Size.y * 0.03f });

					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0,0,0,0 });
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0,0,0,0 });
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0,0,0,0 });
					tex = tex_map["Assets/BackMenuBtn.dds"];

					if (UI::UIImageButton((void*)static_cast<size_t>(tex->GetID()), ImVec2{ tex->GetWidth() * 0.7f, tex->GetHeight() * 0.7f }, { 0,0 }, { 1,1 }, 0, { 0,0,0,0 }, btnTintHover, btnTintPressed))
					{
						if (auto edit_instance = dynamic_cast<EditTimeInstance*>(&instance))
						{
							// have to put here cuz instance is gonna be assessing entities that are unloaded
							attacker.Clear();
							defender.Clear();
							sequence = UNDEFINED;

							// move back to select conflict res
							Service<EventManager>::Get().instant_dispatch<BottomRightOverlayTrigger>("Saving...");
							edit_instance->save_current_conflict_resolution();
							instance.unload_current_conflict_resolution();
						}

						auto fn = [&]()
						{
							OverlayOpen = false;
							Service<EventManager>::Get().instant_dispatch<OpenMainMenuTrigger>(4);
							Service<EventManager>::Get().instant_dispatch<CloseAllConResOverlayTrigger>();
						};

						Service<EventManager>::Get().instant_dispatch<WipeTrigger>(.15f, .15f, 0.f, fn);

					}
					ImGui::SameLine();
					ImGui::Dummy(ImVec2{ 10.0f, 0.0f });
					ImGui::SameLine();
					auto quickMenuPos = ImGui::GetCursorPos();
					tex = tex_map["Assets/QuickMenuBtn.dds"];

					if (UI::UIImageButton((void*)static_cast<size_t>(tex->GetID()), ImVec2{ tex->GetWidth() * 0.7f, tex->GetHeight() * 0.7f }, { 0,0 }, { 1,1 }, 0, { 0,0,0,0 }, btnTintHover, btnTintPressed))
					{
						Service<EventManager>::Get().instant_dispatch<QuickMenuPopupTrigger>(QUICKMENU_POPUP_TYPE::SIMULATE);

						//Tutorial progression
						if (instance.tutorial_enable && instance.tutorial_level == 1 && !tutorial_p2 && tutorial_index == 0)
						{
							tutorial_index = 1;

							AudioEngine ae;
							if (!ae.IsPlaying(voice_line))
								voice_line = ae.Play("Sounds2D/Cr_QuickMenu_1.wav", "VL", 1.0f);
						}

					}
					if (instance.tutorial_enable && tutorial_index == 0 && inter.is_finished() && !tutorial_p2 && instance.tutorial_level == 1)
					{
						if (emitter_1_0 == false)
						{
							emitter_1_0 = true;

							glm::vec2 real_buttonSize;
							real_buttonSize.x = tex->GetWidth() * 0.7f;
							real_buttonSize.y = tex->GetHeight() * 0.7f;

							glm::vec2 real_mousePosition;
							real_mousePosition.x = quickMenuPos.x;
							real_mousePosition.y = quickMenuPos.y;

							//real_mousePosition.x = 500.0f;
							//real_mousePosition.y = 600.0f;

							EmitterSystem_2D::GetInstance().CreateButtonEmitter(m_waypointEmitter, real_mousePosition, real_buttonSize);
							/*if (!m_circularMotionEmitter)
								m_circularMotionEmitter = EmitterSystem_2D::GetInstance().CreateCircularMotionEmitter_2D(real_mousePosition, 100.0f);*/
						}
					}

					ImGui::SameLine();
					ImGui::Dummy(ImVec2{ 10.0f, 0.0f });
					ImGui::SameLine();

					tex = tex_map["Assets/TutorialBtn.dds"];

					if (UI::UIImageButton((void*)static_cast<size_t>(tex->GetID()), ImVec2{ tex->GetWidth() * 0.7f, tex->GetHeight() * 0.7f }, { 0,0 }, { 1,1 }, 0, { 0,0,0,0 }, btnTintHover, btnTintPressed))
					{
						Service<EventManager>::Get().instant_dispatch<TutorialPopupTrigger>(TUTORIAL_POPUP_TYPES::SIMULATE_TUT);
					}
					ImGui::PopStyleColor(3);
				}

				/*	if (instance.tutorial_temp_enable && instance.ecs.view<tc::Graph>(exclude_t<tc::Destroyed>()).size_hint() <= 0 &&
					instance.ecs.view<tc::Unit>(exclude_t<tc::Destroyed>()).size_hint() <= 0 &&
					instance.ecs.view<tc::Weapon>(exclude_t<tc::Destroyed>()).size_hint() <= 0)
					ImGui::OpenPopup("TutorialPopupConfirm");
				if (!instance.tutorial_enable && UI::ConfirmTutorialPopup("TutorialPopupConfirm", "Do you want a tutorial?", false, []() {}))
				{
					instance.tutorial_enable = true;
					instance.tutorial_level = 1;
					tutorial_p2 = false;
				}
				else
				{
					instance.tutorial_temp_enable = false;
				}*/
			}

			ImGui::PopStyleVar();

			ImGui::End();

			if (!ImGui::GetHoveredID())
				HoveredID = 0;
			else if (HoveredID != ImGui::GetHoveredID())
			{
				AudioEngine ae;
				ae.Play("Sounds2D/Button_Highlight.wav", "SFX", 0.8f);
				HoveredID = ImGui::GetHoveredID();
			}
		}

		if (!m_waypointEmitter.expired() && (!OverlayOpen || !instance.tutorial_enable))
		{
			m_waypointEmitter.lock()->m_GM.m_active = false;
		}
	}
	void SimulateOverlay::display_unit_section(Instance& instance, const ImVec2 start_pos, bool is_attacker)
	{
		Entity temp = is_attacker ? attacker.unit_id : defender.unit_id;
		ImVec4 tint{ 1,1,1,1 };

		// character display
		float offset1 = inter_nest[0].get();
		if (!is_attacker) offset1 = -offset1;

		ImGui::SetCursorPos(ImVec2{ start_pos.x - 35.0f + offset1, start_pos.y - 60.0f });
		auto tex = tex_map["Assets/CharacterIcon.dds"];
		std::string chara_name{ "CHARACTER" };
		if (temp != UNDEFINED)
		{
			auto charac = instance.ecs.get<tc::Character>(temp);
			chara_name = charac.name;
			tint = ImVec4{ charac.color.x, charac.color.y,charac.color.z,1 };
		}


		if (UI::UICharButton_Toggle((void*)static_cast<size_t>(tex->GetID()), ImVec2{ tex->GetWidth() * 0.7f, tex->GetHeight() * 0.7f },
			chara_name, "##charaname" + is_attacker, temp != UNDEFINED, { 0, 0 }, { 1,1 }, 2, ImVec4{ 0,0,0,0 }, tint))
		{
			if (instance.tutorial_enable && tutorial_p2 && is_attacker)
				tutorial_index = 3;
			Service<EventManager>::Get().instant_dispatch<SimulatePopupTrigger>(
				SIMULATE_POPUP_TYPE::UNIT, is_attacker, temp);
		}


		ImGui::SameLine();
		ImGui::SetCursorPos(ImVec2{ ImGui::GetCursorPosX() - 15.0f, ImGui::GetCursorPosY() + 15.0f });
		push_button_style();
		ImGui::PushID("chara" + is_attacker);
		if (temp != UNDEFINED && UI::UIImageButton((void*)static_cast<size_t>(enter_button->GetID()), ImVec2{ enter_button->GetWidth() * 1.0f, enter_button->GetHeight() * 1.0f }))
		{
			Service<EventManager>::Get().instant_dispatch<SimulatePopupTrigger>(
				SIMULATE_POPUP_TYPE::EDIT_UNIT, is_attacker, temp);
		}
		ImGui::PopID();
		pop_button_style();

		// weapon

		float offset2 = inter_nest[1].get();
		if (!is_attacker) offset2 = -offset2;

		temp = is_attacker ? attacker.weapon : defender.weapon;
		ImGui::SetCursorPos({ start_pos.x + offset2, start_pos.y + padding * 4.0f });
		if (UI::UIButton_Weapon(instance, temp, "NO WEAPON", "NO WEAPON", ImGui::GetCursorPos(), { 0,0 }, FONT_PARA))
		{
			if (instance.tutorial_enable && tutorial_p2 && is_attacker)
				tutorial_index = 5;
			Service<EventManager>::Get().instant_dispatch<SimulatePopupTrigger>(
				SIMULATE_POPUP_TYPE::WEAPON, is_attacker, temp);
		}
		/*ImGui::SameLine();
		ImGui::Dummy({ 80.f,0.f });
		ImGui::SameLine();*/
		ImGui::SetCursorPos(ImVec2{ ImGui::GetCursorPosX() + 100.f, ImGui::GetCursorPosY() - 30.f });
		push_button_style();
		ImGui::PushID("weapon" + is_attacker);
		if (temp != UNDEFINED && UI::UIImageButton((void*)static_cast<size_t>(enter_button->GetID()), ImVec2{ enter_button->GetWidth() * 1.0f, enter_button->GetHeight() * 1.0f }))
		{
			Service<EventManager>::Get().instant_dispatch<SimulatePopupTrigger>(
				SIMULATE_POPUP_TYPE::EDIT_WEAPON, is_attacker, temp);
		}
		ImGui::PopID();
		pop_button_style();


		// action

		float offset3 = inter_nest[2].get();
		if (!is_attacker) offset3 = -offset3;


		temp = is_attacker ? attacker.action : defender.action;
		ImGui::SetCursorPos({ start_pos.x + offset3, start_pos.y + padding * 7.0f });
		ImGui::PushID("action" + is_attacker);
		if (UI::UIButton_Action(instance, temp, "NO ACTION", "NO ACTION", ImGui::GetCursorPos(), { 0,0 }, FONT_PARA))
		{
			if (instance.tutorial_enable && tutorial_p2 && is_attacker)
				tutorial_index = 7;
			Service<EventManager>::Get().instant_dispatch<SimulatePopupTrigger>(
				SIMULATE_POPUP_TYPE::ACTION, is_attacker, temp);
		}
		ImGui::PopID();
		ImGui::SetCursorPos(ImVec2{ ImGui::GetCursorPosX() + 100.f, ImGui::GetCursorPosY() - 30.f });
		push_button_style();
		if (temp != UNDEFINED && UI::UIImageButton((void*)static_cast<size_t>(enter_button->GetID()), ImVec2{ enter_button->GetWidth() * 1.0f, enter_button->GetHeight() * 1.0f }))
		{
			OverlayOpen = false;
			Service<EventManager>::Get().instant_dispatch<CloseOverlayTrigger>(QUICKMENU_POPUP_TYPE::SIMULATE);
			Service<EventManager>::Get().instant_dispatch<OpenGraphTrigger>(temp, instance, OPEN_GRAPH_TYPE::GRAPH_ACTION);
		}
		pop_button_style();
	}

	void SimulateOverlay::push_button_style() const
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0,0,0,0 });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0,0,0,0 });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0,0,0,0 });
	}

	void SimulateOverlay::pop_button_style() const
	{
		ImGui::PopStyleColor(3);
	}

	void SimulateOverlay::UnitData::Reset(Instance& instance)
	{
		if (!instance.ecs.view<Components::Character>(exclude_t<tc::Destroyed>()).contains(unit_id))
			unit_id = UNDEFINED;

		if (!instance.ecs.view<Components::Weapon>(exclude_t<tc::Destroyed>()).contains(weapon))
			weapon = UNDEFINED;

		if (!instance.ecs.view<Components::Graph>(exclude_t<tc::Destroyed>()).contains(action))
			action = UNDEFINED;
	}

	void SimulateOverlay::UnitData::Clear()
	{
		unit_id = UNDEFINED;
		weapon = UNDEFINED;
		action = UNDEFINED;
	}
}