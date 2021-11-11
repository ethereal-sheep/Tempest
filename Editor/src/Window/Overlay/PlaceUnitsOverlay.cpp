/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "PlaceUnitsOverlay.h"
#include "Tempest/src/Graphics/OpenGL/Texture.h"
#include "Graphics/Basics/RenderSystem.h"
#include "Triggers/Triggers.h"
#include "Instance/RuntimeInstance.h"
#include "ECS/Prototypes/Prototype_Category.h"

namespace Tempest
{
	void PlaceUnitsOverlay::open_popup(const Event& e)
	{
		OverlayOpen = true;
		auto a = event_cast<OpenPlaceUnitsOverlay>(e);
		entities = a.entities;
		chars.assign(entities.size(), INVALID);

		window_flags |= ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBackground;
		auto& cam = Service<RenderSystem>::Get().GetCamera();
		cam_ctrl.set_fixed_camera(cam, 90, 45);




	}
	void PlaceUnitsOverlay::init(Instance&)
	{
		OverlayOpen = false;

		window_flags =
			ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar;

		Service<EventManager>::Get().register_listener<OpenPlaceUnitsOverlay>(&PlaceUnitsOverlay::open_popup, this);

		chars.assign(4, INVALID);
	}


	void PlaceUnitsOverlay::show(Instance& instance [[maybe_unused]] )
	{
		if (!dynamic_cast<RuntimeInstance*>(&instance))
			return;

		auto& runtime = dynamic_cast<RuntimeInstance&>(instance);

		if (OverlayOpen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.f });

