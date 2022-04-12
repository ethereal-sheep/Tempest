/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "InspectorWindow.h"
#include "Util/GuizmoController.h"
#include "Tempest/src/Actions/Action.h"
#include "Tempest/src/Graphics/OpenGL/Texture.h"
#include "Events/EventManager.h"
#include "Triggers/Triggers.h"

namespace Tempest
{
	void InspectorWindow::init(Instance&)
	{
		window_flags |= ImGuiWindowFlags_MenuBar;
	}
	void InspectorWindow::show(Instance& instance)
	{
		// window
		if (ImGui::Begin(window_name(), &visible, window_flags))
		{
			if(instance.selected == INVALID)
			{
				ImGui::Text("NO ENTITY SELECTED");
				ImGui::End(); return;
			}

			// menu bar
			if (ImGui::BeginMenuBar())
			{
				UI::Tooltip(ICON_FA_QUESTION_CIRCLE, "Simple UI for inspecting objects. We should improve the UI once the UI/UX for it is done.", false);

				ImGui::EndMenuBar();
			}
			
			const auto padding = 80.f;
			// Meta data
			if (auto meta = instance.ecs.get_if<tc::Meta>(instance.selected))
			{
				UI::RenderText("Id:", padding);
				ImGui::Text("%u", instance.selected);

				UI::RenderText("Name:", padding);
				ImGui::InputText("##meta_name", &meta->name, ImGuiInputTextFlags_CharsNoBlank);
			}

			UI::PaddedSeparator(1.f);
			// transform data
			if (auto transform = instance.ecs.get_if<tc::Transform>(instance.selected))
			{
				//auto rb = instance.ecs.get_if<tc::Rigidbody>(instance.selected);
				ImGui::PushFont(FONT_BOLD);
				bool header = ImGui::CollapsingHeader("Transform##TransformHeader", nullptr, ImGuiTreeNodeFlags_DefaultOpen);
				ImGui::PopFont();
				ImGui::Dummy({ 0.f, 1.f });

				if (header)
				{
					UI::DragFloat3ColorBox("Position", "##TransformPosDrag", ImVec2{ padding , 0.f }, glm::value_ptr(transform->position), 0.f, 0.1f);

					/*auto& GC = Service<GuizmoController>::Get();

					if (GC.GetOperation() == GuizmoController::Operation::TRANSLATE)
					{
						static vec3 prevPos = vec3{ 0,0,0 };

						if (GC.GetState() == GuizmoController::State::START_USE)
							prevPos = transform->position;
						else if (GC.GetState() == GuizmoController::State::END_USE)
							instance.action_history.Commit<EditPosition>(instance.selected, prevPos, transform->position);
					}

					if (GC.GetOperation() != GuizmoController::Operation::SCALE)
					{

						ImGui::Dummy({ padding + 5.f, 0.f });
						ImGui::SameLine();

						if (ImGui::RadioButton("Local##TransfromGizmoLoc", GC.GetMode() == GuizmoController::Mode::LOCAL))
							GC.SetMode(GuizmoController::Mode::LOCAL);
						ImGui::SameLine();
						if (ImGui::RadioButton("World##TransfromGizmoWorld", GC.GetMode() == GuizmoController::Mode::WORLD))
							GC.SetMode(GuizmoController::Mode::WORLD);
					}

					{
						static glm::highp_vec3 prevRot{ 0,0,0 };
						static bool rotationEdited = false;
						auto euler = glm::eulerAngles(transform->rotation);
						if (UI::DragFloat3ColorBox("Rotation", "##TransformRotDrag", ImVec2{ padding , 0.f }, glm::value_ptr(euler), 0.f, 0.1f).second)
						{
							rotationEdited = true;

							transform->rotation = glm::quat(euler);
							if (ImGui::IsMouseClicked(0))
								prevRot = euler;
						}

						else if (rotationEdited && ImGui::IsMouseReleased(0))
						{
							rotationEdited = false;
							instance.action_history.Commit<EditRotation>(instance.selected, prevRot, glm::eulerAngles(transform->rotation));
						}
					}

					{
						static bool uniformScale = false;
						static vec3 newScale = transform->scale;
						

						if (UI::UniformScaleFloat3("Scale", "##TransformScaDrag", ImVec2{ padding , 0.f }, &uniformScale, glm::value_ptr(newScale), 1.f, 1.f, 1.f, 1000.f).second)
						{
							if (rb)
								rb->isDirty = true;

							instance.action_history.Commit<EditScale>(instance.selected, transform->scale, newScale);
							transform->scale = newScale;

						}
					}*/
				}
			}

			//StatsLine
			if (auto sl = instance.ecs.get_if<tc::Statline>(instance.selected))
			{
				ImGui::PushFont(FONT_BOLD);
				bool header = ImGui::CollapsingHeader("Statline##Statline", nullptr, ImGuiTreeNodeFlags_DefaultOpen);
				ImGui::PopFont();
				ImGui::Dummy({ 0.f, 1.f });
				if (header)
				{
					static bool show = false;
					if (ImGui::Button("Show"))
					{
						show = true;
					}
					if (show)
					{
						if (ImGui::Begin("Stats", &show))
						{
							for (auto& [enabled, s] : sl->get_stat_range())
							{
								ImGui::Text(s.c_str());
							}


							/*if (ImGui::Button("ADD"))
							{
								sl->add_stat(("Random" + std::to_string(sl->size() - 2)));
							}*/
						}
						ImGui::End();
					}
				}
			}

			if (auto w = instance.ecs.get_if<tc::Weapon>(instance.selected))
			{
				ImGui::PushFont(FONT_BOLD);
				bool header = ImGui::CollapsingHeader("Weapon##Weapon", nullptr, ImGuiTreeNodeFlags_DefaultOpen);
				auto StatsView = instance.ecs.view<Components::Statline>(exclude_t<tc::Destroyed>());
				Entity StateLineId = UNDEFINED;
				for (auto id : StatsView)
					StateLineId = id;
				auto sl = instance.ecs.get_if<tc::Statline>(StateLineId);
				ImGui::PopFont();
				ImGui::Dummy({ 0.f, 1.f });
				if (header)
				{
					static bool show = false;
					if (ImGui::Button("Show"))
					{
						show = true;
					}
					if (show)
					{
						if (ImGui::Begin("Weap", &show))
						{
							for (auto i = 0; i < sl->size(); i++)
							{
								string stat = sl->operator[](i) + " :";
								ImGui::Text(stat.c_str());
								ImGui::SameLine();
								ImGui::Text(std::to_string(w->get_stat(i)).c_str());
							}
							
						}
						ImGui::End();
					}
				}
			}

			//CharacterSheet
			if (auto cs = instance.ecs.get_if<tc::Character>(instance.selected))
			{
				ImGui::PushFont(FONT_BOLD);
				bool header = ImGui::CollapsingHeader("Character##Character", nullptr, ImGuiTreeNodeFlags_DefaultOpen);
				ImGui::PopFont();
				ImGui::Dummy({ 0.f, 1.f });
				if (header)
				{
					if (ImGui::Button("Show/Edit"))
					{
						//show = true;
						Service<EventManager>::Get().instant_dispatch<OpenUnitSheetTrigger>(false,instance, instance.selected);
					}

				}
			}
		}

		ImGui::End();
	}
}
