/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include "Instance/Instance.h"
#include "Graphics/OpenGL/RenderSystem.h"
#include "Util/UIElements.h"
#include "ECS/Prototypes/Prototype_Category.h"
#include "../Viewport/CameraControls.h"

namespace Tempest
{
	class test_window4 : public Window
	{
		bool OverlayOpen = false;
		CameraControls cam_ctrl;

		enum struct State {
			NO_SELECTED,
			MENU,
			ATTACKING,
			MOVING,
			NEXT,
		};

		State state = State::NO_SELECTED;

		const char* window_name() override
		{
			return "test_window3";
		}

		void init(Instance&) override
		{
			ImGuiStyle* style = &ImGui::GetStyle();

			style->FramePadding = ImVec2(8, 6);
			style->Colors[ImGuiCol_Button] = ImColor(40, 40, 40, 255);
			style->Colors[ImGuiCol_ButtonActive] = ImColor(40, 40, 40, 255);
			style->Colors[ImGuiCol_ButtonHovered] = ImColor(30, 30, 30, 255);

			// remember to do this
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar;


			Service<EventManager>::Get().register_listener<OpenCombatModeTrigger>(&test_window4::open_popup, this);
		}

		void open_popup(const Event&)
		{
			OverlayOpen = true;
			auto& cam = Service<RenderSystem>::Get().GetCamera();
			cam_ctrl.set_fixed_camera(cam, 45, 70);
		}

		void show(Instance& instance [[maybe_unused]] ) override
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

					//static int selected = -1;
					//static tvector<Entity> chars(4, INVALID);
					//bool selectable_hovered = false;
					auto& cam = Service<RenderSystem>::Get().GetCamera();

					cam_ctrl.controls(cam);
					cam_ctrl.update(cam);
					glm::ivec2 world_mouse = calculate_world_mouse(cam);

					// character list
					{/*
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
						}*/
					}
					//// character list
					//if (ImGui::Button("Fight!"))
					//{
					//	//Service<EventManager>::Get().instant_dispatch<Fight>();
					//}

					// selected

					if (!instance.selected || !instance.ecs.valid(instance.selected))
						state = State::NO_SELECTED;

					switch (state)
					{
					case State::NO_SELECTED:
						no_selected(runtime, world_mouse);
						break;
					case State::MENU:
						menu(runtime, world_mouse);
						break;
					case State::ATTACKING:
						attacking(runtime, world_mouse);
						break;
					case State::MOVING:
						moving(runtime, world_mouse);
						break;
					case State::NEXT:
						break;
					default:
						break;
					}
				}

				ImGui::End();

