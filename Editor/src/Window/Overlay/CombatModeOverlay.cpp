/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "CombatModeOverlay.h"
#include "ECS/Prototypes/Prototype_Category.h"
#include "Graphics/Basics/RenderSystem.h"
#include "Triggers/SimulationTriggers.h"
#include "Events/Events/ScriptEvents.h"

namespace Tempest
{

	glm::ivec2 CombatModeOverlay::calculate_world_mouse(const Camera& cam)
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

	void CombatModeOverlay::no_selected(RuntimeInstance& instance, const glm::ivec2& world_mouse)
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
		else
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

	void CombatModeOverlay::menu(RuntimeInstance& instance, const glm::ivec2& world_mouse)
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
					/*if (ImGui::Button("Attack"))
					{
						state = State::ATTACKING;
					}
					if (ImGui::Button("Move"))
					{
					}*/
					ImGui::EndGroup();
					ImGui::PopFont();
				}
				ImGui::SetCursorPos(temp);

				// action
				const ImGuiViewport* viewport = ImGui::GetMainViewport();

				if (battle_state == BATTLE_STATE::CURR_TURN || battle_state == BATTLE_STATE::SELECT_ACTION || battle_state == BATTLE_STATE::SELECT_WEAPON)
				{
					UI::ActionUI(ImVec2{ viewport->Size.x, viewport->Size.y - action_background_size.y }, battle_state == BATTLE_STATE::SELECT_WEAPON ? "SELECT A WEAPON" : "SELECT AN ACTION");
					ImGui::SetCursorPos(ImVec2{ viewport->Size.x - action_background_size.x * 0.85f , viewport->Size.y - action_background_size.y * 0.7f });
					if (ImGui::BeginChild("Action content", ImVec2{ action_background_size.x * 0.85f, action_background_size.y * 0.7f }, true, ImGuiWindowFlags_NoScrollbar))
					{
						switch (battle_state)
						{
						case Tempest::CombatModeOverlay::BATTLE_STATE::CURR_TURN:
						{
							// action
							if (UI::UIButton_2("ACTION", "ACTION", ImVec2{ ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.3f, ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y * 0.5f }, { -150,80 }, FONT_SHEAD))
							{
								battle_state = BATTLE_STATE::SELECT_ACTION;
							}

							ImGui::SameLine();
							const ImVec2 cursor{ ImGui::GetCursorPos() };
							const ImVec2 avail_region{ ImGui::GetContentRegionAvail() };
							if (UI::UIButton_2("ITEMS", "ITEMS", ImVec2{ cursor.x + avail_region.x * 0.5f, cursor.y - avail_region.y * 0.25f }, { -200,10 }, FONT_SHEAD))
							{

							}

							if (UI::UIButton_2("MOVE", "MOVE", ImVec2{ cursor.x + avail_region.x * 0.5f, cursor.y + avail_region.y * 0.6f }, { -200,10 }, FONT_SHEAD))
							{
								state = State::MOVING;
							}
						}
						break;
						case Tempest::CombatModeOverlay::BATTLE_STATE::SELECT_ACTION: // TODO: account for no actions
						{
							auto& charac = instance.ecs.get<tc::Character>(instance.selected);
							unsigned i = 0;
							float xpos = ImGui::GetCursorPosX() + 60.0f;
							for (auto id : charac.actions)
							{
								auto& action = instance.ecs.get<tc::Graph>(id);

								ImGui::SetCursorPos(ImVec2{ selected_action == id ? xpos - action_button_diff : xpos, ImGui::GetCursorPosY() });
								if (UI::UIActionButton(action.g.name.c_str(), "##ACTIONSTUFF" + i++, selected_action == id))
								{
									selected_action = id;
									battle_state = BATTLE_STATE::SELECT_WEAPON;
								}
							}
						}
						break;
						case Tempest::CombatModeOverlay::BATTLE_STATE::SELECT_WEAPON: // TODO: account for no weapons
						{
							auto& charac = instance.ecs.get<tc::Character>(instance.selected);
							unsigned i = 0;
							float xpos = ImGui::GetCursorPosX() + 60.0f;

							if (charac.weapons.empty())
							{
								selected_weapon = INVALID;
								other_entity = INVALID;
								battle_state = BATTLE_STATE::SELECT_OTHER;
								state = State::ATTACKING;
							}
							else
							{
								for (auto id : charac.weapons)
								{
									auto& action = instance.ecs.get<tc::Weapon>(id);

									ImGui::SetCursorPos(ImVec2{ selected_weapon == id ? xpos - action_button_diff : xpos, ImGui::GetCursorPosY() });
									if (UI::UIActionButton(action.name.c_str(), "##WEAPONSTUFF" + i++, selected_weapon == id))
									{
										selected_weapon = id;
										other_entity = INVALID;
										battle_state = BATTLE_STATE::SELECT_OTHER;
										state = State::ATTACKING;
									}
								}
							}

						}
						break;
						default:
							break;
						}

					}
					ImGui::EndChild();
				}

				// display the back button at the top right
				if (battle_state == BATTLE_STATE::SELECT_ACTION || battle_state == BATTLE_STATE::SELECT_WEAPON)
				{
					if (UI::UIButton_2("Back", "Back", ImVec2{ viewport->Size.x * 0.92f, viewport->Size.y - action_background_size.y * 1.2f }, { 0,0 }, FONT_BODY))
					{
						battle_state = battle_state == BATTLE_STATE::SELECT_ACTION ? BATTLE_STATE::CURR_TURN : BATTLE_STATE::SELECT_ACTION;
					}
				}
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
	
	void CombatModeOverlay::attacking(RuntimeInstance& instance, const glm::ivec2& world_mouse)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
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

			if (other_entity)
			{
				auto character_id = other_entity;
				
				if (battle_state == BATTLE_STATE::CURR_TURN || battle_state == BATTLE_STATE::SELECT_ACTION || battle_state == BATTLE_STATE::SELECT_WEAPON)
				{
					UI::ActionUI(ImVec2{ viewport->Size.x, viewport->Size.y - action_background_size.y }, battle_state == BATTLE_STATE::SELECT_WEAPON ? "SELECT A WEAPON" : "SELECT AN ACTION");
					ImGui::SetCursorPos(ImVec2{ viewport->Size.x - action_background_size.x * 0.85f , viewport->Size.y - action_background_size.y * 0.7f });
					if (ImGui::BeginChild("Action content", ImVec2{ action_background_size.x * 0.85f, action_background_size.y * 0.7f }, true, ImGuiWindowFlags_NoScrollbar))
					{
						switch (battle_state)
						{
						case Tempest::CombatModeOverlay::BATTLE_STATE::CURR_TURN:
						{
							// action
							if (UI::UIButton_2("ACTION", "ACTION", ImVec2{ ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.3f, ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y * 0.5f }, { -150,80 }, FONT_SHEAD))
							{
								battle_state = BATTLE_STATE::SELECT_ACTION;
							}

							/*ImGui::SameLine();
							const ImVec2 cursor{ ImGui::GetCursorPos() };
							const ImVec2 avail_region{ ImGui::GetContentRegionAvail() };
							if (UI::UIButton_2("ITEMS", "ITEMS", ImVec2{ cursor.x + avail_region.x * 0.5f, cursor.y - avail_region.y * 0.25f }, { -200,10 }, FONT_SHEAD))
							{

							}

							if (UI::UIButton_2("MOVE", "MOVE", ImVec2{ cursor.x + avail_region.x * 0.5f, cursor.y + avail_region.y * 0.6f }, { -200,10 }, FONT_SHEAD))
							{
								state = State::MOVING;
							}*/
						}
						break;
						case Tempest::CombatModeOverlay::BATTLE_STATE::SELECT_ACTION: // TODO: account for no actions
						{
							auto& charac = instance.ecs.get<tc::Character>(character_id);
							unsigned i = 0;
							float xpos = ImGui::GetCursorPosX() + 60.0f;
							for (auto id : charac.actions)
							{
								auto& action = instance.ecs.get<tc::Graph>(id);

								ImGui::SetCursorPos(ImVec2{ other_selected_action == id ? xpos - action_button_diff : xpos, ImGui::GetCursorPosY() });
								if (UI::UIActionButton(action.g.name.c_str(), "##ACTIONSTUFF" + i++, other_selected_action == id))
								{
									other_selected_action = id;
									battle_state = BATTLE_STATE::SELECT_WEAPON;
								}
							}
						}
						break;
						case Tempest::CombatModeOverlay::BATTLE_STATE::SELECT_WEAPON: // TODO: account for no weapons
						{
							auto& charac = instance.ecs.get<tc::Character>(character_id);
							unsigned i = 0;
							float xpos = ImGui::GetCursorPosX() + 60.0f;


							if (charac.weapons.empty())
							{
								other_selected_weapon = INVALID;
								battle_state = BATTLE_STATE::BATTLE_GLIMPSE;
								state = State::GLIMPSE;

								auto& atker = instance.ecs.get<tc::Character>(instance.selected);
								auto& defer = instance.ecs.get<tc::Character>(other_entity);

								atker.chosen_weapon = selected_weapon;
								defer.chosen_weapon = other_selected_weapon;

								auto sequence = instance.ecs.view_first<tc::ConflictGraph>();

								/*SimulateConflict s{
									instance.selected,
									other_entity,
									selected_action,
									other_selected_action,
									sequence, freq, win, lose, attack, defend, finish };*/

								Service<EventManager>::Get().instant_dispatch<SimulateConflict>(
									instance.selected,
									other_entity,
									selected_action,
									other_selected_action,
									sequence, freq, win, lose, attack, defend, finish);
							}
							else
							{
								for (auto id : charac.weapons)
								{
									auto& action = instance.ecs.get<tc::Weapon>(id);

									ImGui::SetCursorPos(ImVec2{ other_selected_weapon == id ? xpos - action_button_diff : xpos, ImGui::GetCursorPosY() });
									if (UI::UIActionButton(action.name.c_str(), "##WEAPONSTUFF" + i++, other_selected_weapon == id))
									{
										other_selected_weapon = id;
										battle_state = BATTLE_STATE::BATTLE_GLIMPSE;
										state = State::GLIMPSE;

										auto& atker = instance.ecs.get<tc::Character>(instance.selected);
										auto& defer = instance.ecs.get<tc::Character>(other_entity);

										atker.chosen_weapon = selected_weapon;
										defer.chosen_weapon = other_selected_weapon;

										auto sequence = instance.ecs.view_first<tc::ConflictGraph>();

										/*SimulateConflict s{
											instance.selected,
											other_entity,
											selected_action,
											other_selected_action,
											sequence, freq, win, lose, attack, defend, finish };*/

										Service<EventManager>::Get().instant_dispatch<SimulateConflict>(
											instance.selected,
											other_entity,
											selected_action,
											other_selected_action,
											sequence, freq, win, lose, attack, defend, finish);
									}
								}
							}


						}
						break;
						default:
							break;
						}

					}
					ImGui::EndChild();
				}

				// display the back button at the top right
				if (battle_state == BATTLE_STATE::SELECT_ACTION || battle_state == BATTLE_STATE::SELECT_WEAPON)
				{
					if (UI::UIButton_2("Back", "Back", ImVec2{ viewport->Size.x * 0.92f, viewport->Size.y - action_background_size.y * 1.2f }, { 0,0 }, FONT_BODY))
					{
						battle_state = battle_state == BATTLE_STATE::SELECT_ACTION ? BATTLE_STATE::CURR_TURN : BATTLE_STATE::SELECT_ACTION;
					}
				}
			}
			else
			{
				// outlines all units in the map
				for (auto& [x, m] : instance.character_map)
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
							if (x == p_x && y == p_y)
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
							battle_state = BATTLE_STATE::SELECT_WEAPON;
						}
					}
					else
					{
						/*Line l;
						l.p0 = glm::vec3(w_x + .5f - .1, 0, w_y + .5f - .1);
						l.p1 = glm::vec3(w_x + .5f + .1, 0, w_y + .5f + .1);

						Line r;
						r.p0 = glm::vec3(w_x + .5f - .1, 0, w_y + .5f + .1);
						r.p1 = glm::vec3(w_x + .5f + .1, 0, w_y + .5f - .1);
						Service<RenderSystem>::Get().DrawLine(l, { 0,1,0,1 });
						Service<RenderSystem>::Get().DrawLine(r, { 0,1,0,1 });*/

						auto id = instance.character_map[w_x][w_y];

						auto t_position = instance.ecs.get<tc::Transform>(id).position;
						t_position.y += 2;
						auto t_ss = cam.WorldspaceToScreenspace(t_position);
						t_ss.x = (1 + t_ss.x) / 2 * vp.z;
						t_ss.y = vp.w - ((1 + t_ss.y) / 2 * vp.w);
						
						auto temp_cursor = ImGui::GetCursorPos();
						// Draw whatever thing on their head
						{

							ImGui::PushFont(FONT_BOLD);
							auto t_text_size = ImGui::CalcTextSize(ICON_FA_ARROW_ALT_CIRCLE_DOWN);
							auto t_cursor_pos = ImVec2{ t_ss.x - t_text_size.x / 2.f, t_ss.y - t_text_size.y / 2 };
							if (t_cursor_pos.x < viewport->WorkSize.x && t_ss.y + t_text_size.y / 2 < viewport->WorkSize.y) {
								ImGui::SetCursorPos(t_cursor_pos);
								ImGui::Text(ICON_FA_ARROW_ALT_CIRCLE_DOWN);
							}
							ImGui::PopFont();
						}
						ImGui::SetCursorPos(temp_cursor);



						auto& transform = instance.ecs.get<tc::Transform>(instance.selected);

						// rotate the fella if the 
						auto front = transform.rotation * vec3{ 0,0,-1 };
						auto mouse = glm::normalize(vec3{ w_x + .5f, 0, w_y + .5f } - transform.position);
						auto angle = glm::degrees(glm::orientedAngle(mouse, front, vec3{ 0,1,0 }));

						if (angle < -45.5f)
						{
							transform.rotation = transform.rotation * glm::angleAxis(glm::radians(90.f), vec3{ 0,1,0 });
						}
						if (angle > 45.5f)
						{
							transform.rotation = transform.rotation * glm::angleAxis(glm::radians(-90.f), vec3{ 0,1,0 });
						}

						if (io.MouseClicked[0])
						{
							// Attack
							auto attacker = instance.selected;
							other_entity = instance.character_map[w_x][w_y];
							battle_state = BATTLE_STATE::CURR_TURN;

							LOG_ASSERT(instance.ecs.has<tc::Character>(attacker));
							LOG_ASSERT(instance.ecs.has<tc::Character>(other_entity));

							// for now we just cancel
							//state = State::MENU;
						}
					}


				}
			}

		}
		else
			state = State::MENU;
	}
	
	void CombatModeOverlay::moving(RuntimeInstance& instance, const glm::ivec2& world_mouse)
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

			if (visited[w_x][w_y])
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

					auto& transform = instance.ecs.get<tc::Transform>(instance.selected);

					// rotate the fella if the 
					auto front = transform.rotation * vec3{ 0,0,-1 };
					auto mouse = glm::normalize(vec3{ w_x + .5f, 0, w_y + .5f } - transform.position);
					auto angle = glm::degrees(glm::orientedAngle(mouse, front, vec3{0,1,0}));

					if (angle < -45.5f)
					{
						transform.rotation = transform.rotation * glm::angleAxis(glm::radians(90.f), vec3{ 0,1,0 });
					}
					if (angle > 45.5f)
					{
						transform.rotation = transform.rotation * glm::angleAxis(glm::radians(-90.f), vec3{ 0,1,0 });
					}


					if (io.MouseClicked[0])
					{
						// move
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

	void CombatModeOverlay::glimpse(RuntimeInstance& instance)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		// display chance of success

		int value = 0;
		if (win + lose) {
			value = int(100.f * win / (win + lose));
		}

		UI::AttackSuccessUI(instance.ecs.get<tc::Graph>(selected_action).g.name.c_str(), ImVec2{ viewport->Size.x * 0.5f, viewport->Size.y * 0.72f }, value);

		if (UI::UIButton_2("Confirm", "Confirm", ImVec2{ viewport->Size.x * 0.45f, viewport->Size.y * 0.8f }, { -60,0 }, FONT_BODY))
		{
			state = State::FIGHT;
			display_other_stat = false;
			display_curr_stat = false;

			auto sequence = instance.ecs.view_first<tc::ConflictGraph>();

			try_build_all(instance);

			instance.srm.instant_dispatch_to_id<Simulate>(sequence, instance.selected, other_entity, selected_action, other_selected_action);
			if (auto var = instance.srm.get_variable_to_id(sequence, "Win"))
			{
				LOG_ASSERT(var->get_type() == pin_type::Int);
				win = var->get<int>();
			}
			if (auto var = instance.srm.get_variable_to_id(sequence, "AttackRollOutput"))
			{
				atk_output = var->get<int>();
			}
			if (auto var = instance.srm.get_variable_to_id(sequence, "DefendRollOutput"))
			{
				def_output = var->get<int>();
			}
			atk_rolled = false;
			def_rolled = false;
			// clear
			instance.srm.clear();

			selected_action = UNDEFINED;
			other_selected_action = UNDEFINED;
			other_selected_weapon = UNDEFINED;
			selected_weapon = UNDEFINED;
		}

		if (UI::UIButton_2("Cancel", "Cancel", ImVec2{ viewport->Size.x * 0.55f, viewport->Size.y * 0.8f }, { -60,0 }, FONT_BODY))
		{
			//battle_state = BATTLE_STATE::SELECT_OTHER;
			instance.selected = INVALID;
			battle_state = BATTLE_STATE::CURR_TURN; // temp testing
		}
	}

	void CombatModeOverlay::fight(RuntimeInstance& instance)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		if (UI::CharacterTurnData(instance, instance.selected, { 0.f, viewport->Size.y - placeholder_height }, false, true))
		{
			// attacker roll
			atk_rolled = true;
		}

		if (UI::CharacterTurnData(instance, other_entity, { viewport->Size.x, viewport->Size.y - placeholder_height }, true, true))
		{
			// defender roll
			def_rolled = true;
		}


		// draw the combat roll
		auto tex = tex_map["Assets/CombatRollBG.png"];
		ImVec2 point{ 0.0f,viewport->Size.y * 0.2f };
		{
			ImVec2 Min{ point.x, point.y };
			ImVec2 Max{ Min.x + viewport->Size.x , Min.y + viewport->Size.y * 0.3f };
			ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(tex->GetID()), Min, Max);
		}

		auto& attacker = instance.ecs.get<tc::Character>(instance.selected);
		auto& defender = instance.ecs.get<tc::Character>(other_entity);

		// display the character name and rolls
		ImGui::PushFont(FONT_HEAD);

		std::string roll = atk_rolled ? std::to_string(atk_output) : "";
		ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.45f - ImGui::CalcTextSize(attacker.name.c_str()).x, viewport->Size.y * 0.27f });
		ImGui::Text(attacker.name.c_str());
		ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.4f - ImGui::CalcTextSize(roll.c_str()).x , viewport->Size.y * 0.35f });
		ImGui::Text(roll.c_str());

		roll = def_rolled ? std::to_string(def_output) : "";
		ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.5f + ImGui::CalcTextSize(defender.name.c_str()).x * 0.5f, viewport->Size.y * 0.27f });
		ImGui::Text(defender.name.c_str());
		ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.6f + ImGui::CalcTextSize(roll.c_str()).x , viewport->Size.y * 0.35f });
		ImGui::Text(roll.c_str());
		ImGui::PopFont();

		// only trigger this if no more rolls
		if (atk_rolled && def_rolled && UI::UIButton_2("Confirm", "Confirm", ImVec2{ viewport->Size.x * 0.5f, viewport->Size.y * 0.55f }, { 0,0 }, FONT_BODY))
		{
			// TODO: affect the entities

			instance.selected = INVALID;
			battle_state = BATTLE_STATE::CURR_TURN; // temp testing

			//  TODO: change to the next entity turn
			selected_action = UNDEFINED;
			selected_weapon = UNDEFINED;
		}
	}

	void CombatModeOverlay::display_selected(RuntimeInstance& instance, Entity character)
	{
		if (!character || !instance.ecs.valid(character))
			return;

		if (instance.ecs.has<tc::Character>(character))
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			if (!display_curr_stat)
			{
				UI::CharacterTurnData(instance, character, { 0.f, viewport->Size.y - placeholder_height });

				if (UI::UIButton_2("More Information >", "More Information >", ImVec2{ action_background_size.x * 0.72f, viewport->Size.y - action_background_size.y * 1.2f }, { 0.f, 8.0f }, FONT_BODY))
				{
					display_curr_stat = true;
				}
			}
			else
			{
				if (UI::UIButton_2("< Hide", "< Hide", ImVec2{ viewport->Size.x * 0.3f, viewport->Size.y * 0.15f }, { 0.f, 8.0f }, FONT_BODY))
				{
					display_curr_stat = false;
				}

				push_style_color();

				const float infoSize = viewport->Size.y * 0.8f / 3.0f;
				ImGui::SetCursorPos(ImVec2{ 0.f, viewport->Size.y * 0.2f });
				render_more_info(instance, *viewport, INFO_TYPE::CHAR, character);
				ImGui::SetCursorPos(ImVec2{ 0.f, viewport->Size.y * 0.2f + infoSize });
				render_more_info(instance, *viewport, INFO_TYPE::ACTIONS, character);
				ImGui::SetCursorPos(ImVec2{ 0.f, viewport->Size.y * 0.2f + infoSize * 2.0f });
				render_more_info(instance, *viewport, INFO_TYPE::WEAPONS, character);

				pop_style_color();
			}
		}
		
		if (instance.ecs.has<tc::Shape>(character) && instance.ecs.has<tc::Transform>(character))
		{
			// draw existing collider
			{
				auto shape = instance.ecs.get_if<tc::Shape>(character);
				auto& transform = instance.ecs.get<tc::Transform>(character);

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
				if (instance.ecs.has<tc::Door>(character))
					color = { 0,0,1,1 };
				else if (instance.ecs.has<tc::Obstacle>(character))
					color = { 0,0,1,1 };
				else if (instance.ecs.has<tc::Unit>(character))
					color = { 0.1,0.1,0.1,1 };

				Service<RenderSystem>::Get().DrawLine(box, color);
			}
		}
	}

	void CombatModeOverlay::display_selected_right(RuntimeInstance& instance, Entity character)
	{
		if (!character || !instance.ecs.valid(character))
			return;

		if (instance.ecs.has<tc::Character>(character))
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			if (!display_other_stat)
			{
				UI::CharacterTurnData(instance, character, { viewport->Size.x, viewport->Size.y - placeholder_height }, true);
				if (UI::UIButton_2("< More Information", "< More Information", ImVec2{ viewport->Size.x - action_background_size.x * 0.6f, viewport->Size.y - action_background_size.y * 1.2f }, { 0.f, 8.0f }, FONT_BODY))
				{
					display_other_stat = true;
				}
			}

			else
			{
				if (UI::UIButton_2("Hide >", "Hide >", ImVec2{ viewport->Size.x * 0.7f, viewport->Size.y * 0.15f }, { 0.f, 8.0f }, FONT_BODY))
				{
					display_other_stat = false;
				}

				push_style_color();

				const float infoSize = viewport->Size.y * 0.8f / 3.0f;
				ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.7f, viewport->Size.y * 0.2f });
				render_more_info(instance, *viewport, INFO_TYPE::CHAR, character);
				ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.7f, viewport->Size.y * 0.2f + infoSize });
				render_more_info(instance, *viewport, INFO_TYPE::ACTIONS, character);
				ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.7f, viewport->Size.y * 0.2f + infoSize * 2.0f });
				render_more_info(instance, *viewport, INFO_TYPE::WEAPONS, character);

				pop_style_color();
			}

		}

		if (instance.ecs.has<tc::Shape>(character) && instance.ecs.has<tc::Transform>(character))
		{
			// draw existing collider
			{
				auto shape = instance.ecs.get_if<tc::Shape>(character);
				auto& transform = instance.ecs.get<tc::Transform>(character);

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
				if (instance.ecs.has<tc::Door>(character))
					color = { 0,0,1,1 };
				else if (instance.ecs.has<tc::Obstacle>(character))
					color = { 0,0,1,1 };
				else if (instance.ecs.has<tc::Unit>(character))
					color = { 0.1,0.1,0.1,1 };

				Service<RenderSystem>::Get().DrawLine(box, color);
			}
		}
	}

	void CombatModeOverlay::open_popup(const Event& e)
	{
		auto a = event_cast<OpenCombatModeTrigger>(e);

		OverlayOpen = true;
		window_flags |= ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBackground;

		// hardget the entities
		units = a.units;

		placeholder_height = (float)tex_map["Assets/Placeholder_Character.png"]->GetHeight();
		action_background_size = ImVec2{ tex_map["Assets/ActionBackdrop.png"]->GetWidth() * 1.0f, tex_map["Assets/ActionBackdrop.png"]->GetHeight() * 1.0f};

		action_button_diff = (float)tex_map["Assets/SkillSelected.png"]->GetWidth() - (float)tex_map["Assets/SkillUnselected.png"]->GetWidth();
		battle_state = BATTLE_STATE::CURR_TURN; 

		auto& cam = Service<RenderSystem>::Get().GetCamera();
		cam_ctrl.set_fixed_camera(cam, 0 , 70);

		selected_action = UNDEFINED;
		other_selected_action = UNDEFINED;
		other_selected_weapon = UNDEFINED;
		selected_weapon = UNDEFINED;
	}

	void CombatModeOverlay::visibility(const Event& e)
	{
		OverlayOpen = event_cast<CombatModeVisibility>(e).isVisible;
		action_button_diff = (float)tex_map["Assets/SkillSelected.png"]->GetWidth() - tex_map["Assets/SkillUnselected.png"]->GetWidth();
		battle_state = BATTLE_STATE::CURR_TURN;
	}

	void CombatModeOverlay::show(Instance& instance)
	{
		if (!dynamic_cast<RuntimeInstance*>(&instance))
			return;

		auto& runtime = dynamic_cast<RuntimeInstance&>(instance);

        const ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		if (OverlayOpen)
		{
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

				ImGui::Dummy(ImVec2{ 5.0f,0.f });
				bool first = true;
				float padding = 0.0f;
				for (auto id : units)
				{
					// check if turn is over
					UI::CharacterTurn(instance, id, { 0.f, ImGui::GetCursorPosY() + padding }, instance.selected == id);
					padding += 85.0f;
					first = false;
				}
				
				if (!instance.selected || !instance.ecs.valid(instance.selected))
				{
					state = State::NO_SELECTED;
					other_entity = INVALID;
				}

				switch (state)
				{
				case State::NO_SELECTED:
					no_selected(runtime, world_mouse);
					break;
				case State::MENU:
					display_selected(runtime, instance.selected);
					menu(runtime, world_mouse);
					break;
				case State::ATTACKING:
					display_selected(runtime, other_entity);
					attacking(runtime, world_mouse);
					break;
				case State::MOVING:
					display_selected(runtime, instance.selected);
					moving(runtime, world_mouse);
					break;
				case State::GLIMPSE:
					display_selected(runtime, instance.selected);
					display_selected_right(runtime, other_entity);
					glimpse(runtime);
					break;
				case State::FIGHT:
					fight(runtime);
					break;
				default:
					break;
				}
			}

			if (UI::UIButton_2("Turn Order", "Turn Order", ImVec2{ viewport->Size.x * 0.8f, viewport->Size.y * 0.06f }, { 10.f,10.f }, FONT_PARA))
			{
				Service<EventManager>::Get().instant_dispatch<OpenTurnOrderOverlay>();
				Service<EventManager>::Get().instant_dispatch<CombatModeVisibility>(false);
			}

			if (UI::UIButton_2("Place Units", "Place Units", ImVec2{ viewport->Size.x * 0.9f, viewport->Size.y * 0.06f }, { 10.f,10.f }, FONT_PARA))
			{

			}

			ImGui::End();

			ImGui::PopStyleVar();
			ImGui::PopStyleColor(2);
		}


		//if (OverlayOpen)
		//{
		//	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		//	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 0.f});

		//	if (ImGui::Begin("Combat Mode Screen", nullptr, window_flags))
		//	{
		//		// top bar

		//		if (curr_entity != UNDEFINED)
		//		{
		//			ImGui::Dummy(ImVec2{ 5.0f,0.f });

		//			// render the entity stat

		//			if (!display_curr_stat)
		//			{
		//				// TODO: get from the added list
		//				if (battle_state == BATTLE_STATE::CURR_TURN || battle_state == BATTLE_STATE::SELECT_ACTION || battle_state == BATTLE_STATE::SELECT_WEAPON)
		//				{
		//					bool first = true;
		//					float padding = 0.0f;
		//					for (auto id : units)
		//					{
		//						// check if turn is over
		//						UI::CharacterTurn(instance, id, { 0.f, ImGui::GetCursorPosY() + padding }, first);
		//						padding += 100.0f;
		//						first = false;
		//					}
		//				}
		//			
		//				if (battle_state != BATTLE_STATE::SELECT_OTHER && battle_state != BATTLE_STATE::COMMENCE_BATTLE)
		//				{
		//					UI::CharacterTurnData(instance, curr_entity, { 0.f, viewport->Size.y - placeholder_height });

		//					if (UI::UIButton_2("More Information >", "More Information >", ImVec2{ action_background_size.x * 0.72f, viewport->Size.y - action_background_size.y * 1.2f }, { 0.f, 8.0f }, FONT_BODY))
		//					{
		//						display_curr_stat = true;
		//					}
		//				}
		//			}
		//			else
		//			{
		//				if (UI::UIButton_2("< Hide", "< Hide", ImVec2{ viewport->Size.x * 0.3f, viewport->Size.y * 0.15f }, { 0.f, 8.0f }, FONT_BODY))
		//				{
		//					display_curr_stat = false;
		//				}

		//				push_style_color();

		//				const float infoSize = viewport->Size.y * 0.8f / 3.0f;
		//				ImGui::SetCursorPos(ImVec2{ 0.f, viewport->Size.y * 0.2f});
		//				render_more_info(instance, *viewport, INFO_TYPE::CHAR, curr_entity);
		//				ImGui::SetCursorPos(ImVec2{ 0.f, viewport->Size.y * 0.2f + infoSize }); 
		//				render_more_info(instance, *viewport, INFO_TYPE::ACTIONS, curr_entity);
		//				ImGui::SetCursorPos(ImVec2{ 0.f, viewport->Size.y * 0.2f + infoSize * 2.0f});
		//				render_more_info(instance, *viewport, INFO_TYPE::WEAPONS, curr_entity);

		//				pop_style_color();
		//			}

		//			if (battle_state == BATTLE_STATE::CURR_TURN || battle_state == BATTLE_STATE::SELECT_ACTION || battle_state == BATTLE_STATE::SELECT_WEAPON)
		//			{
		//				UI::ActionUI(ImVec2{ viewport->Size.x, viewport->Size.y - action_background_size.y }, battle_state == BATTLE_STATE::SELECT_WEAPON ? "SELECT A WEAPON" : "SELECT AN ACTION");
		//				ImGui::SetCursorPos(ImVec2{ viewport->Size.x - action_background_size.x * 0.85f , viewport->Size.y - action_background_size.y * 0.7f });
		//				if (ImGui::BeginChild("Action content", ImVec2{ action_background_size.x * 0.85f, action_background_size.y * 0.7f }, true, ImGuiWindowFlags_NoScrollbar))
		//				{
		//					switch (battle_state)
		//					{
		//					case Tempest::CombatModeOverlay::BATTLE_STATE::CURR_TURN:
		//					{
		//						// action
		//						if (UI::UIButton_2("ACTION", "ACTION", ImVec2{ ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.3f, ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y * 0.5f }, { -150,80 }, FONT_SHEAD))
		//						{
		//							battle_state = BATTLE_STATE::SELECT_ACTION;
		//						}

		//						ImGui::SameLine();
		//						const ImVec2 cursor{ ImGui::GetCursorPos() };
		//						const ImVec2 avail_region{ ImGui::GetContentRegionAvail() };
		//						if (UI::UIButton_2("ITEMS", "ITEMS", ImVec2{ cursor.x + avail_region.x * 0.5f, cursor.y - avail_region.y * 0.25f }, { -200,10 }, FONT_SHEAD))
		//						{

		//						}

		//						if (UI::UIButton_2("MOVE", "MOVE", ImVec2{ cursor.x + avail_region.x * 0.5f, cursor.y + avail_region.y * 0.6f }, { -200,10 }, FONT_SHEAD))
		//						{

		//						}
		//					}
		//					break;
		//					case Tempest::CombatModeOverlay::BATTLE_STATE::SELECT_ACTION: // TODO: account for no actions
		//					{
		//						auto& charac = instance.ecs.get<tc::Character>(curr_entity);
		//						unsigned i = 0;
		//						float xpos = ImGui::GetCursorPosX() + 60.0f;
		//						for (auto id : charac.actions)
		//						{
		//							auto& action = instance.ecs.get<tc::Graph>(id);

		//							ImGui::SetCursorPos(ImVec2{ selected_action == id ? xpos - action_button_diff : xpos, ImGui::GetCursorPosY() });
		//							if (UI::UIActionButton(action.g.name.c_str(), "##ACTIONSTUFF" + i++, selected_action == id))
		//							{
		//								selected_action = id;
		//								battle_state = BATTLE_STATE::SELECT_WEAPON;
		//							}
		//						}
		//					}
		//					break;
		//					case Tempest::CombatModeOverlay::BATTLE_STATE::SELECT_WEAPON: // TODO: account for no weapons
		//					{
		//						auto& charac = instance.ecs.get<tc::Character>(curr_entity);
		//						unsigned i = 0;
		//						float xpos = ImGui::GetCursorPosX() + 60.0f;
		//						for (auto id : charac.weapons)
		//						{
		//							auto& action = instance.ecs.get<tc::Weapon>(id);

		//							ImGui::SetCursorPos(ImVec2{ selected_weapon == id ? xpos - action_button_diff : xpos, ImGui::GetCursorPosY() });
		//							if (UI::UIActionButton(action.name.c_str(), "##WEAPONSTUFF" + i++, selected_weapon == id))
		//							{
		//								selected_weapon = id;
		//								battle_state = BATTLE_STATE::SELECT_OTHER;
		//							}
		//						}
		//					}
		//					break;
		//					default:
		//						break;
		//					}

		//				}
		//				ImGui::EndChild();
		//			}

		//			// display the back button at the top right
		//			if (battle_state == BATTLE_STATE::SELECT_ACTION || battle_state == BATTLE_STATE::SELECT_WEAPON)
		//			{
		//				if (UI::UIButton_2("Back", "Back", ImVec2{ viewport->Size.x * 0.92f, viewport->Size.y - action_background_size.y * 1.2f }, { 0,0 }, FONT_BODY))
		//				{
		//					battle_state = battle_state == BATTLE_STATE::SELECT_ACTION ? BATTLE_STATE::CURR_TURN : BATTLE_STATE::SELECT_ACTION;
		//				}
		//			}

		//			// display the end turn button
		//			else if (battle_state == BATTLE_STATE::CURR_TURN)
		//			{
		//			}

		//			switch (battle_state)
		//			{
		//			case Tempest::CombatModeOverlay::BATTLE_STATE::SELECT_OTHER:
		//			{
		//				// go to select the thing
		//				battle_state = BATTLE_STATE::BATTLE_GLIMPSE;
		//			}
		//			break;
		//			case Tempest::CombatModeOverlay::BATTLE_STATE::BATTLE_GLIMPSE:
		//			{
		//				// display other character
		//				if (!display_other_stat)
		//				{
		//					UI::CharacterTurnData(instance, other_entity, { viewport->Size.x, viewport->Size.y - placeholder_height }, true);
		//					if (UI::UIButton_2("< More Information", "< More Information", ImVec2{ viewport->Size.x - action_background_size.x * 0.6f, viewport->Size.y - action_background_size.y * 1.2f }, { 0.f, 8.0f }, FONT_BODY))
		//					{
		//						display_other_stat = true;
		//					}
		//				}

		//				else
		//				{
		//					if (UI::UIButton_2("Hide >", "Hide >", ImVec2{ viewport->Size.x * 0.7f, viewport->Size.y * 0.15f }, { 0.f, 8.0f }, FONT_BODY))
		//					{
		//						display_other_stat = false;
		//					}

		//					push_style_color();

		//					const float infoSize = viewport->Size.y * 0.8f / 3.0f;
		//					ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.7f, viewport->Size.y * 0.2f });
		//					render_more_info(instance, *viewport, INFO_TYPE::CHAR, other_entity);
		//					ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.7f, viewport->Size.y * 0.2f + infoSize });
		//					render_more_info(instance, *viewport, INFO_TYPE::ACTIONS, other_entity);
		//					ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.7f, viewport->Size.y * 0.2f + infoSize * 2.0f });
		//					render_more_info(instance, *viewport, INFO_TYPE::WEAPONS, other_entity);

		//					pop_style_color();
		//				}

		//				// display chance of success
		//				UI::AttackSuccessUI(instance.ecs.get<tc::Graph>(selected_action).g.name.c_str(), ImVec2{viewport->Size.x * 0.5f, viewport->Size.y * 0.72f },100);

		//				if (UI::UIButton_2("Confirm", "Confirm", ImVec2{ viewport->Size.x * 0.45f, viewport->Size.y * 0.8f }, { -60,0 }, FONT_BODY))
		//				{
		//					battle_state = BATTLE_STATE::COMMENCE_BATTLE;
		//					display_other_stat = false;
		//					display_curr_stat = false;
		//				}

		//				if (UI::UIButton_2("Cancel", "Cancel", ImVec2{ viewport->Size.x * 0.55f, viewport->Size.y * 0.8f }, { -60,0 }, FONT_BODY))
		//				{
		//					//battle_state = BATTLE_STATE::SELECT_OTHER;
		//					battle_state = BATTLE_STATE::CURR_TURN; // temp testing
		//				}
		//			}
		//			break;
		//			case Tempest::CombatModeOverlay::BATTLE_STATE::COMMENCE_BATTLE:
		//			{
		//				if (UI::CharacterTurnData(instance, curr_entity, { 0.f, viewport->Size.y - placeholder_height }, false, true))
		//				{
		//					// attacker roll
		//				}

		//				if (UI::CharacterTurnData(instance, other_entity, { viewport->Size.x, viewport->Size.y - placeholder_height }, true, true))
		//				{
		//					// defender roll
		//				}
		//				
		//				// draw the combat roll
		//				auto tex = tex_map["Assets/CombatRollBG.png"];
		//				ImVec2 point{ 0.0f,viewport->Size.y * 0.2f };
		//				{
		//					ImVec2 Min{ point.x, point.y };
		//					ImVec2 Max{ Min.x + viewport->Size.x , Min.y + viewport->Size.y * 0.3f};
		//					ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(tex->GetID()), Min, Max);
		//				}

		//				auto& attacker = instance.ecs.get<tc::Character>(curr_entity);
		//				auto& defender = instance.ecs.get<tc::Character>(curr_entity);

		//				// display the character name and rolls
		//				ImGui::PushFont(FONT_HEAD);

		//				std::string roll = "34";
		//				ImGui::SetCursorPos(ImVec2{viewport->Size.x * 0.4f - ImGui::CalcTextSize(attacker.name.c_str()).x *0.5f, viewport->Size.y * 0.27f });
		//				ImGui::Text(attacker.name.c_str());
		//				ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.4f - ImGui::CalcTextSize(roll.c_str()).x * 0.5f, viewport->Size.y * 0.35f });
		//				ImGui::Text(roll.c_str());

		//				roll = "9";
		//				ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.6f - ImGui::CalcTextSize(defender.name.c_str()).x * 0.5f, viewport->Size.y * 0.27f });
		//				ImGui::Text(defender.name.c_str());
		//				ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.6f - ImGui::CalcTextSize(roll.c_str()).x * 0.5f, viewport->Size.y * 0.35f });
		//				ImGui::Text(roll.c_str());
		//				ImGui::PopFont();

		//				// only trigger this if no more rolls
		//				if (UI::UIButton_2("Confirm", "Confirm", ImVec2{viewport->Size.x * 0.5f, viewport->Size.y * 0.55f}, { 0,0 }, FONT_BODY))
		//				{
		//					// TODO: affect the entities

		//					battle_state = BATTLE_STATE::CURR_TURN;

		//					//  TODO: change to the next entity turn
		//					selected_action = UNDEFINED;
		//					selected_weapon = UNDEFINED;
		//				}

		//			}
		//			break;
		//			default:
		//				break;
		//			}
		//		}

		//		if (UI::UIButton_2("Turn Order", "Turn Order", ImVec2{ viewport->Size.x * 0.8f, viewport->Size.y * 0.06f }, { 10.f,10.f }, FONT_PARA))
		//		{
		//			Service<EventManager>::Get().instant_dispatch<OpenTurnOrderOverlay>();
		//			Service<EventManager>::Get().instant_dispatch<CombatModeVisibility>(false);
		//		}

		//		if (UI::UIButton_2("Place Units", "Place Units", ImVec2{ viewport->Size.x * 0.9f, viewport->Size.y * 0.06f }, { 10.f,10.f }, FONT_PARA))
		//		{

		//		}
		//	}

  //          ImGui::End();

		//	ImGui::PopStyleVar();
  //          ImGui::PopStyleColor();
  //          
		//}
	}

	void CombatModeOverlay::render_more_info(Instance& instance, const ImGuiViewport& viewport, INFO_TYPE type, Entity entity)
	{
		const int identifier = static_cast<int>(type) + static_cast<int>(entity * 2.5f); // this is a scam
		if (ImGui::BeginChild(std::string("Char more info" + std::to_string(identifier)).c_str(), ImVec2{ viewport.Size.x * 0.3f, viewport.Size.y * 0.8f / 3.0f }, true))
		{
			auto& charac = instance.ecs.get<tc::Character>(entity);

			ImVec2 winMin = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y };
			ImVec2 TextMin = { ImGui::GetWindowPos().x + 10.f, ImGui::GetWindowPos().y + 5.f };
			ImVec2 winMax = { winMin.x + ImGui::GetWindowWidth() * 0.5f, winMin.y + ImGui::GetWindowHeight() * 0.1f };
			ImVec4 col = { 0.980f, 0.768f, 0.509f, 1.f };
			ImVec4 textcol = { 0,0,0,1 };
			if (!ImGui::IsWindowFocused())
			{
				col = { 0.980f, 0.768f, 0.509f, 0.7f };
				textcol = { 0,0,0,0.7f };
			}

			ImGui::GetWindowDrawList()->AddRectFilled({ winMin.x, winMin.y }, { winMax.x, winMax.y }, ImGui::GetColorU32(col));
			ImGui::PushFont(FONT_OPEN);
			switch (type)
			{
			case Tempest::CombatModeOverlay::INFO_TYPE::CHAR:
				ImGui::GetWindowDrawList()->AddText({ TextMin.x, TextMin.y }, ImGui::GetColorU32({ 0,0,0,1 }), charac.name.c_str());
				break;
			case Tempest::CombatModeOverlay::INFO_TYPE::ACTIONS:
				ImGui::GetWindowDrawList()->AddText({ TextMin.x, TextMin.y }, ImGui::GetColorU32({ 0,0,0,1 }), "ACTIONS");
				break;
			case Tempest::CombatModeOverlay::INFO_TYPE::WEAPONS:
				ImGui::GetWindowDrawList()->AddText({ TextMin.x, TextMin.y }, ImGui::GetColorU32({ 0,0,0,1 }), "WEAPONS");
				break;
			default:
				break;
			}

			ImGui::Dummy(ImVec2{0, 30.0f});

			if (ImGui::BeginChild(std::string("Internal content" + std::to_string(identifier)).c_str(), ImVec2{ ImGui::GetContentRegionAvailWidth() * 0.95f, ImGui::GetContentRegionAvail().y * 0.8f }, true))
			{
				ImVec2 cursor{ ImGui::GetCursorPosX() + 100.0f, ImGui::GetCursorPosY() + 30.0f };
				unsigned i = 0; 
				unsigned j = 0;
				switch (type)
				{
				case Tempest::CombatModeOverlay::INFO_TYPE::CHAR:
				{
					auto StatsView = instance.ecs.view<Components::Statline>(exclude_t<tc::Destroyed>());
					Entity StateLineId = UNDEFINED;
					for (auto id : StatsView)
						StateLineId = id;
					auto sl = instance.ecs.get_if<tc::Statline>(StateLineId);

					ImGui::Dummy(ImVec2{ 0.0f, 10.0f });
					ImGui::PushFont(FONT_BODY);
					bool NextLine = false;
					ImVec2 PrevPos{ 0.f ,0.f };
					float frontPadding = 5.f;
					for (auto counter = 0; counter < sl->size(); counter++)
					{
						if ((*sl)(counter))
						{
							string stat = sl->operator[](counter) + " " + std::to_string(charac.get_stat(counter));

							if (!NextLine)
							{
								ImGui::Dummy({ frontPadding * 15, 0 });
								ImGui::SameLine();
								PrevPos = ImGui::GetCursorPos();
								ImGui::Text(stat.c_str());
								ImGui::Dummy({ frontPadding * 15, 0 });
								ImGui::SameLine();
							}

							else
							{
								ImGui::SetCursorPos(PrevPos);
								ImGui::Dummy({ 250.f - frontPadding * 15, 0.f });
								ImGui::SameLine();

								ImGui::Text(stat.c_str());
								ImGui::Dummy({ 250 + frontPadding, 0 });
								ImGui::SameLine();

								ImGui::SetCursorPos(PrevPos);
								ImGui::Dummy({ 0, 50.f });
							}

							NextLine = !NextLine;

						}

					}
					ImGui::PopFont();
				}
					// display stas
					break;
				case Tempest::CombatModeOverlay::INFO_TYPE::ACTIONS:
					// display actions
					for (auto id : charac.actions)
					{
						auto& action = instance.ecs.get<tc::Graph>(id);
						if (UI::UIButton_2(action.g.name.c_str(), action.g.name.c_str(), { cursor.x + i++ * 190, cursor.y + j * 50 }, { 0,0 }, FONT_PARA))
						{
							// go edit page
						}

						if (i / 2)
						{
							i = 0;
							++j;
						}
					}
					break;
				case Tempest::CombatModeOverlay::INFO_TYPE::WEAPONS:
					// display weapons
					for (auto id : charac.weapons)
					{
						auto& weapon = instance.ecs.get<tc::Weapon>(id);
						if (UI::UIButton_2(weapon.name.c_str(), weapon.name.c_str(), { cursor.x + i++ * 190, cursor.y + j * 50 }, { 0,0 }, FONT_PARA))
						{
							// go edit page
						}

						if (i / 2)
						{
							i = 0;
							++j;
						}
					}
					break;
				default:
					break;
				}
			}
			ImGui::EndChild();
			
			ImGui::PopFont();

			// display the edit here	
		}
		ImGui::EndChild();
	}

	void CombatModeOverlay::push_style_color() const
	{
		const ImVec4 borderCol = { 0.980f, 0.768f, 0.509f, 1.f };
		ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 2.f);
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.f });
		ImGui::PushStyleColor(ImGuiCol_Border, borderCol);
		ImGui::PushStyleColor(ImGuiCol_ChildBg, { 0.06f,0.06f, 0.06f, 0.85f });
	}

	void  CombatModeOverlay::pop_style_color() const
	{
		ImGui::PopStyleColor(2);
		ImGui::PopStyleVar(3);
	}
}