			if (ImGui::Begin("Combat Mode Screen", nullptr, window_flags))
			{

				bool selectable_hovered = false;
				auto& cam = Service<RenderSystem>::Get().GetCamera();

				cam_ctrl.controls(cam);
				cam_ctrl.update(cam);

				// character list
				/*{
					for (int i = 0; i < (int)chars.size(); ++i)
					{
						string name = "Character_";
						name += std::to_string(i);

						if (ImGui::Selectable(name.c_str(), chars[i] != INVALID))
						{
							selected = i;
						}

						if (ImGui::IsItemHovered())
							selectable_hovered = true;

						if (i == selected)
						{
							ImGui::SameLine();
							ImGui::Text(ICON_FA_CIRCLE);
						}
					}
				}*/

				auto window = ImGui::GetWindowDrawList();

				auto sideBg = tex_map["Assets/FadingSideBG.png"];
				auto imgWidth = tex_map["Assets/SmallTurnSelectedBtn.png"]->GetWidth();
				ImVec2 sideBarSize = { imgWidth + 20.f, ImGui::GetWindowHeight() * 0.6f };
				ImVec2 sideBgMin = ImGui::GetCurrentWindow()->Pos;
				ImVec2 sideBgMax = { sideBgMin.x + (float)sideBg->GetWidth(), sideBgMin.x + (float)ImGui::GetWindowHeight() };
				ImVec4 col = { 0.980f, 0.768f, 0.509f, 1.f };
				ImGui::PushFont(FONT_TURN);
				string str = "PLACING UNIT";
				auto strSize = ImGui::CalcTextSize(str.c_str());
				ImGui::PopFont();

				//ImGui::GetForegroundDrawList()->AddRectFilled({ winMin.x, winMin.y }, { winMax.x, winMax.y }, ImGui::GetColorU32(col));
				window->AddImage((void*)static_cast<size_t>(sideBg->GetID()), sideBgMin, sideBgMax);
				window->AddRectFilled({ sideBgMin.x, sideBgMin.y + ImGui::GetWindowHeight() * 0.1f }, { sideBgMin.x + imgWidth - 20.f,  sideBgMin.y + ImGui::GetWindowHeight() * 0.1f + strSize.y }, ImGui::GetColorU32(col));

				ImGui::PushFont(FONT_TURN);
				window->AddText({ sideBgMin.x + 5.f, sideBgMin.y + ImGui::GetWindowHeight() * 0.1f }, ImGui::GetColorU32({ 0,0,0,1 }), str.c_str());
				ImGui::PopFont();

				ImGui::PushFont(FONT_BODY);
				string str2 = "Place your selected units into the level";
				window->AddText({ sideBgMin.x + 5.f, sideBgMin.y + ImGui::GetWindowHeight() * 0.15f }, ImGui::GetColorU32({ 1,1,1,1 }), str2.c_str());
				ImGui::PopFont();

				ImGui::SetCursorPosY(sideBgMin.y + ImGui::GetWindowHeight() * 0.2f);
				DrawSideBar(instance, sideBarSize);
				ImVec2 buttonPos = { sideBgMin.x + 100.f, sideBgMin.y + ImGui::GetWindowHeight() * 0.87f };
				if (UI::UIButton_2("Turn Order", "Turn Order", buttonPos, { -40.f,20.f }, FONT_BODY))
				{

				}
				if (UI::UIButton_2("Save", "Save", { buttonPos.x + 200.f, buttonPos.y }, { -40.f,20.f }, FONT_BODY))
				{
					bool okay = true;
					for (auto i : chars)
						if (i == INVALID) okay = false;
					if (okay)
					{
						Service<EventManager>::Get().instant_dispatch<OpenCombatModeTrigger>(chars);
						OverlayOpen = false;
					}
				}


				// character list
				/*if (ImGui::Button("Fight!"))
				{
					bool okay = true;
					for (auto i : chars)
						if (i == INVALID) okay = false;
					if (okay)
					{
						Service<EventManager>::Get().instant_dispatch<OpenCombatModeTrigger>(chars);
						OverlayOpen = false;
					}
				}*/

				// highlight stuff on viewport
				{
					auto ray = cam.GetMouseRay();
					auto start = cam.GetPosition();
					float dist = 0;
					if (glm::intersectRayPlane(start, ray, glm::vec3{}, glm::vec3{ 0,1,0 }, dist) && ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows))
					{
						auto inter = cam.GetPosition() + ray * dist;

						int w_x = (int)std::round(inter.x - .5f);
						int w_y = (int)std::round(inter.z - .5f);

						id_t id = INVALID;
						if (runtime.collision_map.count(w_x) && runtime.collision_map[w_x].count(w_y))
							id = runtime.collision_map[w_x][w_y];

						if (id && instance.ecs.valid(id) && instance.ecs.has<tc::Shape>(id) && instance.ecs.has<tc::Transform>(id))
						{
							// draw existing collider
							{
								auto shape = instance.ecs.get_if<tc::Shape>(id);
								auto& transform = instance.ecs.get<tc::Transform>(id);

								const int& x = shape->x;
								const int& y = shape->y;
								const int one = 1;

								AABB box;

								int a_x = x, a_y = y, e_x = 0, e_y = 0;
								if (a_x % 2 != a_y % 2)
								{
									a_x = a_y = std::min(x, y);
									e_x = x - a_x;
									e_y = y - a_y;

								}

								box.min.x = -.5f - (a_x - 1) / 2.f;
								box.min.z = -.5f - (a_y - 1) / 2.f;

								box.max.x = +.5f + (a_x - 1) / 2.f + e_x;
								box.max.z = +.5f + (a_y - 1) / 2.f + e_y;

								auto rot = transform.rotation;
								box.min = rot * box.min;
								box.max = rot * box.max;

								box.min.x += transform.position.x;
								box.min.z += transform.position.z;
								box.min.y = 0;

								box.max.x += transform.position.x;
								box.max.z += transform.position.z;
								box.max.y = 0;

								Service<RenderSystem>::Get().DrawLine(box, { 1,0,0,1 });
							}
						}
						else if (selected >= 0 && selected < (int)chars.size())
						{
							// draw one box collider
							{
								AABB box;

								int a_x = 1, a_y = 1, e_x = 0, e_y = 0;
								if (a_x % 2 != a_y % 2)
								{
									a_x = a_y = std::min(1, 1);
									e_x = 1 - a_x;
									e_y = 1 - a_y;
								}

								inter.x = a_x % 2 ? std::floor(inter.x) + .5f : std::round(inter.x);
								inter.y = 0;
								inter.z = a_y % 2 ? std::floor(inter.z) + .5f : std::round(inter.z);

								box.min.x = inter.x - .5f - (a_x - 1) / 2.f;
								box.min.z = inter.z - .5f - (a_y - 1) / 2.f;
								box.min.y = 0;

								box.max.x = inter.x + .5f + (a_x - 1) / 2.f + e_x;
								box.max.z = inter.z + .5f + (a_y - 1) / 2.f + e_y;
								box.max.y = 0;

								Service<RenderSystem>::Get().DrawLine(box, { 0,1,0,1 });
							}

							auto& io = ImGui::GetIO();
							if (io.MouseClicked[0] && !ImGui::IsAnyItemHovered())
							{
								// create or move
								if (chars[selected])
								{
									// move existing
									auto& transform = instance.ecs.get<tc::Transform>(chars[selected]);
									// take note that inter has already been processed
									transform.position = inter;
								}
								else
								{
									// create proto
									// create prefab
									// then throw it into ecs

									auto random_char_id = instance.ecs.view_first<tc::Character>();
									auto proto_p = instance.scene.get_prototype_if("Unit", "Unit");
									auto prefab = proto_p ? proto_p->instance() : create_new_prototype("Unit").instance();
									auto entity = instance.ecs.create(prefab);

									chars[selected] = entity;
									LOG_ASSERT(instance.ecs.has<tc::Character>(entity));
									LOG_ASSERT(instance.ecs.has<tc::Transform>(entity));
									LOG_ASSERT(instance.ecs.has<tc::Model>(entity));

									auto& transform = instance.ecs.get<tc::Transform>(entity);
									//auto& model = instance.ecs.get<tc::Model>(entity);

									/* ===========================================
									* NOTE TO UI!!!!
									* ADD CHARACTER SHEET HERE TO THE CHARACTER!!
									* JUST ASSIGN THE COMPONENT
									* instance.ecs.get<tc::Character>(entity) = instance.ecs.get<tc::Character>(char_sheet_id)
									* ============================================
									*/
									if (random_char_id)
										instance.ecs.get<tc::Character>(entity) = entities[selected] ? instance.ecs.get<tc::Character>(entities[selected]) : instance.ecs.get<tc::Character>(random_char_id);

									transform.position = inter;
								}
							}
						}
					}
				}
				// highlight stuff on viewport

