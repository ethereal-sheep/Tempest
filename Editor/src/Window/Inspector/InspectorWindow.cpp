#include "InspectorWindow.h"
#include "Util/GuizmoController.h"
#include <Tempest/src/Actions/Action.h>
#include <Tempest/src/Graphics/OpenGL/Texture.h>
#include <Tempest/src/Graphics/OpenGL/RenderSystem.cpp>

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
				auto rb = instance.ecs.get_if<tc::Rigidbody>(instance.selected);
				ImGui::PushFont(FONT_BOLD);
				bool header = ImGui::CollapsingHeader("Transform##TransformHeader", nullptr, ImGuiTreeNodeFlags_DefaultOpen);
				ImGui::PopFont();
				ImGui::Dummy({ 0.f, 1.f });

				if (header)
				{
					UI::DragFloat3ColorBox("Position", "##TransformPosDrag", ImVec2{ padding , 0.f }, transform->position.data(), 0.f, 0.1f);

					auto& GC = Service<GuizmoController>::Get();

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
						bool UIScaled = false;

						if (UI::UniformScaleFloat3("Scale", "##TransformScaDrag", ImVec2{ padding , 0.f }, &uniformScale, newScale.data(), 1.f, 1.f, 1.f, 1000.f).second)
						{
							if (rb)
								rb->isDirty = true;

							instance.action_history.Commit<EditScale>(instance.selected, transform->scale, newScale);
							transform->scale = newScale;

						}
					}
				}
			}

			//RigidBody
			if (auto rb = instance.ecs.get_if<tc::Rigidbody>(instance.selected))
			{
				auto& rbConfig = rb->rb_config;
				auto transform = instance.ecs.get_if<tc::Transform>(instance.selected);
				ImGui::PushFont(FONT_BOLD);
				bool header = ImGui::CollapsingHeader("RigidBody##RigidBody", nullptr, ImGuiTreeNodeFlags_DefaultOpen);
				ImGui::PopFont();
				ImGui::Dummy({ 0.f, 1.f });
				if (header)
				{
					UI::DragFloat3ColorBox("Linear Vel", "##RbLVel", ImVec2{ padding , 0.f }, rbConfig.linear_velocity.data(), 0.f, 0.1f);
					UI::DragFloat("L_Damping", "##RbLDamping", ImVec2{ padding , 0.f }, &rbConfig.linear_damping, 0.1f, 0.f);
					UI::DragFloat3ColorBox("Angular Vel", "##RbAVel", ImVec2{ padding , 0.f }, rbConfig.angular_velocity.data(), 0.f, 0.1f);
					UI::DragFloat("A_Damping", "##RbADamping", ImVec2{ padding , 0.f }, &rbConfig.angular_damping, 0.1f, 0.f);
					UI::DragFloat("Mass", "##RbMass", ImVec2{ padding , 0.f }, &rbConfig.mass, 0.1f, 0.f);
					UI::DragFloat("Density", "##RbDensity", ImVec2{ padding , 0.f }, &rbConfig.density, 0.1f, 0.f);
					UI::Checkbox("Is_Static", "##RbStatic", ImVec2{ padding , 0.f }, &rbConfig.is_static);
					UI::Checkbox("Gravity", "##RbGravity", ImVec2{ padding , 0.f }, &rbConfig.gravity);
					UI::DragFloat3("Physics Mat", "##RbPhysMat", ImVec2{ padding , 0.f }, rbConfig.material.data(), 0.f, 0.1f);
					UI::Tooltip(ICON_FA_QUESTION_CIRCLE, "Physics Material: Static Friction , Dynamic Friction, restitution(bounciness)");
					
					//bool changed = false;
					ImGui::PushID("Collider Type");
					int collider_current = static_cast<int>(rb->shape_data.type);
					const char* Colliders[] = { "NONE", "SPHERE", "BOX", "CAPSULE" };
					ImGui::Text("ShapeT");
					ImGui::SameLine();
					ImGui::Dummy(ImVec2{ 60.f - ImGui::GetItemRectSize().x, 0.f });
					ImGui::SameLine();
					ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() - 20.f);
					if (ImGui::Combo("", &collider_current, Colliders, IM_ARRAYSIZE(Colliders)))
					{
						rb->shape_data.type = static_cast<SHAPE_TYPE>(collider_current);
						rb->isDirty = true;
					}
					ImGui::PopID();
					
					//Collider Size
					switch (rb->shape_data.type)
					{
					case SHAPE_TYPE::SPHERE:
						rb->isDirty |= UI::DragFloat("Radius", "##RbRadius", ImVec2{ padding , 0.f }, &rb->shape_data.shapeData.x, 0.1f, 0.5f).second;
						break;
					case SHAPE_TYPE::CAPSULE:
					{
						vec2 temp = { rb->shape_data.shapeData.x, rb->shape_data.shapeData.y };
						rb->isDirty |= UI::DragFloat2("Radius&Height", "##RbRadius&Height", ImVec2{ padding , 0.f }, temp.data(), 0.1f, 0.5f).second;
						rb->shape_data.shapeData.x = temp.x ;
						rb->shape_data.shapeData.y = temp.y;
						break;
					}
					break;
					case SHAPE_TYPE::BOX:
					{
						rb->isDirty |= UI::DragFloat3("X,Y,Z", "##RbRadius&Height", ImVec2{ padding , 0.f }, rb->shape_data.shapeData.data(), 0.1f, 0.5f).second;
					}
					break;
					case SHAPE_TYPE::NONE:
						LOG_ERROR("No shape data");
						break;
					}

					//IF Change reattached a new shape
					if (rb->isDirty)
					{
						LOG("Value Changed");

						switch (rb->shape_data.type)
						{
						case SHAPE_TYPE::SPHERE:
							rb->shape_data.shapeData.x = transform->scale.x;
							break;
						case SHAPE_TYPE::CAPSULE:
						{
							rb->shape_data.shapeData.x = transform->scale.x;
							rb->shape_data.shapeData.y = transform->scale.y / 2;
							break;
						}
						break;
						case SHAPE_TYPE::BOX:
						{
							rb->shape_data.shapeData.x = transform->scale.x / 2;
							rb->shape_data.shapeData.y = transform->scale.y / 2;
							rb->shape_data.shapeData.z = transform->scale.z / 2;
						}
						break;
						case SHAPE_TYPE::NONE:
							LOG_ERROR("No shape data");
							break;
						}

						//Reattach of newShape onto Actor
						physx::PxShape* newShape = instance.po.CreateActorShape(rbConfig, rb->shape_data);
						const physx::PxU32 numShapes = rb->internal_rb.get()->getNbShapes();
						
						for (physx::PxU32 i = 0; i < numShapes; i++)
						{
							physx::PxShape* CurShape = nullptr;
							rb->internal_rb.get()->getShapes(&CurShape, 1, i);
							rb->internal_rb.get()->detachShape(*CurShape);
						}
						rb->internal_rb.get()->attachShape(*newShape);
						newShape->release();
						rb->isDirty = false;
					}
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
							for (auto i : sl->get_stats())
							{
								ImGui::Text(i.c_str());
							}


							if (ImGui::Button("ADD"))
							{
								sl->add_stat(("Random" + std::to_string(sl->size() - 2)));
							}
						}
						ImGui::End();
					}
				}
			}

			if (auto w = instance.ecs.get_if<tc::Weapon>(instance.selected))
			{
				ImGui::PushFont(FONT_BOLD);
				bool header = ImGui::CollapsingHeader("Weapon##Weapon", nullptr, ImGuiTreeNodeFlags_DefaultOpen);
				auto sl = instance.ecs.get_if<tc::Statline>(1);
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
								string stat = sl->get_stats()[i] + " :";
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
					auto sl = instance.ecs.get_if<tc::Statline>(1);
					static bool show = false;
					if (ImGui::Button("Show/Edit"))
					{
						show = true;
					}
					
					if (show)
					{
						if (ImGui::Begin("Data", &show))
						{
							float center_x = ImGui::GetContentRegionAvailWidth() / 2.f;
							
							UI::Header(ImVec2{ center_x - 100.f,10}, "Unit Creation");
							ImGui::SameLine();
							bool save = UI::UIButton_1("Save", "Save", { center_x + 300.f,30.f }, { 90.f, 0.f }, FONT_PARA);
							ImGui::Dummy({ 0, 20.f});
							if (save)
							{
								LOG("SAVED");
								//TODO
							}

							
							ImGui::Columns(4, "UnitCreation", true);
							auto texture = Service<RenderSystem>::Get().tex->GetID();
							GLuint tex_id = static_cast<GLuint>(texture);
							ImGui::Image((void*)static_cast<size_t>(tex_id), ImVec2(ImGui::GetColumnWidth(0), ImGui::GetColumnWidth(0)));
							/*==================================================================
												CHARACTER INFOMATION COLUMN
							==================================================================*/
							ImGui::NextColumn();
							string text = "Character Infomation";
							float FirstColWidth = ImGui::GetColumnWidth(1)/2;
							float font_size = ImGui::GetFontSize() * text.size() / 2;
							float offsetX = FirstColWidth - font_size + (font_size / 2);
							float frontPadding = 5.f;
							ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.f);
							ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, 0.f);
							ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.f);
							ImGui::Dummy({ offsetX, 0 });
							ImGui::SameLine();
							ImGui::Text(text.c_str());
							ImGui::Dummy({ 0, 10.f });
							ImGui::BeginChild("##CharacterInfo", ImVec2(ImGui::GetColumnWidth(1) - 10.f, 300.0f));
							//Name
							ImGui::Dummy({ frontPadding, 0});
							ImGui::SameLine();
							ImGui::Text("Name");
							
							ImGui::Dummy({ frontPadding, 0 });
							ImGui::SameLine();
							ImGui::InputText("##CharacterName", &cs->name);
							
							ImGui::Dummy({ 0, 10.f });

							for (auto i = 0; i < sl->size(); i++)
							{
								string stat = sl->get_stats()[i] + " :";
								string label = "##" + stat;
								auto data = std::to_string(cs->get_stat(i));
								ImGui::Dummy({ frontPadding, 0 });
								ImGui::SameLine();
								ImGui::Text(stat.c_str());
								ImGui::Dummy({ frontPadding, 0 });
								ImGui::SameLine();
								ImGui::InputInt(label.c_str(), &cs->get_stat(i), 0);
								ImGui::Dummy({ 0, 10.f });
							}
							ImGui::EndChild();
							ImGui::PopStyleVar(3);
							
							/*==================================================================
												WEAPON AND ITEMS COLUMN
							==================================================================*/
							ImGui::NextColumn();
							string WeapText = "Weapons and Items";
							float SecondColWidth = ImGui::GetColumnWidth(2) / 2;
							font_size = ImGui::GetFontSize() * WeapText.size() / 2;
							float SecondoffsetX = SecondColWidth - font_size + (font_size / 2);
							
							ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.f);
							ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, 0.f);
							ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.f);
							ImGui::Dummy({ SecondoffsetX, 0 });
							ImGui::SameLine();
							ImGui::Text(WeapText.c_str());
							ImGui::Dummy({ 0, 10.f });
							ImGui::BeginChild("##WeapAndItem", ImVec2(ImGui::GetColumnWidth(2) - 10.f, 100.0f));

							ImGui::Dummy({ frontPadding, 0 });
							ImGui::SameLine();
							ImGui::Text("Equipped");
							
							if (cs->weapon)
							{
								auto weap = instance.ecs.get<tc::Weapon>(cs->weapon);
								ImGui::Dummy({ frontPadding, 0 });
								ImGui::SameLine();
								UI::UIButton_1(weap.name.c_str(), weap.name.c_str(), { frontPadding + 10.f, 20.f }, { 100.f, 10.f }, FONT_PARA);
							}
							ImGui::EndChild();
							float btnPos = ImGui::GetColumnWidth(0) + ImGui::GetColumnWidth(1);
							bool addWeap = UI::UIButton_1("Add Weapon/Item", "Add Weapon/Item", { btnPos + 15, 430.f }, { 50.f, 10.f }, FONT_PARA);
							
							static bool open = false;
							if (addWeap)
							{
								open = true;
								LOG("ADDING WEAPON");
							}
							if (open)
							{
								ImGui::OpenPopup("Add Weapon");
								ImGui::SetNextWindowSize(ImVec2(500, 550));
								ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar|
									ImGuiWindowFlags_NoScrollbar;
								if (ImGui::BeginPopupModal("Add Weapon",NULL, flags))
								{
									float RegionWidth = ImGui::GetContentRegionAvailWidth() / 2.f;
									string sub = "Add Weapon";
									font_size = ImGui::GetFontSize() * sub.size() / 2;
									float center = RegionWidth - font_size;
									Entity CurSelection = UNDEFINED;

									UI::SubHeader({ center - 100.f, 0 },"Add Weapon");
									ImGui::Dummy({ 0, 20.f });
									ImGui::Columns(2, "Weapons", true);

									/*==================================================================
														ADD WEAPON COLUMN
									==================================================================*/
									ImGui::BeginChild("##weap", ImVec2(ImGui::GetColumnWidth(0) - 10.f, 300.0f));
									auto view = instance.ecs.view<Components::Weapon>(exclude_t<tc::Destroyed>());
									ImVec2 pos = { 30.f, 10.f };
									float index = 1;
									for (auto id : view)
									{
										auto& weap = instance.ecs.get<Components::Weapon>(id);
										pos.y = pos.y * index + (60 * (index - 1));
										bool WeapSelected = UI::UIButton_1(weap.name.c_str(), weap.name.c_str(),pos, { 100.f, 10.f }, FONT_PARA);
										if (WeapSelected)
										{
											CurSelection = id;
										}
										index++;
									}
									ImGui::EndChild();
									/*==================================================================
														Create WEAPON COLUMN
									==================================================================*/
									ImGui::NextColumn();

									if (ImGui::Button("CLOSE"))
									{
										ImGui::CloseCurrentPopup();
										open = false;
									}

									ImGui::EndPopup();
								}
							}
							
							ImGui::PopStyleVar(3);
							/*==================================================================
													SKILL COLUMN
							==================================================================*/
							ImGui::NextColumn();
							string SkillText = "Skill";
							font_size = ImGui::GetFontSize() * SkillText.size() / 2;
							float ThirdColWidth = ImGui::GetColumnWidth(2) / 2;
							float ThirdoffsetX = ThirdColWidth - font_size + (font_size / 2);

							ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.f);
							ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, 0.f);
							ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.f);
							ImGui::Dummy({ ThirdoffsetX, 0 });
							ImGui::SameLine();
							ImGui::Text(SkillText.c_str());
							ImGui::Dummy({ 0, 10.f });
							ImGui::BeginChild("##SKILL", ImVec2(ImGui::GetColumnWidth(2) - 10.f, 300.0f));


							ImGui::EndChild();
							ImGui::PopStyleVar(3);
							
							ImGui::EndColumns();
							//static ImVec2 pos = { 200,500 };
							//ImVec2 slider_padding = { 100,0 };
							//UI::DragFloat2("Pos ", "5466", slider_padding, &pos.x);
							//ImGui::Getcol
							

							
							/*ImGui::NextColumn();
							for (auto i = 0; i < sl->size(); i++)
							{
								string stat = sl->get_stats()[i] + " :";
								ImGui::Text(stat.c_str());
								ImGui::SameLine();
								ImGui::Text(std::to_string(cs->get_stat(i)).c_str());
							}
							ImGui::NextColumn();
							for (auto i = 0; i < sl->size(); i++)
							{
								string stat = sl->get_stats()[i] + " :";
								ImGui::Text(stat.c_str());
								ImGui::SameLine();
								ImGui::Text(std::to_string(cs->get_stat(i)).c_str());
							}*/
						}
						ImGui::End();
					}
				}
			}
		}

		ImGui::End();
	}
}