				ImGui::PopStyleVar();
				ImGui::PopStyleColor(2);
			}
		}

		glm::ivec2 calculate_world_mouse(const Camera& cam)
		{
			auto ray = cam.GetMouseRay();
			auto start = cam.GetPosition();
			float dist = 0;
			glm::ivec2 mouse = { -INT_MAX, -INT_MAX };
			if (glm::intersectRayPlane(start, ray, glm::vec3{}, glm::vec3{ 0,1,0 }, dist))
			{
				auto inter = cam.GetPosition() + ray * dist;

				int w_x = (int)std::round(inter.x - .5f);
				int w_y = (int)std::round(inter.z - .5f);

				mouse.x = w_x;
				mouse.y = w_y;
			}
			return mouse;
		}

		void no_selected(RuntimeInstance& instance, const glm::ivec2& world_mouse)
		{
			int w_x = world_mouse.x;
			int w_y = world_mouse.y;

			id_t id = INVALID;
			if (instance.collision_map.count(w_x) && instance.collision_map[w_x].count(w_y))
				id = instance.collision_map[w_x][w_y];

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

					vec4 color = { 1,0,0,1 };
					if (instance.ecs.has<tc::Door>(id))
						color = { 0,0,1,1 };
					else if (instance.ecs.has<tc::Obstacle>(id))
						color = { 0,0,1,1 };
					else if (instance.ecs.has<tc::Unit>(id))
						color = { 0.1,0.1,0.1,1 };

					Service<RenderSystem>::Get().DrawLine(box, color);
				}

				auto& io = ImGui::GetIO();
				if (io.MouseClicked[0] && !ImGui::IsAnyItemHovered())
				{
					// click
					instance.selected = id;
					state = State::MENU;
				}
			}
			else if (instance.selected && instance.ecs.has<tc::Unit>(instance.selected))
			{
				// draw one box collider
				{
					AABB box;

					box.min.x = (float)w_x;
					box.min.z = (float)w_y;
					box.min.y = 0;

					box.max.x = w_x + 1.f;
					box.max.z = w_y + 1.f;
					box.max.y = 0;

					Service<RenderSystem>::Get().DrawLine(box, { 0,1,0,1 });
				}
			}
		}

		void menu(RuntimeInstance& instance, const glm::ivec2& world_mouse)
		{
			auto& cam = Service<RenderSystem>::Get().GetCamera();
			auto& io = ImGui::GetIO();
			// draw stuff on selected heads
			{
				if (instance.selected && instance.ecs.has<tc::Unit>(instance.selected) && instance.ecs.has<tc::Transform>(instance.selected))
				{
					auto position = instance.ecs.get<tc::Transform>(instance.selected).position;
					position.y += 2;
					auto ss = cam.WorldspaceToScreenspace(position);
					auto vp = cam.GetViewport();
					ss.x = (1 + ss.x) / 2 * vp.z;
					ss.y = vp.w - ((1 + ss.y) / 2 * vp.w);
					auto temp = ImGui::GetCursorPos();

					int p_x = (int)std::floor(position.x);
					int p_y = (int)std::floor(position.z);

					LOG_ASSERT(instance.character_map[p_x][p_y] == instance.selected);
					LOG_ASSERT(instance.collision_map[p_x][p_y] == instance.selected);

					// Draw whatever thing on their head
					{
						ImGui::PushFont(FONT_BOLD);
						auto text_size = ImGui::CalcTextSize(ICON_FA_ICE_CREAM);
						auto cursor_pos = ImVec2{ ss.x - text_size.x / 2.f, ss.y - text_size.y / 2 };
						ImGui::SetCursorPos(cursor_pos);
						ImGui::BeginGroup();
						ImGui::Text(ICON_FA_ICE_CREAM);
						if (ImGui::Button("Attack"))
						{
							state = State::ATTACKING;
						}
						if (ImGui::Button("Move"))
						{
							state = State::MOVING;
						}
						ImGui::EndGroup();
						ImGui::PopFont();
					}
					ImGui::SetCursorPos(temp);
				}

			}
			// draw stuff on unit heads

			int w_x = world_mouse.x;
			int w_y = world_mouse.y;
			// unselect or select something else
			if (io.MouseClicked[0] && !ImGui::IsAnyItemHovered())
			{
				if (instance.collision_map.count(w_x) && instance.collision_map[w_x].count(w_y))
					instance.selected = instance.collision_map[w_x][w_y];
				else
				{
					instance.selected = INVALID;
					state = State::NO_SELECTED;
				}
			}
		}
		void attacking(RuntimeInstance& instance, const glm::ivec2& world_mouse)
		{
			auto& cam = Service<RenderSystem>::Get().GetCamera();
			if (instance.selected && instance.ecs.has<tc::Unit>(instance.selected) && instance.ecs.has<tc::Transform>(instance.selected))
			{
				auto position = instance.ecs.get<tc::Transform>(instance.selected).position;
				position.y += 2;
				auto ss = cam.WorldspaceToScreenspace(position);
				auto vp = cam.GetViewport();
				ss.x = (1 + ss.x) / 2 * vp.z;
				ss.y = vp.w - ((1 + ss.y) / 2 * vp.w);
				auto temp = ImGui::GetCursorPos();

				int p_x = (int)std::floor(position.x);
				int p_y = (int)std::floor(position.z);

				int w_x = world_mouse.x;
				int w_y = world_mouse.y;

				// outlines all units in the map
				for(auto& [x, m] : instance.character_map)
					for (auto& [y, id] : m)
					{
						if (id)
						{
							AABB box;

							box.min.x = (float)x;
							box.min.z = (float)y;
							box.min.y = 0;

							box.max.x = x + 1.f;
							box.max.z = y + 1.f;
							box.max.y = 0;
							if(x == p_x && y == p_y)
								Service<RenderSystem>::Get().DrawLine(box, { 0,1,0,1 });
							else
								Service<RenderSystem>::Get().DrawLine(box, { 1,0,0,1 });
						}
					}

				// if u want display like the triangle on top
				// need either imgui::image or drawlist

				if (instance.character_map[w_x][w_y])
				{
					auto& io = ImGui::GetIO();
					if (p_x == w_x && p_y == w_y)
					{
						Line l;
						l.p0 = glm::vec3(w_x + .5f - .5, 0, w_y + .5f - .5);
						l.p1 = glm::vec3(w_x + .5f + .5, 0, w_y + .5f + .5);

						Line r;
						r.p0 = glm::vec3(w_x + .5f - .5, 0, w_y + .5f + .5);
						r.p1 = glm::vec3(w_x + .5f + .5, 0, w_y + .5f - .5);
						Service<RenderSystem>::Get().DrawLine(l, { 1,0,0,1 });
						Service<RenderSystem>::Get().DrawLine(r, { 1,0,0,1 });

						if (io.MouseClicked[0])
						{
							// cancel
							state = State::MENU;
						}
					}
					else
					{
						Line l;
						l.p0 = glm::vec3(w_x + .5f - .1, 0, w_y + .5f - .1);
						l.p1 = glm::vec3(w_x + .5f + .1, 0, w_y + .5f + .1);

						Line r;
						r.p0 = glm::vec3(w_x + .5f - .1, 0, w_y + .5f + .1);
						r.p1 = glm::vec3(w_x + .5f + .1, 0, w_y + .5f - .1);
						Service<RenderSystem>::Get().DrawLine(l, { 0,1,0,1 });
						Service<RenderSystem>::Get().DrawLine(r, { 0,1,0,1 });

						if (io.MouseClicked[0])
						{
							// Attack
							auto attacker = instance.selected;
							auto defender = instance.character_map[w_x][w_y];

							LOG_ASSERT(instance.ecs.has<tc::Character>(attacker));
							LOG_ASSERT(instance.ecs.has<tc::Character>(defender));

							// for now we just cancel
							state = State::MENU;
						}
					}


				}

			}
			else
				state = State::MENU;
		}
		void moving(RuntimeInstance& instance, const glm::ivec2& world_mouse)
		{
			auto& cam = Service<RenderSystem>::Get().GetCamera();
			if (instance.selected && instance.ecs.has<tc::Unit>(instance.selected) && instance.ecs.has<tc::Transform>(instance.selected))
			{
				auto position = instance.ecs.get<tc::Transform>(instance.selected).position;
				position.y += 2;
				auto ss = cam.WorldspaceToScreenspace(position);
				auto vp = cam.GetViewport();
				ss.x = (1 + ss.x) / 2 * vp.z;
				ss.y = vp.w - ((1 + ss.y) / 2 * vp.w);
				auto temp = ImGui::GetCursorPos();

				int p_x = (int)std::floor(position.x);
				int p_y = (int)std::floor(position.z);

				LOG_ASSERT(instance.character_map[p_x][p_y] == instance.selected);
				LOG_ASSERT(instance.collision_map[p_x][p_y] == instance.selected);

				// draw bfs if unit
				tmap<int, tmap<int, bool>> visited;
				tmap<int, tmap<int, uint32_t>> distance;
				{
					//bfs
					const uint32_t range = 4;
					const tvector<tpair<int, int>> dir = { {0,1}, {0,-1}, {1,0}, {-1,0} };
					std::queue<glm::ivec2> q;
					q.push({ p_x, p_y });

					//draw
					while (!q.empty())
					{
						auto p = q.front();
						q.pop();

						if (visited[p.x][p.y])
							continue;

						visited[p.x][p.y] = true;

						auto new_dist = distance[p.x][p.y] + 1;
						if (new_dist > range)
							continue;

						for (auto [x, y] : dir)
						{
							if (instance.collision_map[p.x + x][p.y + y])
								continue;
							if (visited[p.x + x][p.y + y])
								continue;

							distance[p.x + x][p.y + y] = new_dist;
							q.push({ p.x + x, p.y + y });
						}

					}

					for (auto& [x, m] : visited)
						for (auto [y, b] : m)
						{
							if (!b) continue;
							AABB box;

							box.min.x = (float)x;
							box.min.z = (float)y;
							box.min.y = 0;

							box.max.x = x + 1.f;
							box.max.z = y + 1.f;
							box.max.y = 0;

							Service<RenderSystem>::Get().DrawLine(box, { 0,1,0,1 });
						}


					
				}

				int w_x = world_mouse.x;
				int w_y = world_mouse.y;

				if(visited[w_x][w_y])
				{
					auto& io = ImGui::GetIO();
					if (p_x == w_x && p_y == w_y)
					{
						Line l;
						l.p0 = glm::vec3(w_x + .5f - .5, 0, w_y + .5f - .5);
						l.p1 = glm::vec3(w_x + .5f + .5, 0, w_y + .5f + .5);

						Line r;
						r.p0 = glm::vec3(w_x + .5f - .5, 0, w_y + .5f + .5);
						r.p1 = glm::vec3(w_x + .5f + .5, 0, w_y + .5f - .5);
						Service<RenderSystem>::Get().DrawLine(l, { 1,0,0,1 });
						Service<RenderSystem>::Get().DrawLine(r, { 1,0,0,1 });

						if (io.MouseClicked[0])
						{
							// cancel
							state = State::MENU;
						}
					}
					else
					{
						Line l;
						l.p0 = glm::vec3(w_x + .5f - .1, 0, w_y + .5f - .1);
						l.p1 = glm::vec3(w_x + .5f + .1, 0, w_y + .5f + .1);

						Line r;
						r.p0 = glm::vec3(w_x + .5f - .1, 0, w_y + .5f + .1);
						r.p1 = glm::vec3(w_x + .5f + .1, 0, w_y + .5f - .1);
						Service<RenderSystem>::Get().DrawLine(l, { 0,1,0,1 });
						Service<RenderSystem>::Get().DrawLine(r, { 0,1,0,1 });

						if (io.MouseClicked[0])
						{
							// move
							auto& transform = instance.ecs.get<tc::Transform>(instance.selected);
							transform.position.x = w_x + .5f;
							transform.position.z = w_y + .5f;
							instance.selected = INVALID;
							state = State::NO_SELECTED;
						}
					}


				}
			
			}
			else
				state = State::MENU;

		}

	};
}