				// draw stuff on units
				{
					auto view = instance.ecs.view<tc::Unit, tc::Transform>();
					for (auto id : view)
					{
						auto position = instance.ecs.get<tc::Transform>(id).position;
						position.y += 2;
						auto ss = cam.WorldspaceToScreenspace(position);
						auto vp = cam.GetViewport();
						ss.x = (1 + ss.x) / 2 * vp.z;
						ss.y = vp.w - ((1 + ss.y) / 2 * vp.w);
						auto temp = ImGui::GetCursorPos();

						// Draw whatever thing on their head
						{

							ImGui::PushFont(FONT_BOLD);
							auto text_size = ImGui::CalcTextSize(ICON_FA_ICE_CREAM);
							auto cursor_pos = ImVec2{ ss.x - text_size.x / 2.f, ss.y - text_size.y / 2 };
							if (cursor_pos.x < viewport->WorkSize.x && ss.y + text_size.y / 2 < viewport->WorkSize.y) {
								ImGui::SetCursorPos(cursor_pos);
								ImGui::Text(ICON_FA_ICE_CREAM);
							}
							ImGui::PopFont();
						}


						ImGui::SetCursorPos(temp);
					}

				}
				// draw stuff on unit heads
			}

			ImGui::End();

			ImGui::PopStyleVar();
			ImGui::PopStyleColor(2);
		}
	}

	//void PlaceUnitsOverlay::show(Instance& instance [[maybe_unused]] )
	//{
	//	const ImGuiViewport* viewport = ImGui::GetMainViewport();

	//	ImGui::SetNextWindowPos(viewport->Pos);
	//	ImGui::SetNextWindowSize(viewport->Size);

	//	if (OverlayOpen)
	//	{
	//		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	//		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.f });

	//		if (ImGui::Begin("Place Units Overlay", nullptr, window_flags))
	//		{
	//			auto window = ImGui::GetWindowDrawList();

	//			auto sideBg = tex_map["Assets/FadingSideBG.png"];
	//			auto imgWidth = tex_map["Assets/SmallTurnSelectedBtn.png"]->GetWidth();
	//			ImVec2 sideBarSize = { imgWidth + 20.f, ImGui::GetWindowHeight() * 0.6f };
	//			ImVec2 sideBgMin = ImGui::GetCurrentWindow()->Pos;
	//			ImVec2 sideBgMax = { sideBgMin.x + (float)sideBg->GetWidth(), sideBgMin.x + (float)ImGui::GetWindowHeight() };
	//			ImVec4 col = { 0.980f, 0.768f, 0.509f, 1.f };
	//			ImGui::PushFont(FONT_TURN);
	//			string str = "PLACING UNIT";
	//			auto strSize = ImGui::CalcTextSize(str.c_str());
	//			ImGui::PopFont();

	//			//ImGui::GetForegroundDrawList()->AddRectFilled({ winMin.x, winMin.y }, { winMax.x, winMax.y }, ImGui::GetColorU32(col));
	//			window->AddImage((void*)static_cast<size_t>(sideBg->GetID()), sideBgMin, sideBgMax);
	//			window->AddRectFilled({ sideBgMin.x, sideBgMin.y + ImGui::GetWindowHeight() * 0.1f }, { sideBgMin.x + imgWidth - 20.f,  sideBgMin.y + ImGui::GetWindowHeight() * 0.1f + strSize.y  }, ImGui::GetColorU32(col));
	//			
	//			ImGui::PushFont(FONT_TURN);
	//			window->AddText({ sideBgMin.x + 5.f, sideBgMin.y + ImGui::GetWindowHeight() * 0.1f }, ImGui::GetColorU32({ 0,0,0,1 }), str.c_str());
	//			ImGui::PopFont();

	//			ImGui::PushFont(FONT_BODY);
	//			string str2 = "Place your selected units into the level";
	//			window->AddText({ sideBgMin.x + 5.f, sideBgMin.y + ImGui::GetWindowHeight() * 0.15f }, ImGui::GetColorU32({ 1,1,1,1 }), str2.c_str());
	//			ImGui::PopFont();

	//			ImGui::SetCursorPosY(sideBgMin.y + ImGui::GetWindowHeight() * 0.2f);
	//			DrawSideBar(instance, sideBarSize);
	//			ImVec2 buttonPos = { sideBgMin.x + 100.f, sideBgMin.y + ImGui::GetWindowHeight() * 0.87f };
	//			if (UI::UIButton_2("Turn Order", "Turn Order", buttonPos, { -40.f,20.f }, FONT_BODY))
	//			{

	//			}
	//			if (UI::UIButton_2("Save", "Save", { buttonPos.x + 200.f, buttonPos.y }, { -40.f,20.f }, FONT_BODY))
	//			{

	//			}
	//		}
	//		ImGui::End();
	//		ImGui::PopStyleVar();
	//		ImGui::PopStyleColor();
	//	}
	//}
	void PlaceUnitsOverlay::DrawSideBar(Instance& instance, ImVec2 size)
	{
		
		if (ImGui::BeginChild("##PlaceUnitSideBar", size))
		{
			for (int i = 0; i < (int)chars.size(); ++i)
			{
				string name = "Character_";
				name += std::to_string(i);

				auto unitIcon = tex_map["Assets/Unit_Black.png"];
				ImGui::SetCursorPosX(-10.f);
				if (UI::UICharTurnButton((void*)static_cast<size_t>(unitIcon->GetID()), { (float)unitIcon->GetWidth(), (float)unitIcon->GetHeight() }, name.c_str(), "##PlaceUnit" + name, chars[i] == instance.selected|| chars[i] != INVALID))
				{
					selected = i;
				}
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);
			}
		}
		
		
		ImGui::EndChild();

	}
}