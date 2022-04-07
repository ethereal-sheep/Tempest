/**********************************************************************************
* \author		Huang Xurong(h.xurong@digipen.edu),
				Lim Ziyi Jean(ziyijean.lim@digipen.edu)
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
#include "Util/shape_manip.h"
#include "Audio/AudioEngine.h"
#include "Util/pathfinding.h"

#include "Particles/Particles_2D/EmitterSystem_2D.h"
#include "Particles/Particles_2D/LineEmitter_2D.h"
#include "Particles/Particles_2D/ExplosionEmitter_2D.h"

#include "Particles/Particles_3D/EmitterSystem_3D.h"
#include "Particles/Particles_3D/Unit_Turn_IndicatorEmitter_3D.h"
#include "Particles//Particles_3D/CharacterDamageEmitter_3D.h"
#include "Particles/Particles_3D/CharacterDeathEmitter_3D.h"
#include "Particles/Particles_3D/CharacterTileCharged_Emitter_3D.h"

namespace Tempest
{
	static float timer = 0.f;

	auto collide_first_edited(
		int x0, int y0, int x1, int y1,
		tmap<int, tmap<int, id_t>>& collision_map,
		tmap<int, tmap<int, tmap<int, tmap<int, id_t>>>>& wall_map,
		tmap<int, tmap<int, id_t>>& character_map) {

		auto raytrace = [](int x0, int y0, int x1, int y1, auto visit)
		{
			int dx = abs(x1 - x0);
			int dy = abs(y1 - y0);
			int x = x0;
			int y = y0;
			int n = dx + dy;
			int x_inc = (x1 > x0) ? 1 : -1;
			int y_inc = (y1 > y0) ? 1 : -1;
			int error = dx - dy;
			dx *= 2;
			dy *= 2;

			tvector<std::tuple<int, int, int, int>> covered;

			for (; n > 0; --n)
			{
				if (error == 0)
				{

					// first move
					auto x_level = visit(x, y, x, y + y_inc);
					auto y_level = visit(x, y, x + x_inc, y);
					
					if (x_level || y_level)
					{
						covered.push_back(std::make_tuple(x, y, x, y + y_inc));
						if(x_level == 1 || y_level == 1)
							return std::make_tuple(covered, true);
					}

					x_level = visit(x, y + y_inc, x + x_inc, y + y_inc);
					y_level = visit(x + x_inc, y, x + x_inc, y + y_inc);

					if (x_level || y_level)
					{
						covered.push_back(std::make_tuple(x, y + y_inc, x + x_inc, y + y_inc));
						if (x_level == 1 || y_level == 1)
							return std::make_tuple(covered, true);
					}

					x += x_inc;
					y += y_inc;
					error -= dy;
					error += dx;

					--n;
				}
				else if (error > 0)
				{
					auto level = visit(x, y, x + x_inc, y);
					if (level)
					{
						covered.push_back(std::make_tuple(x, y, x + x_inc, y));
						if(level == 1)
							return std::make_tuple(covered, true);
					}
					x += x_inc;
					error -= dy;
				}
				else
				{
					auto level = visit(x, y, x, y + y_inc);
					if (level)
					{
						covered.push_back(std::make_tuple(x, y, x, y + y_inc));
						if (level == 1)
							return std::make_tuple(covered, true);
					}

					y += y_inc;
					error += dx;
				}
			}
			return std::make_tuple(covered, false);
		};
		tmap<int, tmap<int, bool>> intersects;
		return raytrace(x0, y0, x1, y1,
			[&collision_map, &wall_map, &character_map, &intersects](int x, int y, int n_x, int n_y) {

				uint32_t level = INT_MAX;

				if (wall_map[x][y][n_x][n_y])
					level = std::min(wall_map[x][y][n_x][n_y], level);
				if (collision_map[n_x][n_y])
					level = std::min(collision_map[n_x][n_y], level);

				return level == INT_MAX ? 0 : level;
			});
	};

	auto collide_first(
		int x0, int y0, int x1, int y1,
		tmap<int, tmap<int, id_t>>& collision_map,
		tmap<int, tmap<int, tmap<int, tmap<int, id_t>>>>& wall_map,
		tmap<int, tmap<int, id_t>>& character_map)
	{

		auto raytrace = [](int x0, int y0, int x1, int y1, auto visit)
		{
			int dx = abs(x1 - x0);
			int dy = abs(y1 - y0);
			int x = x0;
			int y = y0;
			int n = dx + dy;
			int x_inc = (x1 > x0) ? 1 : -1;
			int y_inc = (y1 > y0) ? 1 : -1;
			int error = dx - dy;
			dx *= 2;
			dy *= 2;

			for (; n > 0; --n)
			{
				if (error == 0)
				{
					if (!visit(x, y, x, y + y_inc)) return std::make_tuple(x, y, x, y + y_inc, true);

					if (!visit(x, y, x + x_inc, y)) return std::make_tuple(x, y, x + x_inc, y, true);

					if (!visit(x, y + y_inc, x + x_inc, y + y_inc)) return std::make_tuple(x, y + y_inc, x + x_inc, y + y_inc, true);

					if (!visit(x + x_inc, y, x + x_inc, y + y_inc)) return std::make_tuple(x + x_inc, y, x + x_inc, y + y_inc, true);

					x += x_inc;
					y += y_inc;
					error -= dy;
					error += dx;

					--n;
				}
				else if (error > 0)
				{
					if (!visit(x, y, x + x_inc, y)) return std::make_tuple(x, y, x + x_inc, y, true);
					x += x_inc;
					error -= dy;
				}
				else
				{
					if (!visit(x, y, x, y + y_inc)) return std::make_tuple(x, y, x, y + y_inc, true);
					y += y_inc;
					error += dx;
				}
			}
			return std::make_tuple(0, 0, 0, 0, false);
		};
		tmap<int, tmap<int, bool>> intersects;
		return raytrace(x0, y0, x1, y1,
			[&collision_map, &wall_map, &character_map, &intersects](int x, int y, int n_x, int n_y) {

				if (wall_map[x][y][n_x][n_y])
					return false;
				if (collision_map[n_x][n_y])
					return false;
				return true;
			});
	}

	void calculate_range(RuntimeInstance& instance, int p_x, int p_y, int range_u, tmap<int, tmap<int, uint32_t>>& range_map, tmap<int, tmap<int, bool>>& visited)
	{
		range_map.clear();
		visited.clear();
		tmap<int, tmap<int, uint32_t>> distance;
		{
			//bfs
			const uint32_t range = range_u < 0 ? 0 : static_cast<uint32_t>(range_u);
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
					if (instance.character_map[p.x + x][p.y + y])
					{
						visited[p.x + x][p.y + y] = true;
						distance[p.x + x][p.y + y] = new_dist;
						continue;
					}
					if (instance.collision_map[p.x + x][p.y + y])
						continue;
					if (instance.wall_map[p.x][p.y][p.x + x][p.y + y])
						continue;
					if (visited[p.x + x][p.y + y])
						continue;

					distance[p.x + x][p.y + y] = new_dist;
					q.push({ p.x + x, p.y + y });
				}

			}

			for (auto& [x, m] : visited)
				for (auto [y, ignore] : m)
				{
					auto [v, b] = collide_first_edited(p_x, p_y, x, y, instance.collision_map, instance.wall_map, instance.character_map);

					if (v.empty())
					{
						range_map[x][y] = 0; // hit nothing
					}
					else if (b)
					{
						auto& [x0, y0, x1, y1] = v.back();
						if (x == x1 && y == y1 && instance.character_map[x1][y1] && !instance.wall_map[x0][y0][x1][y1])
						{
							range_map[x][y] = (unsigned)v.size() + 2; // hit character
						}
						else
							range_map[x][y] = 1; // hit collider
					}
					else
					{
						range_map[x][y] = 2; // hit cover
					}

					/*if (!c)
						range_map[x][y] = 0;
					else if (x == x1 && y == y1 && instance.character_map[x1][y1] && !instance.wall_map[x0][y0][x1][y1])
						range_map[x][y] = 1;
					else
						range_map[x][y] = 2;*/

				}
		}
	}

	void calculate_move(RuntimeInstance& instance, int p_x, int p_y, int move_u, tmap<int, tmap<int, uint32_t>> distance, tmap<int, tmap<int, bool>>& visited)
	{
		distance.clear();
		visited.clear();

		{
			//bfs
			const uint32_t range = move_u < 0 ? 0 : static_cast<uint32_t>(move_u);
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
					if (instance.wall_map[p.x][p.y][p.x + x][p.y + y])
						continue;
					if (visited[p.x + x][p.y + y])
						continue;

					distance[p.x + x][p.y + y] = new_dist;
					q.push({ p.x + x, p.y + y });
				}

			}
		}
	}

	void draw_range(const tmap<int, tmap<int, uint32_t>>& range_map)
	{
		auto& cam = Service<RenderSystem>::Get().GetCamera();

		auto drawbox1 = [&](int x, int y, bool b, auto grey, bool second) {
			if (!b) return;
			AABB box;

			box.min.x = (float)x + .05f;
			box.min.z = (float)y + .05f;
			box.min.y = 0;

			box.max.x = x + .95f;
			box.max.z = y + .95f;
			box.max.y = 0;
			Service<RenderSystem>::Get().DrawLine(box, { grey, grey, grey, 1 });

			if (second)
			{
				AABB box2;
				box2.min.x = (float)x + .15f;
				box2.min.z = (float)y + .15f;
				box2.min.y = 0;

				box2.max.x = x + .85f;
				box2.max.z = y + .85f;
				box2.max.y = 0;

				Service<RenderSystem>::Get().DrawLine(box2, { grey, grey, grey, 1 });
			}
		};
		auto drawbox2 = [&](int x, int y, bool b) {
			if (!b) return;
			AABB box;

			box.min.x = (float)x + .05f;
			box.min.z = (float)y + .05f;
			box.min.y = 0;

			box.max.x = x + .95f;
			box.max.z = y + .95f;
			box.max.y = 0;

			AABB box2;

			box2.min.x = (float)x + .0f;
			box2.min.z = (float)y + .0f;
			box2.min.y = 0;

			box2.max.x = x + 1.0f;
			box2.max.z = y + 1.0f;
			box2.max.y = 0;

			Service<RenderSystem>::Get().DrawLine(box, { 1,0,0,1 });
			Service<RenderSystem>::Get().DrawLine(box2, { 1,0,0,1 });
		};
		
		auto drawbox = [&](int x, int y, ImU32 color)
		{

			auto WorldSpaceAABBtoSSVecOfPts = [](Camera& cam, AABB aabb)
			{
				tvector<vec3> ws_pts{ aabb.min, vec3{aabb.min.x, 0.f, aabb.max.z}, aabb.max, vec3{aabb.max.x, 0.f, aabb.min.z} };
				tvector<ImVec2> ss_pts;
				auto vp = cam.GetViewport();
				for (auto pt : ws_pts)
				{
					auto t_ss = cam.WorldspaceToScreenspace(pt);
					t_ss.x = (1 + t_ss.x) / 2 * vp.z;
					t_ss.y = vp.w - ((1 + t_ss.y) / 2 * vp.w);

					ss_pts.push_back(ImVec2{ t_ss.x, t_ss.y });
				}

				return ss_pts;
			};

			AABB box;
			box.min.x = (float)x + .1f;
			box.min.z = (float)y + .1f;
			box.min.y = 0;

			box.max.x = x + .9f;
			box.max.z = y + .9f;
			box.max.y = 0;

			auto pts = WorldSpaceAABBtoSSVecOfPts(cam, box);

			ImGui::GetBackgroundDrawList()->AddConvexPolyFilled(pts.data(), 4, color);

		};

		for (auto& [x, m] : range_map)
			for (auto [y, i] : m)
			{
				if (i == 0) // hit nothing dark green
					drawbox(x, y, IM_COL32(0x20, 0x20, 0x20, 0x20));
				else if (i == 1) // hit collider dark red
					drawbox(x, y, IM_COL32(0x20, 0x20, 0x20, 0x20));
				else if (i == 2) // hit cover only dark red
					drawbox(x, y, IM_COL32(0x20, 0x20, 0x20, 0x20));
				else if (i == 3) // hit player
					drawbox(x, y, IM_COL32(0xAF, 0x20, 0x20, 0x60));
				else // hit player behind cover
				{
					drawbox(x, y, IM_COL32(0xAF, 0x80, 0x80, 0x20));
				}

			}
	}
	
	void draw_indicator(RuntimeInstance& instance, vec3 pos)
	{
		auto height = 2.5f;
		auto amplitude = .1f;
		auto speed = 2.f;
		auto ring_speed = 2.f;
		auto spin_speed = 2.f;
		auto dice_speed = 4.f;

		pos.y = height;
		pos.y += (float)sin(timer * speed) * amplitude;

		if (auto pf = instance.scene.get_prototype_if("Unit", "Indicator"))
		{
			auto model = pf->get_if<tc::Model>();
			auto local = pf->get_if<tc::Local>();

			auto test = glm::translate(pos)
				* glm::rotate(timer * spin_speed, glm::vec3{ 0, -1, 0 })
				* glm::translate(local->local_position)
				* glm::mat4(local->local_rotation)
				* glm::scale(local->local_scale);

			Service<RenderSystem>::Get().SubmitModel(model->path, test);
		}
		if (auto pf = instance.scene.get_prototype_if("Unit", "IndicatorRing"))
		{
			auto model = pf->get_if<tc::Model>();
			auto local = pf->get_if<tc::Local>();

			auto test = glm::translate(pos)
				* glm::rotate(timer * ring_speed, glm::vec3{ 0, 1, 0 })
				* glm::translate(local->local_position)
				* glm::mat4(local->local_rotation)
				* glm::scale(local->local_scale);

			Service<RenderSystem>::Get().SubmitModel(model->path, test);
		}

		if (auto pf = instance.scene.get_prototype_if("Unit", "IndicatorDice"))
		{
			auto model = pf->get_if<tc::Model>();
			auto local = pf->get_if<tc::Local>();

			auto test = glm::translate(pos)
				* glm::rotate(timer * dice_speed, glm::vec3{ 0, 1, 0 })
				* glm::translate(local->local_position)
				* glm::mat4(local->local_rotation)
				* glm::scale(local->local_scale);

			Service<RenderSystem>::Get().SubmitModel(model->path, test);
		}

		
		



	}
	
	void CombatModeOverlay::reset_menu()
	{
		menu1.start(-1.f, 0.f, 0.6f, 0.f, [](float x) { return glm::sineEaseOut(x); });
		menu2.start(-1.f, 0.f, 0.6f, 0.f, [](float x) { return glm::sineEaseOut(x); });
	}

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
		(void)world_mouse;
		auto& cam = Service<RenderSystem>::Get().GetCamera();
		//auto& io = ImGui::GetIO();
		// draw stuff on selected heads
		{
			if (instance.ecs.has<tc::Unit>(curr_entity) && instance.ecs.has<tc::Character>(curr_entity) && instance.ecs.has<tc::Transform>(curr_entity))
			{
				auto cs = instance.ecs.get<tc::Character>(curr_entity);
				auto position = instance.ecs.get<tc::Transform>(curr_entity).position;
				position.y += 2;
				auto ss = cam.WorldspaceToScreenspace(position);
				auto vp = cam.GetViewport();
				ss.x = (1 + ss.x) / 2 * vp.z;
				ss.y = vp.w - ((1 + ss.y) / 2 * vp.w);
				auto temp = ImGui::GetCursorPos();

				int p_x = (int)std::floor(position.x);
				int p_y = (int)std::floor(position.z);

				LOG_ASSERT(instance.character_map[p_x][p_y] == curr_entity);

				// Turn on particle
				if (!m_unitTileEmitter.expired() && !instance.ecs.get<tc::Unit>(curr_entity).is_moving() && !stopMoving)
				{
					auto tempUnitEmitter = m_unitTileEmitter.lock();
					auto tempPosition = position;
					tempPosition.y -= 2;
					tempUnitEmitter->UpdateWaypoints(tempPosition);

					stopMoving = true;
					m_unitTileEmitter.lock()->m_GM.m_active = true;

				}

				// Draw whatever thing on their head
				{
					//ImGui::PushFont(FONT_BOLD);
					//auto text_size = ImGui::CalcTextSize(ICON_FA_ICE_CREAM);
					//auto cursor_pos = ImVec2{ ss.x - text_size.x / 2.f, ss.y - text_size.y / 2 };
					//ImGui::SetCursorPos(cursor_pos);
					//ImGui::BeginGroup();
					//ImGui::Text(ICON_FA_ICE_CREAM);


					///*if (ImGui::Button("Attack"))
					//{
					//	state = State::ATTACKING;
					//}
					//if (ImGui::Button("Move"))
					//{
					//}*/
					//ImGui::EndGroup();
					//ImGui::PopFont();
					draw_indicator(instance, position);
				}
				ImGui::SetCursorPos(temp);

				// action
				const ImGuiViewport* viewport = ImGui::GetMainViewport();

				if (battle_state == BATTLE_STATE::CURR_TURN || battle_state == BATTLE_STATE::SELECT_ACTION || battle_state == BATTLE_STATE::SELECT_WEAPON)
				{
					// end turn button
					if (battle_state == BATTLE_STATE::CURR_TURN && UI::UIButton_EndTurn({ viewport->Size.x * 0.9f, viewport->Size.y - action_background_size.y * 1.2f }, { 0,0 }, FONT_PARA))
					{
						// reset previous unit to default
					//	instance.ecs.get_if<tc::Animation>(curr_entity)->stop(curr_entity);
					//	instance.ecs.get<tc::Model>(curr_entity).path = "Models\\UnitBlack_CombatStance.a";
						curr_entity = increase_turn();

						if (auto t = instance.ecs.get_if<tc::Transform>(curr_entity))
						{
							cam_ctrl.move_look_at(cam, t->position);
							nextUnit = true;
						}
					}

					UI::ActionUI(ImVec2{ viewport->Size.x - menu1.get() * 700.f, viewport->Size.y - action_background_size.y }, battle_state == BATTLE_STATE::SELECT_WEAPON ? "SELECT A WEAPON" : "SELECT AN ACTION");
					ImGui::SetCursorPos(ImVec2{ viewport->Size.x - action_background_size.x * 0.85f - menu1.get() * 1000.f , viewport->Size.y - action_background_size.y * 0.7f });

					ImGui::PushStyleColor(ImGuiCol_Border, { 0,0,0,0 });

					if (ImGui::BeginChild("Action content", ImVec2{ action_background_size.x * 0.85f, action_background_size.y * 0.7f }, false, ImGuiWindowFlags_NoScrollbar))
					{
						switch (battle_state)
						{
						case Tempest::CombatModeOverlay::BATTLE_STATE::CURR_TURN:
						{
							// make current unit idle (comment out first cuz can't switch fbx)
						//	if (state != State::CINEMATIC)
						//		instance.ecs.get<tc::Model>(curr_entity).path = "Models\\Unit_Idle.fbx";
							
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0,0,0,0 });
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0,0,0,0 });
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0,0,0,0 });

							// action
							const ImVec2 imgSize{ (float)combat_button_tex[0]->GetWidth(), (float)combat_button_tex[0]->GetHeight() };
							const ImVec2 cursor{ ImGui::GetCursorPos() };
							const ImVec2 avail_region{ ImGui::GetContentRegionAvail() };

							ImGui::SetCursorPos(ImVec2{ cursor.x + avail_region.x * 0.2f - imgSize.x * 0.5f - menu1.get() * 100.f, cursor.y + avail_region.y * 0.45f - imgSize.y * 0.5f });
							if (ImGui::ImageButton((void*)static_cast<size_t>(combat_button_tex[0]->GetID()), imgSize))
							{
								battle_state = BATTLE_STATE::SELECT_ACTION;

								if (instance.tutorial_enable && tutorial_index == 2)
									tutorial_index = 3;
							}
							if (ImGui::IsItemHovered())
								combat_button_tex[0] = tex_map["Assets/CActionSelected.dds"];
							else
								combat_button_tex[0] = tex_map["Assets/CActionUnselected.dds"];

							ImGui::SameLine();
							

							// move
							ImGui::SetCursorPos(ImVec2{ cursor.x + avail_region.x * 0.7f - imgSize.x * 0.5f - menu1.get() * 100.f, cursor.y + avail_region.y * 0.45f - imgSize.y * 0.5f });

							if (ImGui::ImageButton((void*)static_cast<size_t>(combat_button_tex[2]->GetID()), imgSize))
							{
								state = State::MOVING;

								if (instance.tutorial_enable && tutorial_index == 0)
									tutorial_index = 1;
							}

							if (ImGui::IsItemHovered())
								combat_button_tex[2] = tex_map["Assets/CMoveSelected.dds"];
							else
								combat_button_tex[2] = tex_map["Assets/CMoveUnselected.dds"];

							ImGui::PopStyleColor(3);
						}
						break;
						case Tempest::CombatModeOverlay::BATTLE_STATE::SELECT_ACTION: // TODO: account for no actions
						{
							auto& charac = instance.ecs.get<tc::Character>(curr_entity);
							unsigned i = 0;
							float xpos = ImGui::GetCursorPosX() + 60.0f;
							for (auto id : charac.actions)
							{
								if (!instance.ecs.has<tc::ActionGraph>(id) || !(instance.ecs.get<tc::ActionGraph>(id).category & tc::ActionGraph::AC_ATTK))
									continue;

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
							auto& charac = instance.ecs.get<tc::Character>(curr_entity);
							unsigned i = 0;
							float xpos = ImGui::GetCursorPosX() + 60.0f;

							if (charac.weapons.empty())
							{
								selected_weapon = INVALID;
								other_entity = INVALID;
								battle_state = BATTLE_STATE::SELECT_OTHER;
								state = State::ATTACKING;
								calculate_range(instance, p_x, p_y, cs.get_stat(3) + cs.get_statDelta(3), range_map, visited);
							}
							else
							{
								bool all_bad = true;
								for (auto id : charac.weapons)
								{
									if (auto weapon = instance.ecs.get_if<tc::Weapon>(id))
									{
										ImGui::SetCursorPos(ImVec2{ selected_weapon == id ? xpos - action_button_diff : xpos, ImGui::GetCursorPosY() });
										if (UI::UIActionButton(weapon->name.c_str(), "##WEAPONSTUFF" + i++, selected_weapon == id))
										{
											selected_weapon = id;
											other_entity = INVALID;
											battle_state = BATTLE_STATE::SELECT_OTHER;
											state = State::ATTACKING;

											calculate_range(instance, p_x, p_y, cs.get_stat(3) + cs.get_statDelta(3) + weapon->get_stat(3), range_map, visited);
										}
										all_bad = false;
									}
								}
								if(all_bad)
									charac.weapons.clear();
							}

						}
						break;
						default:
							break;
						}

					}
					ImGui::EndChild();

					ImGui::PopStyleColor();
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
		
		// object picking for door
		auto ray = cam.GetMouseRay();
		auto start = cam.GetPosition();
		float dist = 0;
		Entity door = INVALID;
		Entity dead_guy = INVALID;
		if (glm::intersectRayPlane(start, ray, glm::vec3{}, glm::vec3{ 0,1,0 }, dist))
		{
			auto inter = cam.GetPosition() + ray * dist;

			if (instance.character_map.count((int)std::round(inter.x - .5f)) && instance.character_map[(int)std::round(inter.x - .5f)].count((int)std::round(inter.z - .5f)))
			{
				dead_guy = instance.character_map[(int)std::round(inter.x - .5f)][(int)std::round(inter.z - .5f)];
			}

			float r_x = std::round(inter.x) - inter.x;
			float r_y = std::round(inter.z) - inter.z;

			if (abs(r_x) < 0.1f)
			{
				auto c_y = (int)std::round(inter.z - .5f);
				auto a_x = (int)std::round(inter.x);
				auto b_x = (int)std::round(inter.x) - 1;

				if (auto check = instance.door_map[a_x][c_y][b_x][c_y])
				{
					door = instance.door_map[a_x][c_y][b_x][c_y];
				}

			}
			else if (abs(r_y) < 0.1f)
			{
				auto c_x = (int)std::round(inter.x - .5f);
				auto a_y = (int)std::round(inter.z);
				auto b_y = (int)std::round(inter.z) - 1;

				if (auto check = instance.door_map[c_x][a_y][c_x][b_y])
				{
					door = instance.door_map[c_x][a_y][c_x][b_y];
				}
			}

		}

		if (door && instance.ecs.has<tc::Door>(door) && instance.ecs.has<tc::Shape>(door) && instance.ecs.has<tc::Transform>(door))
		{
			auto shape = instance.ecs.get_if<tc::Shape>(door);
			auto& transform = instance.ecs.get<tc::Transform>(door);

			const int& x = shape->x;
			const int& y = shape->y;

			AABB box;

			auto [a_x, a_y, b_x, b_y] = shape_bounding_for_rotation(x, y);

			box.min.x = a_x;
			box.min.z = a_y;

			box.max.x = b_x;
			box.max.z = b_y;

			auto rot = transform.rotation;
			box.min = rot * box.min;
			box.max = rot * box.max;

			box.min.x += transform.position.x;
			box.min.z += transform.position.z;
			box.min.y = 0;

			box.max.x += transform.position.x;
			box.max.z += transform.position.z;
			box.max.y = 0;

			Service<RenderSystem>::Get().DrawLine(box, { 0.1,0.1,0.1,1 });


			if (ImGui::GetIO().MouseClicked[0])
			{
				auto door_p = instance.ecs.get_if<tc::Door>(door);
				auto next = door_p->get_current_state() == tc::Door::State::CLOSE ? tc::Door::State::OPEN : tc::Door::State::CLOSE;
				if (door_p->change_state(next))
				{
					AudioEngine ae;
					ae.Play("Sounds2D/Door_Open.wav", "SFX");
				}
			}
		}
		
		if (dead_guy && instance.ecs.has<tc::Unit>(dead_guy) && instance.ecs.get<tc::Unit>(dead_guy).is_dead() && instance.ecs.has<tc::Shape>(dead_guy) && instance.ecs.has<tc::Transform>(dead_guy))
		{
			auto shape = instance.ecs.get_if<tc::Shape>(dead_guy);
			auto& transform = instance.ecs.get<tc::Transform>(dead_guy);

			const int& x = shape->x;
			const int& y = shape->y;

			AABB box;

			auto [a_x, a_y, b_x, b_y] = shape_bounding_for_rotation(x, y);

			box.min.x = a_x;
			box.min.z = a_y;

			box.max.x = b_x;
			box.max.z = b_y;

			auto rot = transform.rotation;
			box.min = rot * box.min;
			box.max = rot * box.max;

			box.min.x += transform.position.x;
			box.min.z += transform.position.z;
			box.min.y = 0;

			box.max.x += transform.position.x;
			box.max.z += transform.position.z;
			box.max.y = 0;

			Service<RenderSystem>::Get().DrawLine(box, { 0.1,0.1,0.1,1 });


			if (ImGui::GetIO().MouseClicked[0])
			{
				AudioEngine ae;
				ae.Play("Sounds2D/Door_Open.wav", "SFX");


				const ImGuiViewport* viewport = ImGui::GetMainViewport();
				ImGui::OpenPopup("End Unit Cycle");
				ImGui::SetNextWindowPos(ImVec2{ viewport->Size.x * 0.5f, viewport->Size.y * 0.5f }, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
				ImGui::SetNextWindowSize(ImVec2(600, 300));

				other_entity = dead_guy;
			}
		}


		//const ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImVec4 borderCol = { 0.980f, 0.768f, 0.509f, 1.f };
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.f });
		ImGui::PushStyleColor(ImGuiCol_Border, borderCol);
		ImGui::PushStyleColor(ImGuiCol_PopupBg, { 0.06f,0.06f, 0.06f, 0.85f });

		if (ImGui::BeginPopupModal("End Unit Cycle", nullptr, flags))
		{
			auto& ocs = instance.ecs.get<tc::Character>(other_entity);
			auto& ounit = instance.ecs.get<tc::Unit>(other_entity);

			ImVec2 winMin = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y };
			ImVec2 TextMin = { ImGui::GetWindowPos().x + 10.f, ImGui::GetWindowPos().y + 2.5f };
			ImVec2 winMax = { winMin.x + ImGui::GetWindowWidth() * 0.25f, winMin.y + ImGui::GetWindowHeight() * 0.075f };
			ImVec4 col = { 0.980f, 0.768f, 0.509f, 1.f };
			ImVec4 textcol = { 0,0,0,1 };

			auto bgImg = tex_map["Assets/Popup_Backdrop.dds"];
			auto warnImg = tex_map["Assets/YellowWarningIco.dds"];
			string te = "CONFIRMATION";
			ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(bgImg->GetID()), winMin, { winMin.x + ImGui::GetWindowWidth() * 0.8f,winMin.y + ImGui::GetWindowHeight() });
			ImGui::GetWindowDrawList()->AddRectFilled({ winMin.x, winMin.y }, { winMax.x, winMax.y }, ImGui::GetColorU32(col));

			ImGui::PushFont(FONT_OPEN);
			ImGui::GetWindowDrawList()->AddText({ TextMin.x, TextMin.y }
			, ImGui::GetColorU32({ 0,0,0,1 }), te.c_str());
			ImGui::PopFont();


			ImGui::Dummy({ 0.f, ImGui::GetWindowHeight() * 0.2f });
			ImGui::PushFont(FONT_SHEAD);
			auto windowWidth = ImGui::GetWindowSize().x;
			string warningstr = "";
			auto warningSize = ImGui::CalcTextSize(warningstr.c_str()).x;
			ImGui::PushStyleColor(ImGuiCol_Text, { 0.792f,0.22f,0.22f,1.f });
			ImGui::SetCursorPosX((windowWidth - warningSize) * 0.5f - ((float)warnImg->GetWidth() * 0.7f));
			ImGui::Image((void*)static_cast<size_t>(warnImg->GetID()), { (float)warnImg->GetWidth(), (float)warnImg->GetHeight()});
			ImGui::SameLine();
			ImGui::Text(warningstr.c_str());
			ImGui::PopStyleColor();
			ImGui::PopFont();

			ImGui::Dummy({ 0.f, ImGui::GetWindowHeight() * 0.05f });
			ImGui::PushFont(FONT_BODY);
			string str = ocs.name + " has died.";
			string str2 = "What would you like to do?";
			auto strSize = ImGui::CalcTextSize(str.c_str()).x;
				
			ImGui::SetCursorPosX((windowWidth - strSize) * 0.5f);
			ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetColorU32({ 1,0,0,1 }));
			ImGui::Text(ocs.name.c_str());
			ImGui::PopStyleColor();
			ImGui::SameLine();
			ImGui::Text(" has died.");
			ImGui::SetCursorPosX((windowWidth - ImGui::CalcTextSize(str2.c_str()).x) * 0.5f);
			ImGui::Text(str2.c_str());
			ImGui::PopFont();

			ImGui::SetCursorPosX(0);
			ImGui::SetCursorPosY(0);
			if (UI::UIButton_2("Revive", "Revive", { ImGui::GetCursorPosX() + ImGui::GetWindowWidth() * 0.205f, ImGui::GetCursorPosY() + ImGui::GetWindowHeight() * 0.8f }, { -30.f, 0.f }, FONT_PARA))
			{
				units.push_back(other_entity);
				if (auto unit = instance.scene.get_prototype_if("Unit", "Unit"))
				{
					// create a new entity
					/*auto new_e = instance.ecs.create(dead->instance());
					LOG_ASSERT(instance.ecs.has<tc::Transform>(new_e));
					LOG_ASSERT(instance.ecs.has<tc::Character>(new_e));

					instance.ecs.get<tc::Transform>(new_e) = oxform;
					instance.ecs.get<tc::Character>(new_e) = ocs;

					instance.ecs.destroy(other_entity);*/

					LOG_ASSERT(unit->get_if<tc::Model>());
					LOG_ASSERT(unit->get_if<tc::Local>());
					//LOG_ASSERT(instance.ecs.has<tc::Character>(new_e));

					ounit.revive();
					instance.ecs.get<tc::Model>(other_entity) = *unit->get_if<tc::Model>();
					instance.ecs.get<tc::Local>(other_entity) = *unit->get_if<tc::Local>();
				}

				ocs.set_statDelta(0, 0);
				other_entity = INVALID;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SetCursorPosX(0);
			ImGui::SetCursorPosY(0);
			if (UI::UIButton_2("Delete", "Delete", { ImGui::GetCursorPosX() + ImGui::GetWindowWidth() * 0.495f, ImGui::GetCursorPosY() + ImGui::GetWindowHeight() * 0.8f }, { -30.f, 0.f }, FONT_PARA))
			{
				EmitterSystem_3D::GetInstance().CreateSmokePoofEmitter(m_unit_Remove_VFX, instance.ecs.get<tc::Transform>(other_entity).position);
				instance.ecs.destroy(other_entity);
				other_entity = INVALID;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SetCursorPosX(0);
			ImGui::SetCursorPosY(0);
			if (UI::UIButton_2("Ignore", "Ignore", { ImGui::GetCursorPosX() + ImGui::GetWindowWidth() * 0.785f, ImGui::GetCursorPosY() + ImGui::GetWindowHeight() * 0.8f }, { -30.f, 0.f }, FONT_PARA))
			{
				other_entity = INVALID;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
				
		}
		ImGui::PopStyleVar(3);
		ImGui::PopStyleColor(2);

	}

	void CombatModeOverlay::attacking(RuntimeInstance& instance, const glm::ivec2& world_mouse)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		auto& cam = Service<RenderSystem>::Get().GetCamera();
		if (curr_entity && instance.ecs.has<tc::Unit>(curr_entity) && instance.ecs.has<tc::Transform>(curr_entity))
		{
			auto position = instance.ecs.get<tc::Transform>(curr_entity).position;
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
					if (ImGui::BeginChild("Action content", ImVec2{ action_background_size.x * 0.85f, action_background_size.y * 0.7f }, false, ImGuiWindowFlags_NoScrollbar))
					{
						switch (battle_state)
						{
						case Tempest::CombatModeOverlay::BATTLE_STATE::CURR_TURN:
						{
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0,0,0,0 });
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0,0,0,0 });
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0,0,0,0 });

							// action
							const ImVec2 imgSize{ (float)combat_button_tex[0]->GetWidth(), (float)combat_button_tex[0]->GetHeight() };
							ImGui::SetCursorPos(ImVec2{ ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.25f - imgSize.x * 0.5f, ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y * 0.5f - imgSize.y * 0.5f });
							if (ImGui::ImageButton((void*)static_cast<size_t>(combat_button_tex[0]->GetID()), imgSize))
							{
								battle_state = BATTLE_STATE::SELECT_ACTION;

								if (instance.tutorial_enable)
								{
									if (tutorial_index == 1)
										tutorial_index++;
									else if (tutorial_index >= 3)
									{
									//	instance.tutorial_enable = false;
										tutorial_index = 0;
									}
										
								}
							}
							if (ImGui::IsItemHovered())
								combat_button_tex[0] = tex_map["Assets/CActionSelected.dds"];
							else
								combat_button_tex[0] = tex_map["Assets/CActionUnselected.dds"];

							ImGui::PopStyleColor(3);
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
								if (!instance.ecs.has<tc::ActionGraph>(id) ||!(instance.ecs.get<tc::ActionGraph>(id).category & tc::ActionGraph::AC_DEF))
									continue;

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

								auto& atker = instance.ecs.get<tc::Character>(curr_entity);
								auto& defer = instance.ecs.get<tc::Character>(other_entity);

								atker.chosen_weapon = selected_weapon;
								defer.chosen_weapon = other_selected_weapon;

								auto& runtime = dynamic_cast<RuntimeInstance&>(instance);

								auto sequence = runtime.sequences[0];

								/*SimulateConflict s{
									instance.selected,
									other_entity,
									selected_action,
									other_selected_action,
									sequence, freq, win, lose, attack, defend, finish };*/

								Service<EventManager>::Get().instant_dispatch<SimulateConflict>(
									curr_entity,
									other_entity,
									selected_action,
									other_selected_action,
									sequence, freq, win, lose, attack, defend, finish);
							}
							else
							{
								bool all_bad = true;
								for (auto id : charac.weapons)
								{
									if (auto action = instance.ecs.get_if<tc::Weapon>(id))
									{
										ImGui::SetCursorPos(ImVec2{ other_selected_weapon == id ? xpos - action_button_diff : xpos, ImGui::GetCursorPosY() });
										if (UI::UIActionButton(action->name.c_str(), "##WEAPONSTUFF" + i++, other_selected_weapon == id))
										{
											other_selected_weapon = id;
											battle_state = BATTLE_STATE::BATTLE_GLIMPSE;
											state = State::GLIMPSE;

											auto& atker = instance.ecs.get<tc::Character>(curr_entity);
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
												curr_entity,
												other_entity,
												selected_action,
												other_selected_action,
												sequence, freq, win, lose, attack, defend, finish);
										}
										all_bad = false;
									}
								}
								if (all_bad)
									charac.weapons.clear();
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
				// draw character turn data here
				auto& charac_icon = tex_map["Assets/CharacterIcon.dds"];
				const ImVec2 child_size{ charac_icon->GetWidth() * units.size() * 1.1f + 20.0f * units.size() - 1, charac_icon->GetHeight() * 2.f };
				ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.5f - child_size.x * 0.5f, viewport->Size.y * 0.2f - child_size.y * 0.5f});
				if (ImGui::BeginChild("Select other entity attack", child_size, true))
				{
					unsigned i = 0;
					for ([[maybe_unused]]auto id : units)
					{
						auto &charac = instance.ecs.get<tc::Character>(id);
						if (curr_turn != i++)
						{
							if (!i)
							{
								ImGui::Dummy(ImVec2{ 20.0f, charac_icon->GetHeight() * 0.9f });
							//	ImGui::SameLine();
							}
							
							UI::UICharButton_NoDelete((void*)static_cast<size_t>(charac_icon->GetID()), ImVec2{ charac_icon->GetWidth() * 1.0f, charac_icon->GetHeight() * 1.0f }, "", "noID", false,
								ImVec2{ 0,0 }, ImVec2{ 1,1 }, 2, ImVec4{ 0,0,0,0 }, ImVec4{ charac.color.x,charac.color.y, charac.color.z, 1});
						}
							
						else
							UI::UICharButton_Arrow((void*)static_cast<size_t>(charac_icon->GetID()), ImVec2{ charac_icon->GetWidth() * 1.0f, charac_icon->GetHeight() * 1.0f }, "", "noID", true,
								ImVec2{ 0,0 }, ImVec2{ 1,1 }, 2, ImVec4{ 0,0,0,0 }, ImVec4{ charac.color.x,charac.color.y, charac.color.z, 1 });

						ImGui::SameLine();
					}
				}

				ImGui::EndChild();
				
				// outlines all units in the map
				/*for (auto& [x, m] : instance.character_map)
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
					}*/

				// if u want display like the triangle on top
				// need either imgui::image or drawlist


				// helpers
				//draw_range(range_map);
				auto drawbox = [&](int x, int y, ImU32 color)
				{

					auto WorldSpaceAABBtoSSVecOfPts = [](Camera& cam, AABB aabb)
					{
						tvector<vec3> ws_pts{ aabb.min, vec3{aabb.min.x, 0.f, aabb.max.z}, aabb.max, vec3{aabb.max.x, 0.f, aabb.min.z} };
						tvector<ImVec2> ss_pts;
						auto vp = cam.GetViewport();
						for (auto pt : ws_pts)
						{
							auto t_ss = cam.WorldspaceToScreenspace(pt);
							t_ss.x = (1 + t_ss.x) / 2 * vp.z;
							t_ss.y = vp.w - ((1 + t_ss.y) / 2 * vp.w);

							ss_pts.push_back(ImVec2{ t_ss.x, t_ss.y });
						}

						return ss_pts;
					};

					AABB box;
					box.min.x = (float)x + .1f;
					box.min.z = (float)y + .1f;
					box.min.y = 0;

					box.max.x = x + .9f;
					box.max.z = y + .9f;
					box.max.y = 0;

					auto pts = WorldSpaceAABBtoSSVecOfPts(cam, box);

					ImGui::GetBackgroundDrawList()->AddConvexPolyFilled(pts.data(), 4, color);

				};

				for (auto& [x, m] : range_map)
					for (auto [y, i] : m)
					{
						if (p_x == x && p_y == y)
							continue;

						if (i == 0) // hit nothing dark green
							drawbox(x, y, IM_COL32(0xFF, 0x20, 0x20, 0x60));
						else if (i == 1) // hit collider dark red
						{
							drawbox(x, y, IM_COL32(0x80, 0x80, 0x80, 0x60));
						}
						else if (i == 2) // hit cover only dark red
							drawbox(x, y, IM_COL32(0xFF, 0xAF, 0x20, 0x60));
						else if (i == 3) // hit player
							drawbox(x, y, IM_COL32(0xFF, 0x20, 0x20, 0x60));
						else // hit player behind cover
							drawbox(x, y, IM_COL32(0xFF, 0xAF, 0x20, 0x60));

					}



				if (visited[w_x][w_y])
				{
					if (p_x == w_x && p_y == w_y)
					{
						Line l;
						l.p0 = glm::vec3(w_x + .5f - .5f, 0, w_y + .5f - .5f);
						l.p1 = glm::vec3(w_x + .5f + .5f, 0, w_y + .5f + .5f);

						Line r;
						r.p0 = glm::vec3(w_x + .5f - .5f, 0, w_y + .5f + .5f);
						r.p1 = glm::vec3(w_x + .5f + .5f, 0, w_y + .5f - .5f);
						Service<RenderSystem>::Get().DrawLine(l, { 1,0,0,1 });
						Service<RenderSystem>::Get().DrawLine(r, { 1,0,0,1 });

						if (ImGui::GetIO().MouseClicked[0])
						{
							// cancel
							state = State::MENU;
							battle_state = BATTLE_STATE::CURR_TURN;
						}
					}
					else if(instance.character_map[w_x][w_y])
					{
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

						auto& transform = instance.ecs.get<tc::Transform>(curr_entity);

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

						auto can_hit = range_map[w_x][w_y] >= 3;
						if (ImGui::GetIO().MouseClicked[0] && can_hit)
						{
							// Attack
							auto attacker = curr_entity;


						//	if (other_entity != UNDEFINED)
						//		instance.ecs.get_if<tc::Animation>(other_entity)->stop(other_entity);

							other_entity = instance.character_map[w_x][w_y];

						//	auto anim = instance.ecs.get_if<tc::Animation>(other_entity);
						//	if (anim->if_end(other_entity))
						//	{
						//		anim->change_animation(other_entity, "../../../Resource/Animations/Unit_Idle.fbx");
						//		anim->play(other_entity, true);
						//	}

							// PSEUDO (this does not work ? when it needs to be changed to block)
						//	instance.ecs.get<tc::Model>(other_entity).path = "Models\\Unit_Idle.fbx";
							battle_state = BATTLE_STATE::SELECT_ACTION;

							LOG_ASSERT(instance.ecs.has<tc::Character>(attacker));
							LOG_ASSERT(instance.ecs.has<tc::Character>(other_entity));
							LOG_ASSERT(instance.ecs.has<tc::Transform>(other_entity));

							// for now we just cancel
							//state = State::MENU;

							// rotate the other fella
							auto& otransform = instance.ecs.get<tc::Transform>(other_entity);
							otransform.rotation = transform.rotation * glm::angleAxis(glm::radians(180.f), vec3{ 0,1,0 });
						}
					}
					else
					{
						/*Line l;
						l.p0 = glm::vec3(w_x + .5f - .1f, 0, w_y + .5f - .1f);
						l.p1 = glm::vec3(w_x + .5f + .1f, 0, w_y + .5f + .1f);

						Line r;
						r.p0 = glm::vec3(w_x + .5f - .1f, 0, w_y + .5f + .1f);
						r.p1 = glm::vec3(w_x + .5f + .1f, 0, w_y + .5f - .1f);
						Service<RenderSystem>::Get().DrawLine(l, { 0,1,0,1 });
						Service<RenderSystem>::Get().DrawLine(r, { 0,1,0,1 });*/


					}


				}

				//  segmented line of sight
				{
				/*
				if (visited[w_x][w_y])
				{

					auto [v, b] = collide_first_edited(p_x, p_y, w_x, w_y, instance.collision_map, instance.wall_map, instance.character_map);


					vec3 start_p = vec3(p_x + .5f, 0, p_y + .5f);
					vec3 end_p = vec3(w_x + .5f, 0, w_y + .5f);
					vec3 line_v = vec3(w_x, 0, w_y) - vec3(p_x, 0, p_y);
					vec4 color = { 0,1,0,1 };
					vec4 orange = { 1,0.3725,0.121,1 };
					vec3 curr = start_p;
					bool check_character = false;
					for (auto i = 0; i < (int)v.size(); ++i)
					{
						const auto& [x0, y0, x1, y1] = v[i];

						// collision points
						auto p0 = vec3(x0 + .5f, 0, y0 + .5f);
						auto p1 = vec3(x1 + .5f, 0, y1 + .5f);

						// normal of plane
						auto normal = glm::normalize(p1 - p0);

						// intersection point
						auto ex = glm::mix(p0, p1, .5f);


						// 
						float dist = 0.f;
						//bool check_character = w_x == x1 && w_y == y1 && instance.character_map[x1][y1] && !instance.wall_map[x0][y0][x1][y1];
						
						if (glm::intersectRayPlane(start_p, glm::normalize(line_v), ex, normal, dist))
						{
							vec3 intersection = start_p + glm::normalize(line_v) * dist;

							// uncomment for multiple line segments
							Line pointer;
							pointer.p0 = curr;
							pointer.p1 = intersection;
							Service<RenderSystem>::Get().DrawLine(pointer, color);

							curr = intersection;

							color = orange;
							check_character = w_x == x1 && w_y == y1 && instance.character_map[x1][y1] && !instance.wall_map[x0][y0][x1][y1];
						}

					}
					
					// draw from current to end
					// if hit something last colour is red

					if (b && !check_character)
					{
						color = { 1,0,0,1 };

						Line l;
						l.p0 = glm::vec3(curr.x - .1f, 0, curr.z - .1f);
						l.p1 = glm::vec3(curr.x + .1f, 0, curr.z + .1f);

						Line r;
						r.p0 = glm::vec3(curr.x - .1f, 0, curr.z + .1f);
						r.p1 = glm::vec3(curr.x + .1f, 0, curr.z - .1f);

						Service<RenderSystem>::Get().DrawLine(l, color);
						Service<RenderSystem>::Get().DrawLine(r, color);
					}

					Line pointer;
					pointer.p0 = curr;
					pointer.p1 = end_p;
					Service<RenderSystem>::Get().DrawLine(pointer, color);
				}
				*/
				}

				// lousy line of sight
				if (range_map.count(w_x) && range_map[w_x].count(w_y))
				{
					const float thickness = .15f;
					auto i = range_map[w_x][w_y];
					vec3 start_p = vec3(p_x + .5f, 0, p_y + .5f);
					vec3 end_p = vec3(w_x + .5f, 0, w_y + .5f);
					vec3 line_v = vec3(w_x, 0, w_y) - vec3(p_x, 0, p_y);
					ImU32 color;

					if (i == 0) // hit nothing not blocked
						color = IM_COL32(0xFF, 0x20, 0x20, 0x80);
					else if (i == 1) // hit collider
						color = IM_COL32(0x80, 0x80, 0x80, 0x80);
					else if (i == 2) // hit cover
						color = IM_COL32(0xFF, 0xAF, 0x20, 0x80);
					else if (i == 3) // hit collider and collider is player
						color = IM_COL32(0xFF, 0x20, 0x20, 0xFF);
					else // hit player and its behind cover
						color = IM_COL32(0xFF, 0xAF, 0x20, 0xFF);

					auto WorldSpaceLinetoSSVecOfPts = [](Camera& cam, vec3 start, vec3 end, float thickness)
					{
						auto v = glm::normalize(end - start);
						auto n = vec3(v.z, 0.f, -v.x);
						n *= thickness;

						tvector<vec3> ws_pts{ 
							start + n,
							end + n,
							end - n,
							start - n, };

						tvector<ImVec2> ss_pts;
						auto vp = cam.GetViewport();
						for (auto pt : ws_pts)
						{
							auto t_ss = cam.WorldspaceToScreenspace(pt);
							t_ss.x = (1 + t_ss.x) / 2 * vp.z;
							t_ss.y = vp.w - ((1 + t_ss.y) / 2 * vp.w);

							ss_pts.push_back(ImVec2{ t_ss.x, t_ss.y });
						}

						return ss_pts;
					};

					auto pts = WorldSpaceLinetoSSVecOfPts(cam, start_p, end_p - glm::normalize(line_v) * thickness, thickness);

					auto drawlist = ImGui::GetBackgroundDrawList();
					drawlist->AddConvexPolyFilled(pts.data(), 4, color);


					auto WorldSpaceToScreenspace = [](Camera& cam, vec3 pt)
					{
						auto vp = cam.GetViewport();
						auto t_ss = cam.WorldspaceToScreenspace(pt);
						t_ss.x = (1 + t_ss.x) / 2 * vp.z;
						t_ss.y = vp.w - ((1 + t_ss.y) / 2 * vp.w);

						return ImVec2{ t_ss.x, t_ss.y };
					};

					auto v = glm::normalize(line_v);
					auto n = vec3(v.z, 0.f, -v.x);
					n *= thickness * 1.5f;

					auto tip = WorldSpaceToScreenspace(cam, end_p);

					vec3 pt1, pt2;
					auto a = end_p - glm::normalize(line_v) * thickness;
					pt1.x = a.x + n.x;
					pt1.z = a.z + n.z;
					pt1.y = 0;

					pt2.x = a.x - n.x;
					pt2.z = a.z - n.z;
					pt2.y = 0;

					auto v1 = WorldSpaceToScreenspace(cam, pt1);
					auto v2 = WorldSpaceToScreenspace(cam, pt2);

					drawlist->AddTriangleFilled(tip, v1, v2, color);


					/*Line pointer;
					pointer.p0 = start_p;
					pointer.p1 = end_p;
					Service<RenderSystem>::Get().DrawLine(pointer, color);*/


				}
			}

		}
		else
			state = State::MENU;
	}
	
	void CombatModeOverlay::moving(RuntimeInstance& instance, const glm::ivec2& world_mouse)
	{
		auto& cam = Service<RenderSystem>::Get().GetCamera();
		if (curr_entity && instance.ecs.has<tc::Unit>(curr_entity) && instance.ecs.has<tc::Character>(curr_entity) && instance.ecs.has<tc::Transform>(curr_entity))
		{
			auto& cs = instance.ecs.get<tc::Character>(curr_entity);
			auto& unit = instance.ecs.get<tc::Unit>(curr_entity);
			auto position = instance.ecs.get<tc::Transform>(curr_entity).position;
			position.y += 2;
			auto ss = cam.WorldspaceToScreenspace(position);
			auto vp = cam.GetViewport();
			ss.x = (1 + ss.x) / 2 * vp.z;
			ss.y = vp.w - ((1 + ss.y) / 2 * vp.w);
			auto temp = ImGui::GetCursorPos();

			int p_x = (int)std::floor(position.x);
			int p_y = (int)std::floor(position.z);

			LOG_ASSERT(instance.character_map[p_x][p_y] == curr_entity);

			int steps = cs.get_stat(4) + cs.get_statDelta(4);
			tmap<int, tmap<int, uint32_t>> distance;
			calculate_move(instance, p_x, p_y, steps, distance, visited);

			int w_x = world_mouse.x;
			int w_y = world_mouse.y;

			for (auto& [x, m] : visited)
				for (auto [y, b] : m)
				{
					if (!b) continue;
					AABB box;

					box.min.x = (float)x + .1f;
					box.min.z = (float)y + .1f;
					box.min.y = 0;

					box.max.x = x + .9f;
					box.max.z = y + .9f;
					box.max.y = 0;

					//Service<RenderSystem>::Get().DrawLine(box, { 0,1,0,1 });


					auto WorldSpaceAABBtoSSVecOfPts = [](Camera& cam, AABB aabb)
					{
						tvector<vec3> ws_pts{ aabb.min, vec3{aabb.min.x, 0.f, aabb.max.z}, aabb.max, vec3{aabb.max.x, 0.f, aabb.min.z} };
						tvector<ImVec2> ss_pts;
						auto vp = cam.GetViewport();
						for (auto pt : ws_pts)
						{
							auto t_ss = cam.WorldspaceToScreenspace(pt);
							t_ss.x = (1 + t_ss.x) / 2 * vp.z;
							t_ss.y = vp.w - ((1 + t_ss.y) / 2 * vp.w);

							ss_pts.push_back(ImVec2{ t_ss.x, t_ss.y });
						}

						return ss_pts;
					};


					auto pts = WorldSpaceAABBtoSSVecOfPts(cam, box);

					auto drawlist = ImGui::GetBackgroundDrawList();
					if (p_x == x && p_y == y)
						;// drawlist->AddConvexPolyFilled(pts.data(), 4, IM_COL32(0xAF, 0x20, 0x20, 0x80));
					else
						drawlist->AddConvexPolyFilled(pts.data(), 4, IM_COL32(0x20, 0xAF, 0x20, 0x60));

				}

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

						// Turn off particle
						if (!m_unitTileEmitter.expired())
						{
							m_unitTileEmitter.lock()->m_GM.m_active = false;
							m_unitTileEmitter.lock()->ClearAllParticles();
						}
					}
				}
				else
				{
					/*Line l;
					l.p0 = glm::vec3(w_x + .5f - .1f, 0, w_y + .5f - .1f);
					l.p1 = glm::vec3(w_x + .5f + .1f, 0, w_y + .5f + .1f);

					Line r;
					r.p0 = glm::vec3(w_x + .5f - .1f, 0, w_y + .5f + .1f);
					r.p1 = glm::vec3(w_x + .5f + .1f, 0, w_y + .5f - .1f);
					Service<RenderSystem>::Get().DrawLine(l, { 0,1,0,1 });
					Service<RenderSystem>::Get().DrawLine(r, { 0,1,0,1 });*/


					auto& transform = instance.ecs.get<tc::Transform>(curr_entity);

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


					auto v = algo::bfs(p_x, p_y, w_x, w_y, steps,
						[&collision_map = instance.collision_map, &wall_map = instance.wall_map](int x, int y, int n_x, int n_y) {

						if (wall_map[x][y][n_x][n_y])
							return true;
						if (collision_map[n_x][n_y])
							return true;
						return false;
					});


					auto WorldSpaceAABBtoSSVecOfPts = [](Camera& cam, AABB aabb)
					{
						tvector<vec3> ws_pts{ aabb.min, vec3{aabb.min.x, 0.f, aabb.max.z}, aabb.max, vec3{aabb.max.x, 0.f, aabb.min.z} };
						tvector<ImVec2> ss_pts;
						auto vp = cam.GetViewport();
						for (auto pt : ws_pts)
						{
							auto t_ss = cam.WorldspaceToScreenspace(pt);
							t_ss.x = (1 + t_ss.x) / 2 * vp.z;
							t_ss.y = vp.w - ((1 + t_ss.y) / 2 * vp.w);

							ss_pts.push_back(ImVec2{ t_ss.x, t_ss.y });
						}

						return ss_pts;
					};

					auto WorldSpaceToScreenspace = [](Camera& cam, vec3 pt)
					{
						auto vp = cam.GetViewport();
						auto t_ss = cam.WorldspaceToScreenspace(pt);
						t_ss.x = (1 + t_ss.x) / 2 * vp.z;
						t_ss.y = vp.w - ((1 + t_ss.y) / 2 * vp.w);

						return ImVec2{ t_ss.x, t_ss.y };
					};


					const auto arrow_color = IM_COL32(0x20, 0xFF, 0x20, 0xFF);
					const auto thickness = .15f;
					auto drawlist = ImGui::GetBackgroundDrawList();
					for (int i = 1; i < v.size(); ++i)
					{
						auto dir = v[i] - v[i-1];
						AABB box;
						if (dir.x)
						{
							box.min.x = v[i - 1].x + .5f + thickness * dir.x;
							box.min.z = v[i - 1].y + .5f + thickness;
							box.min.y = 0;

							box.max.x = v[i].x + .5f - thickness * dir.x;
							box.max.z = v[i].y + .5f - thickness;
							box.max.y = 0;
						}
						else
						{
							box.min.x = v[i - 1].x + .5f + thickness;
							box.min.z = v[i - 1].y + .5f + thickness * dir.y;
							box.min.y = 0;

							box.max.x = v[i].x + .5f - thickness;
							box.max.z = v[i].y + .5f -thickness * dir.y;
							box.max.y = 0;
						}

						auto pts = WorldSpaceAABBtoSSVecOfPts(cam, box);
						drawlist->AddConvexPolyFilled(pts.data(), 4, arrow_color);
					}

					for (int i = 1; i < v.size()-1; ++i)
					{
						AABB box;
						box.min.x = v[i].x + .5f + thickness;
						box.min.z = v[i].y + .5f + thickness;
						box.min.y = 0;

						box.max.x = v[i].x + .5f - thickness;
						box.max.z = v[i].y + .5f - thickness;
						box.max.y = 0;

						auto pts = WorldSpaceAABBtoSSVecOfPts(cam, box);
						drawlist->AddConvexPolyFilled(pts.data(), 4, arrow_color);
					}

					auto dir = v[1] - v[0];
					auto tip = WorldSpaceToScreenspace(cam, vec3{ v.front().x + 0.5f, 0.f, v.front().y + 0.5f });

					vec3 pt1, pt2;

					pt1.x = v.front().x + .5f + thickness * dir.x;
					pt1.z = v.front().y + .5f + thickness * dir.y;
					pt1.y = 0;

					pt2.x = v.front().x + .5f + thickness * dir.x;
					pt2.z = v.front().y + .5f + thickness * dir.y;
					pt2.y = 0;

					if (dir.x)
					{
						pt1.z += thickness * 1.5f;
						pt2.z -= thickness * 1.5f;
					}
					else
					{
						pt1.x += thickness * 1.5f;
						pt2.x -= thickness * 1.5f;
					}

					auto v1 = WorldSpaceToScreenspace(cam, pt1);
					auto v2 = WorldSpaceToScreenspace(cam, pt2);

					drawlist->AddTriangleFilled(tip, v1, v2, arrow_color);


					/*auto WorldSpaceVecToSSVecOfPts = [](Camera& cam, tvector<vec3> ws_pts)
					{
						tvector<ImVec2> ss_pts;
						auto vp = cam.GetViewport();
						for (auto pt : ws_pts)
						{
							auto t_ss = cam.WorldspaceToScreenspace(pt);
							t_ss.x = (1 + t_ss.x) / 2 * vp.z;
							t_ss.y = vp.w - ((1 + t_ss.y) / 2 * vp.w);

							ss_pts.push_back(ImVec2{ t_ss.x, t_ss.y });
						}

						return ss_pts;
					};*/

					//const auto thickness = .25f;
					//tvector<vec3> ws_pts;
					//auto prev = v[1] - v[0];
					//for (int i = 1; i < v.size()-1; ++i)
					//{
					//	auto next = v[i + 1] - v[i];
					//	if (prev.x != next.x || prev.y != next.y)
					//	{
					//		auto a = glm::vec2(prev) * thickness;
					//		auto b = glm::vec2(next) * -thickness;
					//		auto c = a + b;

					//		vec3 d{ v[i].x + c.x + 0.5f, 0.f, v[i].y + c.y + 0.5f };
					//		// add the points first
					//		ws_pts.push_back(d);
					//	}
					//	prev = next;
					//}

					//{
					//	if (prev.x)
					//	{
					//		//ws_pts.push_back(vec3{ v[0].x + 0.5f, 0, v[0].y + 0.5f + thickness });
					//		//ws_pts.push_back(vec3{ v[0].x + 0.5f, 0, v[0].y + 0.5f - thickness });
					//	}
					//	else
					//	{
					//		//ws_pts.push_back(vec3{ v[0].x + 0.5f + thickness, 0, v[0].y + 0.5f });
					//		//ws_pts.push_back(vec3{ v[0].x + 0.5f - thickness, 0, v[0].y + 0.5f });
					//	}
					//	ws_pts.push_back(vec3{ v[0].x + 0.5f, 0, v[0].y + 0.5f });
					//	prev = -prev;
					//}

					//for (int i = v.size() - 2; i >= 1 ; --i)
					//{
					//	auto next = v[i - 1] - v[i];
					//	if (prev.x != next.x || prev.y != next.y)
					//	{
					//		auto a = glm::vec2(prev) * -thickness;
					//		auto b = glm::vec2(next) * thickness;
					//		auto c = a + b;

					//		vec3 d{ v[i].x + c.x + 0.5f, 0.f, v[i].y + c.y + 0.5f };
					//		// add the points first
					//		ws_pts.push_back(d);
					//	}
					//	prev = next;
					//}

					//if (prev.x)
					//{
					//	ws_pts.push_back(vec3{ v[0].x + 0.5f, 0, v[0].y + 0.5f + thickness });
					//	ws_pts.push_back(vec3{ v[0].x + 0.5f, 0, v[0].y + 0.5f - thickness });
					//}
					//else
					//{
					//	ws_pts.push_back(vec3{ v[0].x + 0.5f + thickness, 0, v[0].y + 0.5f });
					//	ws_pts.push_back(vec3{ v[0].x + 0.5f - thickness, 0, v[0].y + 0.5f });
					//}
					////ws_pts.push_back(vec3{ v.back().x + 0.5f - thickness, 0, v.back().y + 0.5f });

					//// cap the end


					//auto drawlist = ImGui::GetBackgroundDrawList();
					//auto ss_pts = WorldSpaceVecToSSVecOfPts(cam, ws_pts);
					//drawlist->AddConvexPolyFilled(ss_pts.data(), ss_pts.size(), IM_COL32(0xAF, 0x20, 0x20, 0xA0));


					// Move
					if (io.MouseClicked[0])
					{
						/*auto v = algo::bfs(p_x, p_y, w_x, w_y, steps,
							[&collision_map = instance.collision_map, &wall_map = instance.wall_map](int x, int y, int n_x, int n_y) {

								if (wall_map[x][y][n_x][n_y])
									return true;
								if (collision_map[n_x][n_y])
									return true;
								return false;
							});*/

						// move
						v.pop_back();
						unit.set_path(v, transform);

						/*transform.position.x = w_x + .5f;
						transform.position.z = w_y + .5f;*/
						instance.selected = INVALID;
						state = State::MENU;

						// Turn off particle
						if (!m_unitTileEmitter.expired())
						{
							m_unitTileEmitter.lock()->m_GM.m_active = false;
							m_unitTileEmitter.lock()->ClearAllParticles();
							stopMoving = false;
						}
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

		if (finish && UI::UIButton_2("Confirm", "Confirm", ImVec2{ viewport->Size.x * 0.45f, viewport->Size.y * 0.8f }, { -60,0 }, FONT_BODY))
		{
			if (instance.tutorial_enable && tutorial_index == 3)
			{
				tutorial_index = 4;
				//instance.tutorial_enable = false;
			}
				
			state = State::FIGHT;
			display_other_stat = false;
			display_curr_stat = false;

			auto sequence = instance.ecs.view_first<tc::ConflictGraph>();

			try_build_all(instance);

			// create temp entities so we can check whats the damage, if any
			auto temp_cc = instance.ecs.get<tc::Character>(curr_entity);
			auto temp_oc = instance.ecs.get<tc::Character>(other_entity);

			instance.srm.instant_dispatch_to_id<Simulate>(sequence, curr_entity, other_entity, selected_action, other_selected_action);
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

			auto cs = instance.ecs.get<tc::Character>(other_entity);

			damage = temp_oc.get_statDelta(0) - cs.get_statDelta(0);

			// get the results
			auto curr_entity_result = units_results.find(curr_entity);
			curr_entity_result->second.dmg_done += damage;
			curr_entity_result->second.total_attacks += 1;
			if (damage > 0)
				curr_entity_result->second.successful_attacks += 1;

			units_results.find(other_entity)->second.dmg_taken += damage;
			if (temp_oc.get_stat(0) + temp_oc.get_statDelta(0) <= 0)
			{
				// for dead
				curr_entity_result->second.defeated += 1;
				units_results.find(other_entity)->second.deaths += 1;
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

		if (finish && UI::UIButton_2("Cancel", "Cancel", ImVec2{ viewport->Size.x * 0.55f, viewport->Size.y * 0.8f }, { -60,0 }, FONT_BODY))
		{
			//battle_state = BATTLE_STATE::SELECT_OTHER;
			display_curr_stat = false;
			display_other_stat = false;
		//	if (other_entity != INVALID)
		//		instance.ecs.get_if<tc::Animation>(other_entity)->stop(other_entity);
				//instance.ecs.get<tc::Model>(other_entity).path = "Models\\UnitBlack_CombatStance.a";
			instance.selected = INVALID;
			state = State::MENU;
			battle_state = BATTLE_STATE::CURR_TURN; // temp testing
		}
	}

	void CombatModeOverlay::fight(RuntimeInstance& instance)
	{
		static bool display_other_end_cycle = false;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		// check if the other_entity has health (only other for now)
		//auto& charac = instance.ecs.get<tc::Character>(other_entity);

		if (UI::CharacterTurnData(instance, curr_entity, { 0.f, viewport->Size.y - placeholder_height }, false, !atk_rolled))
		{
			// attacker roll
			atk_rolled = true;
			inter1.start(0, 1);
		}

		if (UI::CharacterTurnData(instance, other_entity, { viewport->Size.x, viewport->Size.y - placeholder_height }, true, !def_rolled))
		{
			// defender roll
			def_rolled = true;
			inter2.start(0, 1);
		}


		// draw the combat roll
		auto tex = tex_map["Assets/CombatRollBG.dds"];
		ImVec2 point{ 0.0f,viewport->Size.y * 0.2f };
		{
			ImVec2 Min{ point.x, point.y };
			ImVec2 Max{ Min.x + viewport->Size.x , Min.y + viewport->Size.y * 0.3f };
			ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(tex->GetID()), Min, Max);
		}

		auto& attacker = instance.ecs.get<tc::Character>(curr_entity);
		auto& defender = instance.ecs.get<tc::Character>(other_entity);

		// display the character name and rolls
		FONT_HEAD->Scale = 1.0f;
		

		std::string roll = atk_rolled ? std::to_string(atk_output) : "";
		if (inter1.is_in_progress())
		{
			roll = std::to_string(els::random::uniform_rand(0, 999));

			// Ready to show bam VFX
			b_combatRoll_VFX_Ready = true;
			b_playerOne_Rolled = true;
		}

		ImGui::PushFont(FONT_HEAD);
		ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.37f - ImGui::CalcTextSize(attacker.name.c_str()).x * 0.5f, viewport->Size.y * 0.27f });
		ImGui::Text(attacker.name.c_str());
		ImGui::PopFont();

		if (inter1.is_finished())
			FONT_HEAD->Scale = 2.0f;

		ImGui::PushFont(FONT_HEAD);
		ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.37f - ImGui::CalcTextSize(roll.c_str()).x * 0.5f, viewport->Size.y * 0.35f });

		if (inter1.is_finished())
		{
			if (win)
				ImGui::PushStyleColor(ImGuiCol_Text, { 0,1,0,1 });
			else
				ImGui::PushStyleColor(ImGuiCol_Text, { 1,0,0,1 });

			// Defender / Right hand side combat roll
			if (b_combatRoll_VFX_Ready)
			{
				b_combatRoll_VFX_Ready = false;
				ImVec2 spawnPos = ImVec2{ viewport->Size.x * 0.635f - ImGui::CalcTextSize(roll.c_str()).x * 0.1f, viewport->Size.y * 0.38f };
				EmitterSystem_2D::GetInstance().CreateExplosionEmitter(m_combatRoll_VFX, spawnPos);
			}

			if (atk_rolled)
			{
				if (start_inter_atk_roll)
				{
					atk_roll_inter[0].start(3.0f, 2.0f, .5f, 0.f,
						[](float x) { return glm::elasticEaseOut(x);
					});
					atk_roll_inter[1].start(3.0f, 2.0f, .5f, 0.2f,
						[](float x) { return glm::elasticEaseOut(x);
					});
					atk_roll_inter[2].start(3.0f, 2.0f, .5f, 0.4f,
						[](float x) { return glm::elasticEaseOut(x);
					});
					start_inter_atk_roll = false;
				}

				for (int i = 0; i < roll.size(); i++)
				{
					FONT_HEAD->Scale = atk_roll_inter[i].get();
					ImGui::PushFont(FONT_HEAD);
					ImGui::Text(std::string{ roll[i] }.c_str());
					ImGui::PopFont();
					ImGui::SameLine();
				}
			}
		}

		else
			ImGui::Text(roll.c_str());

		ImGui::PopFont();

		if (inter1.is_finished())
			ImGui::PopStyleColor();

		FONT_HEAD->Scale = 1.0f;
		string roll2 = def_rolled ? std::to_string(def_output) : "";
		if (inter2.is_in_progress())
		{
			roll2 = std::to_string(els::random::uniform_rand(0, 999));

			// Ready to show bam VFX
			b_combatRoll_VFX_Ready = true;
			b_playerTwo_Rolled = true;
		}

		ImGui::PushFont(FONT_HEAD);
		ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.63f - ImGui::CalcTextSize(defender.name.c_str()).x * 0.5f, viewport->Size.y * 0.27f });
		ImGui::Text(defender.name.c_str());
		ImGui::PopFont();

		if (inter2.is_finished())
			FONT_HEAD->Scale = 2.0f;
		ImGui::PushFont(FONT_HEAD);
		ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.63f - ImGui::CalcTextSize(roll2.c_str()).x * 0.5f, viewport->Size.y * 0.35f });
		
		if (inter2.is_finished())
		{
			if (!win)
				ImGui::PushStyleColor(ImGuiCol_Text, { 0,1,0,1 });
			else
				ImGui::PushStyleColor(ImGuiCol_Text, { 1,0,0,1 });

			// Attacker / Left hand side combat roll
			if (b_combatRoll_VFX_Ready)
			{
				b_combatRoll_VFX_Ready = false;
				ImVec2 spawnPos = ImVec2{ viewport->Size.x * 0.375f - ImGui::CalcTextSize(roll.c_str()).x * 0.1f, viewport->Size.y * 0.38f };
				EmitterSystem_2D::GetInstance().CreateExplosionEmitter(m_combatRoll_VFX, spawnPos);
			}

			if (def_rolled)
			{
				if (start_inter_def_roll)
				{
					def_roll_inter[0].start(3.0f, 2.0f, .5f, 0.f,
						[](float x) { return glm::elasticEaseOut(x);
					});
					def_roll_inter[1].start(3.0f, 2.0f, .5f, 0.2f,
						[](float x) { return glm::elasticEaseOut(x);
					});
					def_roll_inter[2].start(3.0f, 2.0f, .5f, 0.4f,
						[](float x) { return glm::elasticEaseOut(x);
					});

					start_inter_def_roll = false;
				}

				for (int i = 0; i < roll2.size(); i++)
				{
					FONT_HEAD->Scale = def_roll_inter[i].get();
					ImGui::PushFont(FONT_HEAD);
					ImGui::Text(std::string{ roll2[i] }.c_str());
					ImGui::PopFont();
					ImGui::SameLine();
				}
			}
		}

		else
			ImGui::Text(roll2.c_str());
		ImGui::PopFont();

		if (inter2.is_finished())
			ImGui::PopStyleColor();

		// reset the scale
		FONT_HEAD->Scale = 1.0f;
		
		// VFX for combat roll done
		if (inter1.is_finished() && inter2.is_finished())
		{
			if (b_playerOne_Rolled && b_playerTwo_Rolled)
			{
				ImVec2 startPos = ImVec2{ 0.f, 0.f };
				ImVec2 endPos = ImVec2{ 0.f, 0.f };

				if (win)
					startPos = ImVec2{ viewport->Size.x * 0.36f - ImGui::CalcTextSize(roll.c_str()).x * 0.45f, viewport->Size.y * 0.365f };
				else
					startPos = ImVec2{ viewport->Size.x * 0.62f - ImGui::CalcTextSize(roll.c_str()).x * 0.1f, viewport->Size.y * 0.363f };

				endPos = ImVec2{ startPos.x + 50.0f, startPos.y };
				EmitterSystem_2D::GetInstance().CreateLineEmitter(m_winningNumber_VFX, startPos, endPos);

				b_playerOne_Rolled = false;
				b_playerTwo_Rolled = false;
			}
		}

		// only trigger this if no more rolls
		if (atk_roll_inter[roll.size() - 1].is_finished() && def_roll_inter[roll2.size() - 1].is_finished() && !start_inter_atk_roll && !start_inter_def_roll && UI::UIButton_2("Confirm", "Confirm", ImVec2{ viewport->Size.x * 0.5f, viewport->Size.y * 0.55f }, { 0,0 }, FONT_BODY))
		{
			// TODO: affect the entities

			//Turn off VFX
			{
				m_winningNumber_VFX.lock()->ClearAllParticles();
				m_winningNumber_VFX.lock()->m_MM.m_duration = 0.f;

				m_combatRoll_VFX.lock()->ClearAllParticles();
				m_combatRoll_VFX.lock()->m_MM.m_duration = 0.f;
			}

			///////////////////////// MOVE THIS TO END STATE OF CINEMATIC //////////////////////
			//if (charac.get_stat(0) + charac.get_statDelta(0) <= 0)
			//	display_other_end_cycle = true;

			//else
			//{
			//	instance.selected = INVALID;
			//	other_entity = INVALID;
			//	battle_state = BATTLE_STATE::CURR_TURN; // temp testing
			//	state = State::MENU;

			//	//  TODO: change to the next entity turn
			//	selected_action = UNDEFINED;
			//	selected_weapon = UNDEFINED;
			//}
			auto fn = [&]()
			{
				battle_state = BATTLE_STATE::CURR_TURN; // temp testing
				state = State::CINEMATIC;
				start_inter_atk_roll = true;
				start_inter_def_roll = true;
			};

			Service<EventManager>::Get().instant_dispatch<WipeTrigger>(WipeTrigger(.15f, .15f, 0.f, fn));

		}

		// this stuff moved to show_cinematic state
		//if (display_other_end_cycle)
		//{

		//	// pop up of three ending options
		//	// TODO: shift to new function

		//	ImVec4 borderCol = { 0.980f, 0.768f, 0.509f, 1.f };
		//	ImGui::OpenPopup("End Unit Cycle");
		//	ImGui::SetNextWindowPos(ImVec2{ viewport->Size.x * 0.5f, viewport->Size.y * 0.5f }, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		//	ImGui::SetNextWindowSize(ImVec2(500, 550));

		//	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.f);
		//	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
		//	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.f });
		//	ImGui::PushStyleColor(ImGuiCol_Border, borderCol);
		//	ImGui::PushStyleColor(ImGuiCol_PopupBg, { 0.06f,0.06f, 0.06f, 0.85f });

		//	ImGuiWindowFlags flags{ ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
		//					   ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove };

		//	if (ImGui::BeginPopupModal("End Unit Cycle", NULL, flags))
		//	{
		//		static int end_selection = -1;
		//		// background
		//		ImVec2 winMin = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y };
		//		ImVec2 TextMin = { ImGui::GetWindowPos().x + 10.f, ImGui::GetWindowPos().y + 5.f };
		//		ImVec2 winMax = { winMin.x + ImGui::GetWindowWidth() * 0.5f, winMin.y + ImGui::GetWindowHeight() * 0.05f };
		//		ImVec4 col = { 0.980f, 0.768f, 0.509f, 1.f };
		//		ImVec4 textcol = { 0,0,0,1 };

		//		// text
		//		ImGui::GetWindowDrawList()->AddRectFilled({ winMin.x, winMin.y }, { winMax.x, winMax.y }, ImGui::GetColorU32(col));
		//		ImGui::PushFont(FONT_OPEN);
		//		ImGui::GetWindowDrawList()->AddText({ TextMin.x, TextMin.y }, ImGui::GetColorU32({ 0,0,0,1 }), std::string{ charac.name + "'s HP has reached 0" }.c_str());
		//		ImGui::PopFont();

		//		// halftone
		//		auto halfToneImg = tex_map["Assets/HalftoneWhite.dds"];
		//		ImVec2 htMin = { winMin.x, winMin.y + ImGui::GetWindowHeight() * 0.55f };
		//		ImVec2 htMax = { htMin.x + halfToneImg->GetWidth(), htMin.y + halfToneImg->GetHeight() };
		//		ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(halfToneImg->GetID()), htMin, htMax);

		//		ImGui::BeginChild("##SimualteStuff", ImVec2{ 500,470 });

		//		if (UI::UIButton_2("Revive with full HP", "Revive with full HP", ImVec2{ ImGui::GetContentRegionMax().x * 0.5f, ImGui::GetContentRegionMax().y * 0.35f },
		//			ImVec2{ 0,0 }, FONT_PARA, end_selection == 0))
		//		{
		//			end_selection = 0;
		//		}

		//		if (UI::UIButton_2("Delete from map", "Delete from map", ImVec2{ ImGui::GetContentRegionMax().x * 0.5f, ImGui::GetContentRegionMax().y * 0.5f },
		//			ImVec2{ 0,0 }, FONT_PARA, end_selection == 1))
		//		{
		//			end_selection = 1;
		//		}

		//		if (UI::UIButton_2("Ignore", "Ignore", ImVec2{ ImGui::GetContentRegionMax().x * 0.5f, ImGui::GetContentRegionMax().y * 0.65f },
		//			ImVec2{ 0,0 }, FONT_PARA, end_selection == 2))
		//		{
		//			end_selection = 2;
		//		}

		//		ImGui::EndChild();

		//		// check for stuff here
		//		if (UI::UIButton_2("Confirm", "Confirm", { ImGui::GetContentRegionMax().x * 0.5f, ImGui::GetContentRegionMax().y * 0.9f }, { 0.f, 0.f }, FONT_PARA) && end_selection != -1)
		//		{
		//			switch (end_selection)
		//			{
		//			case 0:
		//				// full health
		//				charac.set_statDelta(0, 0);
		//				break;
		//			case 1:
		//				// totally remove from map
		//				units.erase(std::remove(units.begin(), units.end(), other_entity), units.end());
		//				instance.ecs.destroy(other_entity);
		//				break;
		//			case 2:
		//				// ignore
		//				units.erase(std::remove(units.begin(), units.end(), other_entity), units.end());
		//				break;
		//			default:
		//				break;
		//			}

		//			end_selection = -1;
		//			display_other_end_cycle = false;
		//			ImGui::CloseCurrentPopup();

		//			// clear
		//			instance.selected = INVALID;
		//			other_entity = INVALID;
		//			battle_state = BATTLE_STATE::CURR_TURN; // temp testing
		//			state = State::MENU;

		//			//  TODO: change to the next entity turn
		//			selected_action = UNDEFINED;
		//			selected_weapon = UNDEFINED;
		//		}
		//	}

		//	ImGui::EndPopup();

		//	ImGui::PopStyleVar(3);
		//	ImGui::PopStyleColor(2);
		//}
	}
	
	void CombatModeOverlay::show_cinematic([[maybe_unused]] RuntimeInstance& instance)
	{
		const float dist_from_char = 2.f;
		const float dist_from_ground = 1.f;
		const float angle = 45.f;
		const float oangle = -angle;

		static vec3 cam_pos;
		static quat cam_rot;
		static bool triggered = false;

		static int jump_times = 0;

		static vec3 old_pos;;
		static uint32_t rand1;
		static uint32_t rand2;


		if (curr_entity && instance.ecs.has<tc::Character>(curr_entity) && instance.ecs.has<tc::Transform>(curr_entity) && instance.ecs.has<tc::Unit>(curr_entity)
			&& other_entity && instance.ecs.has<tc::Character>(other_entity) && instance.ecs.has<tc::Transform>(other_entity) && instance.ecs.has<tc::Unit>(other_entity))
		{
			[[maybe_unused]]auto& cs = instance.ecs.get<tc::Character>(curr_entity);
			[[maybe_unused]] auto& unit = instance.ecs.get<tc::Unit>(curr_entity);
			[[maybe_unused]] auto& xform = instance.ecs.get<tc::Transform>(curr_entity);

			[[maybe_unused]] auto& ocs = instance.ecs.get<tc::Character>(other_entity);
			[[maybe_unused]] auto& ounit = instance.ecs.get<tc::Unit>(other_entity);
			[[maybe_unused]] auto& oxform = instance.ecs.get<tc::Transform>(other_entity);

			[[maybe_unused]] auto& cam = Service<RenderSystem>::Get().GetCamera();

			auto back_to_main = [&]() {

				// reset model
				
			//	auto anim = instance.ecs.get_if<tc::Animation>(curr_entity);
			//	anim->stop(curr_entity);
			//	anim->change_animation(curr_entity, "../../../Resource/Animations/Unit_Idle.fbx");
			//	anim->play(curr_entity, true);

			//	instance.ecs.get<tc::Model>(curr_entity).path = "Models\\UnitBlack_CombatStance.a";
			//	if (other_entity != INVALID)
			//		instance.ecs.get_if<tc::Animation>(other_entity)->stop(other_entity);
					//instance.ecs.get<tc::Model>(other_entity).path = "Models\\UnitBlack_CombatStance.a";

				cam.SetPosition(cam_pos);
				cam.SetRotation(cam_rot);

				cam_ctrl.force_reset_pos(cam);
				cam_ctrl.force_reset_rot(cam);

				// clear
				instance.selected = INVALID;
				other_entity = INVALID;
				battle_state = BATTLE_STATE::CURR_TURN; // temp testing
				state = State::MENU;

				//  TODO: change to the next entity turn
				selected_action = UNDEFINED;
				selected_weapon = UNDEFINED;

				// reset menu
				reset_menu();
			};

		// use battle state for internal state
		switch (battle_state)
		{
		case Tempest::CombatModeOverlay::BATTLE_STATE::CURR_TURN: // init
			// start interpolater
			
			battle_state = BATTLE_STATE::SELECT_ACTION;
			
			// save old cam pos
			cam_pos = cam.GetPosition();
			cam_rot = cam.GetQuatRotation();

			// telepoint the camera to the player
			auto new_cam_rot = glm::inverse(xform.rotation) *glm::angleAxis(glm::radians(angle), vec3{ 0,1,0 });
			auto dir = vec3{ 0,0,-1 } * dist_from_char;
			dir = dir * new_cam_rot;
			auto new_cam_pos = xform.position + dir;
			new_cam_pos.y = dist_from_ground;
			cam.SetPosition(new_cam_pos);


			auto yaw = glm::angleAxis(3 * glm::pi<float>()/2.f, glm::vec3{ 0, 1, 0 });
			cam.SetRotation(xform.rotation * yaw);
			/*auto right = vec3{ 1,0,0 } * new_cam_rot;
			auto down = glm::angleAxis(glm::radians(90.f), right);
			cam.SetRotation(down);*/
			auto look_at_pos = xform.position;
			look_at_pos.y = dist_from_ground;
			cam_ctrl.look_at(cam, look_at_pos, 1.f);

			[[fallthrough]];
		case Tempest::CombatModeOverlay::BATTLE_STATE::SELECT_ACTION: // pan the camera if any

			if (cam_ctrl.is_still())
			{
				// once inter is done go the next shit
				battle_state = BATTLE_STATE::SELECT_WEAPON;
				// set up for next shit also

				// play whatever anim u want here
				old_pos = xform.position;
				// for now just jump on the spot
				/*tvector<glm::ivec2> v;
				int p_x = (int)std::floor(oxform.position.x);
				int p_y = (int)std::floor(oxform.position.z);
				v.push_back({ p_x , p_y });
				unit.set_path(v, xform);*/

				unit.attack();

				triggered = false;

				AudioEngine ae;
				ae.Play("Sounds2D/SFX_UnitAttackVoice" + std::to_string(rand() % 4 + 1) + ".wav", "SFX", 1.0f);

				// Attack VFX
				// PSEUDO (well this does not work either when changing from idle to punch)
				if (beginAttack)
				{
					beginAttack = false;
					EmitterSystem_3D::GetInstance().CreateChracterChargedAttackEmitter(m_characterAttackEmitter, xform.position);
				}

				// PSEUDO 
				// instead of jumping onto the enemy, wobble back-front
				auto anim = instance.ecs.get_if<tc::Animation>(curr_entity);
			//	anim->stop(curr_entity);
				anim->change_animation(curr_entity, "../../../Resource/Animations/Unit_Punch.fbx");
				anim->play(curr_entity);
			//	instance.ecs.get<tc::Model>(curr_entity).path = "Models\\Unit_Punch.fbx";
			}


			break;
		case Tempest::CombatModeOverlay::BATTLE_STATE::SELECT_WEAPON: // attack
		{

			// once done -> we can go to the next fella
			// wipe
			if (!unit.is_attacking() && !triggered)
			{
				triggered = true;
				// reset the attacker pos
				inter1.start(0, 1, 0.7f);
				auto fn = [&, oxform, dist_from_char, dist_from_ground, angle]()
				{
					auto new_cam_rot = glm::inverse(oxform.rotation) *glm::angleAxis(glm::radians(-angle), vec3{ 0,1,0 });
					auto dir = vec3{ 0,0,-1 } *dist_from_char;
					dir = dir * new_cam_rot;
					auto new_cam_pos = oxform.position + dir;
					new_cam_pos.y = dist_from_ground;

					xform.position = old_pos;

					cam.SetPosition(new_cam_pos);
					cam.SetRotation(new_cam_rot * glm::angleAxis(glm::radians(180.f), vec3{ 0,1,0 }));

					/*auto look_at_pos = oxform.position;
					look_at_pos.y = dist_from_ground;
					cam_ctrl.look_at(cam, look_at_pos, 2.f);*/
				};

				Service<EventManager>::Get().instant_dispatch<WipeTrigger>(WipeTrigger(.15f, .15f, 0.f, fn));
			}
			if (inter1.is_finished() && triggered)
			{
				battle_state = BATTLE_STATE::SELECT_OTHER;

				/*tvector<glm::ivec2> v;
				int p_x = (int)std::floor(xform.position.x);
				int p_y = (int)std::floor(xform.position.z);
				v.push_back({ p_x , p_y });
				p_x = (int)std::floor(oxform.position.x);
				p_y = (int)std::floor(oxform.position.z);
				v.push_back({ p_x , p_y });
				unit.set_path(v, xform);*/


				triggered = false;
				damageOnce = false;

				inter1.start(0, 1, 0.1f);

				if (!beginAttack)
				{
					beginAttack = true;
					// Turn off the particle
					if(!m_characterAttackEmitter.expired())
						m_characterAttackEmitter.lock()->m_MM.m_duration = 0.0f;
				}

				//PSEUDO
				// make enemy wobble left-right
				auto anim = instance.ecs.get_if<tc::Animation>(other_entity);
			//	anim->stop(other_entity);
				anim->change_animation(other_entity, "../../../Resource/Animations/Unit_Block.fbx");
				anim->play(other_entity);
				//instance.ecs.get<tc::Model>(other_entity).path = "Models\\Unit_Block.fbx";
			}
		}
		break;
		case Tempest::CombatModeOverlay::BATTLE_STATE::SELECT_OTHER:
			// once done -> we can go to the next fella
			// wipe
			if (inter1.is_finished() && !triggered)
			{
				ounit.get_hit(10, 2.f);


				triggered = true;
				//if (damage == 0)
				//{
				//	inter2.start(0, 1, 2.f);
				//	tvector<glm::ivec2> v;
				//	int p_x = (int)std::floor(oxform.position.x);
				//	int p_y = (int)std::floor(oxform.position.z);
				//	v.push_back({ p_x , p_y });
				//	/*p_x = (int)std::floor(xform.position.x);
				//	p_y = (int)std::floor(xform.position.z);*/

				//	glm::vec3 a = glm::inverse(oxform.rotation) * vec3{ 0,0,-1 };

				//	v.push_back({ p_x + (int)std::round(a.x) , p_y + (int)std::round(a.z) });
				//	ounit.set_path(v, oxform);
				//}
				//else
				{

					rand1 = els::random::uniform_rand(0, 3);
					rand2 = els::random::uniform_rand(0, 3);;
					

					float sec = 2.5f;
					float delay = 1.7f;
					inter3.start(0, 1.f, sec, 0.f, [](float x) { return glm::circularEaseOut(x); });
					inter4.start(0, 1.f, sec, 0.f, [](float x) { return glm::sineEaseIn(x); });
					inter5.start(0, 1.f, 0.5f, 0.f, [](float x) { return glm::sineEaseOut(x); });
					inter6.start(0, 1.f, delay, 0.f, [](float x) { return glm::sineEaseIn(x); });


					inter_nest[0].start(2.0f, 1.0f, delay, 0.f,
						[](float x) { return glm::elasticEaseOut(x);
						});
					inter_nest[1].start(2.0f, 1.0f, delay - 0.05f, 0.05f,
						[](float x) { return glm::elasticEaseOut(x);
						});
					inter_nest[2].start(2.0f, 1.0f, delay - 0.1f, 0.1f,
						[](float x) { return glm::elasticEaseOut(x);
						});

					cam_ctrl.shake(cam, .25f, .5f, 17.f);

					auto fn = []()
					{
						auto fn = []()
						{
							Service<EventManager>::Get().instant_dispatch<WipeColourTrigger>(WipeColourTrigger({ 1,1,1 }, .05f, .05f, 0.0f));
						};
						auto empty = []() {};

						Service<EventManager>::Get().instant_dispatch<WipeColourTrigger>(WipeColourTrigger({ 1,1,1 }, .05f, .05f, 0.0f, empty, fn));
					};

					auto empty = []() {};

					Service<EventManager>::Get().instant_dispatch<WipeColourTrigger>(WipeColourTrigger({ 1,1,1 }, .05f, .05f, 0.0f, empty, fn));
				}

				AudioEngine ae;

				ae.Play("Sounds2D/SFX_Hit.wav", "SFX", 1.0f);

				// dead voice
				if (ocs.get_stat(0) + ocs.get_statDelta(0) <= 0)
				{
					ae.Play("Sounds2D/SFX_UnitDeathVoice" + std::to_string(rand() % 4 + 1) + ".wav", "SFX", 1.0f);

					// Character Death Emitter Data
					glm::vec3 minRangeSpawnPos = oxform.position;
					glm::vec3 maxRangeSpawnPos = oxform.position;

					minRangeSpawnPos.x -= 1.0f;
					minRangeSpawnPos.y += 0.3f;
					minRangeSpawnPos.z -= 1.0f;

					maxRangeSpawnPos.x += 1.0f;
					maxRangeSpawnPos.y += 0.7f;
					maxRangeSpawnPos.z += 1.0f;

					glm::vec4 colourBegin;
					colourBegin.r = ocs.color.r;
					colourBegin.g = ocs.color.g;
					colourBegin.b = ocs.color.b;
					colourBegin.a = 1.0f;

					if (m_characterDeathEmitter.expired())
						m_characterDeathEmitter = EmitterSystem_3D::GetInstance().CreateChracterDeathEmitter(oxform.position, minRangeSpawnPos, maxRangeSpawnPos, 3, colourBegin, colourBegin);
					else
					{
						m_characterDeathEmitter.lock()->m_GM.m_position = oxform.position;
						m_characterDeathEmitter.lock()->m_minPos = minRangeSpawnPos;
						m_characterDeathEmitter.lock()->m_maxPos = maxRangeSpawnPos;
						m_characterDeathEmitter.lock()->m_MM.m_duration = 0.3f;
						m_characterDeathEmitter.lock()->m_GM.m_active = true;
						m_characterDeathEmitter.lock()->m_MM.m_preWarm = true;
					}
				}
			}

			if (triggered)
			{
				auto position = instance.ecs.get<tc::Transform>(other_entity).position;
				position.y += 1.4f + inter3.get() * .25f;
				auto ss = cam.WorldspaceToScreenspace(position);
				auto vp = cam.GetViewport();
				ss.x = (1 + ss.x) / 2 * vp.z;
				ss.y = vp.w - ((1 + ss.y) / 2 * vp.w);
				auto temp = ImGui::GetCursorPos();

				// Draw the damage
				{
					glm::vec3 col(1, 0, 0);

					std::string text = std::to_string(damage);

					if (damage == 0)
						col = vec3(1, 1, 1);
					else if (damage < 0)
					{
						text = std::to_string(-damage);
						col = vec3(0, 1, 0);
					}
					else
					{
						auto emitterPosition = oxform.position;
						emitterPosition.y += 1.0f;

						glm::vec4 colourBegin;
						colourBegin.r = ocs.color.r;
						colourBegin.g = ocs.color.g;
						colourBegin.b = ocs.color.b;
						colourBegin.a = 1.0f;

						if (m_characterDamageEmitter.expired())
							m_characterDamageEmitter = EmitterSystem_3D::GetInstance().CreateChracterDamageEmitter(emitterPosition, colourBegin, colourBegin);
						else if (damageOnce == false)
						{
							m_characterDamageEmitter.lock()->m_GM.m_position = emitterPosition;
							//m_characterDamageEmitter.lock()->Emit(1);
							m_characterDamageEmitter.lock()->m_EM.m_burstCycle = 1;
							m_characterDamageEmitter.lock()->m_MM.m_duration = 0.6f;
							m_characterDamageEmitter.lock()->m_GM.m_active = true;
							m_characterDamageEmitter.lock()->m_MM.m_preWarm = true;
							damageOnce = true;
						}
					}

					//ImGui::PushFont(FONT_HEAD);
					//auto text_size = ImGui::CalcTextSize(text.c_str());
					//ImGui::PopFont();
					//auto cursor_pos = ImVec2{ ss.x - text_size.x / 2.f, ss.y - text_size.y / 2 };
					//ImGui::SetCursorPos(cursor_pos);

					//ImGui::BeginGroup();
					//for (int i = 0; i < text.size() && i < 3; ++i)
					//{
					//	float scale = FONT_HEAD->Scale;
					//	FONT_HEAD->Scale = inter_nest[i].get();
					//	ImGui::PushFont(FONT_HEAD);
					//	// the text colour
					//	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(col.r, col.g, col.b, 1.f - inter4.get()));
					//	// the text thingy
					//	ImGui::Text(std::string{ text[i] }.c_str());
					//	ImGui::SameLine(0);

					//	ImGui::PopStyleColor();
					//	ImGui::PopFont();
					//	FONT_HEAD->Scale = scale;
					//}
					//ImGui::EndGroup();

				}
				ImGui::SetCursorPos(temp);
			}

			if (triggered)
			{
				auto position = instance.ecs.get<tc::Transform>(other_entity).position;
				position.y += 1.0f + 1.f * .3f;
				auto ss = cam.WorldspaceToScreenspace(position);
				auto vp = cam.GetViewport();
				ss.x = (1 + ss.x) / 2 * vp.z;
				ss.y = vp.w - ((1 + ss.y) / 2 * vp.w);
				auto temp = ImGui::GetCursorPos();

				// Draw the damage
				{

					const char* sads[] = { ICON_FA_SAD_TEAR, ICON_FA_SAD_CRY, ICON_FA_FROWN, ICON_FA_FROWN_OPEN };
					const char* happys[] = { ICON_FA_GRIN_TONGUE_SQUINT, ICON_FA_LAUGH_BEAM, ICON_FA_LAUGH_SQUINT, ICON_FA_LAUGH_WINK };
					const char* deads[] = { ICON_FA_SKULL_CROSSBONES, ICON_FA_SKULL };

					std::string text; 
					text = ICON_FA_CIRCLE " ";
					text += std::to_string(damage);
					string word = "DAMAGE";
					glm::vec3 col;;
					col = vec3(1, 0, 0);

					if (damage == 0)
					{
						col = vec3(1, 0.6, 0);
						text = ICON_FA_CIRCLE " !";
						word = "MISSED!";
					}
					if (ocs.get_stat(0) + ocs.get_statDelta(0) <= 0)
					{
						// for dead
					}

					FONT_HEAD->Scale = 0.1f + 0.9f * inter5.get();
					ImGui::PushFont(FONT_HEAD);
					// the text colour
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(col.r, col.g, col.b, inter5.get()));
					auto text_size = ImGui::CalcTextSize(text.c_str());
					//auto cursor_pos = ImVec2{ ss.x - text_size.x / 2.f, ss.y - text_size.y / 2 };
					auto cursor_pos = ImVec2{ ss.x , ss.y - text_size.y};
					ImGui::SetCursorPos(cursor_pos);
					{
						ImGui::BeginGroup();
						// the text thingy
						//ImGui::Text(text.c_str());
						auto drawlist = ImGui::GetWindowDrawList();
						ImVec2 padding = { 10.f,10.f };
						ImVec2 minRedBox = { cursor_pos.x - padding.x, cursor_pos.y - padding.y };
						ImVec2 maxRedBox = minRedBox + (ImGui::CalcTextSize(text.c_str()) + padding * 2);// *inter5.get();
						maxRedBox.y -= 4.f;

						drawlist->AddRectFilled(minRedBox, maxRedBox, ImGui::GetColorU32({ col.r, col.g, col.b,inter5.get() }));
						drawlist->AddText(minRedBox + padding, ImGui::GetColorU32({ 0,0,0,inter5.get() }), text.c_str());

						ImVec2 minGreyBox = { maxRedBox.x, minRedBox .y};
						ImVec2 maxGreyBox = minGreyBox + (ImGui::CalcTextSize(word.c_str()) + padding * 2);// *inter5.get();
						drawlist->AddRectFilled(minGreyBox, maxGreyBox, ImGui::GetColorU32({ 0,0,0,0.75f * inter5.get() }));
						drawlist->AddText(minGreyBox + padding, ImGui::GetColorU32({ 0.8f,.8f,.8f,inter5.get() }), word.c_str());


						ImGui::EndGroup();
					}
					ImGui::PopStyleColor();
					ImGui::PopFont();
					FONT_HEAD->Scale = 1.f;
				}
				ImGui::SetCursorPos(temp);
			}


			if (inter2.is_finished() && inter3.is_finished() && inter5.is_finished() && inter6.is_finished() && triggered)
			{
				triggered = false;
				inter1.start(0, 1, 1.f); // just to make sure we do not activate the top shit

				if (ocs.get_stat(0) + ocs.get_statDelta(0) <= 0)
				{
					battle_state = BATTLE_STATE::BATTLE_GLIMPSE;
				}
				else
				{
					Service<EventManager>::Get().instant_dispatch<WipeTrigger>(WipeTrigger(.15f, .15f, 0.f, back_to_main));
				}
			}

			break;
		case Tempest::CombatModeOverlay::BATTLE_STATE::BATTLE_GLIMPSE:
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImVec4 borderCol = { 0.980f, 0.768f, 0.509f, 1.f };
			ImGui::OpenPopup("End Unit Cycle");
			ImGui::SetNextWindowPos(ImVec2{ viewport->Size.x * 0.5f, viewport->Size.y * 0.5f }, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
			ImGui::SetNextWindowSize(ImVec2(600, 300));

			ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove;

			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2.f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.f });
			ImGui::PushStyleColor(ImGuiCol_Border, borderCol);
			ImGui::PushStyleColor(ImGuiCol_PopupBg, { 0.06f,0.06f, 0.06f, 0.85f });

			if (ImGui::BeginPopupModal("End Unit Cycle", nullptr, flags))
			{
				ImVec2 winMin = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y };
				ImVec2 TextMin = { ImGui::GetWindowPos().x + 10.f, ImGui::GetWindowPos().y + 2.5f };
				ImVec2 winMax = { winMin.x + ImGui::GetWindowWidth() * 0.25f, winMin.y + ImGui::GetWindowHeight() * 0.075f };
				ImVec4 col = { 0.980f, 0.768f, 0.509f, 1.f };
				ImVec4 textcol = { 0,0,0,1 };

				auto bgImg = tex_map["Assets/Popup_Backdrop.dds"];
				auto warnImg = tex_map["Assets/YellowWarningIco.dds"];
				string te = "CONFIRMATION";
				ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(bgImg->GetID()), winMin, { winMin.x + ImGui::GetWindowWidth() * 0.8f,winMin.y + ImGui::GetWindowHeight() });
				ImGui::GetWindowDrawList()->AddRectFilled({ winMin.x, winMin.y }, { winMax.x, winMax.y }, ImGui::GetColorU32(col));

				ImGui::PushFont(FONT_OPEN);
				ImGui::GetWindowDrawList()->AddText({ TextMin.x, TextMin.y }
				, ImGui::GetColorU32({ 0,0,0,1 }), te.c_str());
				ImGui::PopFont();


				ImGui::Dummy({ 0.f, ImGui::GetWindowHeight() * 0.2f });
				ImGui::PushFont(FONT_SHEAD);
				auto windowWidth = ImGui::GetWindowSize().x;
				string warningstr = "";
				auto warningSize = ImGui::CalcTextSize(warningstr.c_str()).x;
				ImGui::PushStyleColor(ImGuiCol_Text, { 0.792f,0.22f,0.22f,1.f });
				ImGui::SetCursorPosX((windowWidth - warningSize) * 0.5f - ((float)warnImg->GetWidth() * 0.7f));
				ImGui::Image((void*)static_cast<size_t>(warnImg->GetID()), { (float)warnImg->GetWidth(), (float)warnImg->GetHeight()});
				ImGui::SameLine();
				ImGui::Text(warningstr.c_str());
				ImGui::PopStyleColor();
				ImGui::PopFont();

				ImGui::Dummy({ 0.f, ImGui::GetWindowHeight() * 0.05f });
				ImGui::PushFont(FONT_BODY);
				string str = ocs.name + " has died.";
				string str2 = "What would you like to do?";
				auto strSize = ImGui::CalcTextSize(str.c_str()).x;
				
				ImGui::SetCursorPosX((windowWidth - strSize) * 0.5f);
				ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetColorU32({ 1,0,0,1 }));
				ImGui::Text(ocs.name.c_str());
				ImGui::PopStyleColor();
				ImGui::SameLine();
				ImGui::Text(" has died.");
				ImGui::SetCursorPosX((windowWidth - ImGui::CalcTextSize(str2.c_str()).x) * 0.5f);
				ImGui::Text(str2.c_str());
				ImGui::PopFont();

				ImGui::SetCursorPosX(0);
				ImGui::SetCursorPosY(0);
				if (UI::UIButton_2("Revive", "Revive", { ImGui::GetCursorPosX() + ImGui::GetWindowWidth() * 0.205f, ImGui::GetCursorPosY() + ImGui::GetWindowHeight() * 0.8f }, { -30.f, 0.f }, FONT_PARA))
				{
					ocs.set_statDelta(0, 0);
					ImGui::CloseCurrentPopup();
					Service<EventManager>::Get().instant_dispatch<WipeTrigger>(WipeTrigger(.15f, .15f, 0.f, back_to_main));
				}
				ImGui::SetCursorPosX(0);
				ImGui::SetCursorPosY(0);
				if (UI::UIButton_2("Delete", "Delete", { ImGui::GetCursorPosX() + ImGui::GetWindowWidth() * 0.495f, ImGui::GetCursorPosY() + ImGui::GetWindowHeight() * 0.8f }, { -30.f, 0.f }, FONT_PARA))
				{
					EmitterSystem_3D::GetInstance().CreateSmokePoofEmitter(m_unit_Remove_VFX, instance.ecs.get<tc::Transform>(other_entity).position);

					units.erase(std::remove(units.begin(), units.end(), other_entity), units.end());
					units_results.erase(other_entity); //halp
					instance.ecs.destroy(other_entity);
					other_entity = INVALID;
					ImGui::CloseCurrentPopup();
					Service<EventManager>::Get().instant_dispatch<WipeTrigger>(WipeTrigger(.15f, .15f, 0.f, back_to_main));
				}
				ImGui::SetCursorPosX(0);
				ImGui::SetCursorPosY(0);
				if (UI::UIButton_2("Ignore", "Ignore", { ImGui::GetCursorPosX() + ImGui::GetWindowWidth() * 0.785f, ImGui::GetCursorPosY() + ImGui::GetWindowHeight() * 0.8f }, { -30.f, 0.f }, FONT_PARA))
				{
					units.erase(std::remove(units.begin(), units.end(), other_entity), units.end());
					if (auto dead = instance.scene.get_prototype_if("Unit", "Dead"))
					{
						// create a new entity
						/*auto new_e = instance.ecs.create(dead->instance());
						LOG_ASSERT(instance.ecs.has<tc::Transform>(new_e));
						LOG_ASSERT(instance.ecs.has<tc::Character>(new_e));

						instance.ecs.get<tc::Transform>(new_e) = oxform;
						instance.ecs.get<tc::Character>(new_e) = ocs;

						instance.ecs.destroy(other_entity);*/

						LOG_ASSERT(dead->get_if<tc::Model>());
						LOG_ASSERT(dead->get_if<tc::Local>());
						//LOG_ASSERT(instance.ecs.has<tc::Character>(new_e));
						
						ounit.die();
						instance.ecs.get<tc::Model>(other_entity) = *dead->get_if<tc::Model>();
						instance.ecs.get<tc::Local>(other_entity) = *dead->get_if<tc::Local>();
					}
					other_entity = INVALID;
					ImGui::CloseCurrentPopup();
					Service<EventManager>::Get().instant_dispatch<WipeTrigger>(WipeTrigger(.15f, .15f, 0.f, back_to_main));
				}
				
			}
			ImGui::EndPopup();
			ImGui::PopStyleVar(3);
			ImGui::PopStyleColor(2);

		}
			break;
		case Tempest::CombatModeOverlay::BATTLE_STATE::COMMENCE_BATTLE:
		{
			auto fn = [&]()
			{
				battle_state = BATTLE_STATE::CURR_TURN; // temp testing
				state = State::MENU;
			};

			Service<EventManager>::Get().instant_dispatch<WipeTrigger>(WipeTrigger(.15f, .15f, 0.f, fn));
		}
			break;
		default:
			break;
		}


		}
		else
		{
			battle_state = BATTLE_STATE::CURR_TURN;
			state = State::MENU;

			//PSEUDO
			// change back the models
		//	instance.ecs.get_if<tc::Animation>(other_entity)->stop(other_entity);
		//	instance.ecs.get_if<tc::Animation>(curr_entity)->stop(curr_entity);
		//	instance.ecs.get<tc::Model>(other_entity).path = "Models\\UnitBlack_CombatStance.a";
		//	instance.ecs.get<tc::Model>(curr_entity).path = "Models\\UnitBlack_CombatStance.a";
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
				UI::CharacterTurnData(instance, character, { 0.f + menu1.get() * 500.f, viewport->Size.y - placeholder_height });

				if (UI::UIButton_2("More Information >", "More Information >", ImVec2{ action_background_size.x * 0.72f + menu1.get() * 1000.f, viewport->Size.y - action_background_size.y * 1.2f }, { 0.f, 8.0f }, FONT_BODY))
				{
					display_curr_stat = true;
				}
			}
			else
			{
				auto more_info_BG = tex_map["Assets/MIBG.dds"];
				const ImVec2 BG_size{ more_info_BG->GetWidth() * 1.0f, more_info_BG->GetHeight() * 1.0f};

				ImGui::SetCursorPos(ImVec2{ 0.f, viewport->Size.y - BG_size.y });
				if (ImGui::BeginChild(std::string{ "More Info" + std::to_string(character) }.c_str(), BG_size))
				{
					const ImVec2 end_pos{ ImGui::GetCursorPos() + BG_size };
					auto &charac = instance.ecs.get<tc::Character>(character);
					ImVec2 img_min = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y };
					ImVec2 img_max = { img_min.x + ImGui::GetWindowWidth(), img_min.y + ImGui::GetWindowHeight()};
					const ImVec2 content_region_offset{ ImGui::GetCursorPosX() +  ImGui::GetContentRegionAvailWidth() * 0.1f, ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y * 0.15f };

					ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(more_info_BG->GetID()), img_min, img_max);

					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0,0,0,0 });
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0,0,0,0 });
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0,0,0,0 });

					ImGui::BeginGroup();
					render_tabs(TABS_TYPE::STAT, [&]() {
						ImGui::SetCursorPos(content_region_offset);
						CombatModeOverlay::display_unit_stats(*viewport, instance, &charac);
					});

					render_tabs(TABS_TYPE::WEAPON, [&]() {
						ImGui::SetCursorPos(content_region_offset);
						CombatModeOverlay::display_weapon_stats(*viewport, instance, &charac);
					});

					render_tabs(TABS_TYPE::ACTION, [&]() {
						ImGui::SetCursorPos(content_region_offset);
						CombatModeOverlay::display_actions(*viewport, instance, &charac);
					});
					ImGui::EndGroup();

					ImGui::PopStyleColor(3);


					if (UI::UIButton_2("Confirm", "Confirm", ImVec2{ end_pos.x - 160.0f, end_pos.y - 80.0f }, { 0.f, 8.0f }, FONT_BODY))
					{
						display_curr_stat = false;
					}
				}
				ImGui::EndChild();

				
		/*		const float infoSize = viewport->Size.y * 0.8f / 3.0f;
				ImGui::SetCursorPos(ImVec2{ 0.f, viewport->Size.y * 0.2f });
				render_more_info(instance, *viewport, INFO_TYPE::CHAR, character);
				ImGui::SetCursorPos(ImVec2{ 0.f, viewport->Size.y * 0.2f + infoSize });
				render_more_info(instance, *viewport, INFO_TYPE::ACTIONS, character);
				ImGui::SetCursorPos(ImVec2{ 0.f, viewport->Size.y * 0.2f + infoSize * 2.0f });
				render_more_info(instance, *viewport, INFO_TYPE::WEAPONS, character);*/
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

				//Service<RenderSystem>::Get().DrawLine(box, color);

				if (m_unitTileEmitter.expired())
					m_unitTileEmitter = EmitterSystem_3D::GetInstance().CreateTileWaypointEmitter(glm::vec3(transform.position.x, transform.position.y, transform.position.z));
				else if (nextUnit)
				{
					nextUnit = false;
					// Update the position for the emitter
					if (!m_unitTileEmitter.expired())
					{
						auto tempUnitEmitter = m_unitTileEmitter.lock();
						tempUnitEmitter->UpdateWaypoints(transform.position);
					}
				}
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
				auto more_info_BG = tex_map["Assets/MIBG.dds"];
				const ImVec2 BG_size{ more_info_BG->GetWidth() * 1.0f, more_info_BG->GetHeight() * 1.0f };

				ImGui::SetCursorPos(ImVec2{ viewport->Size.x - BG_size.x, viewport->Size.y - BG_size.y });
				if (ImGui::BeginChild(std::string{ "More Info" + std::to_string(character) }.c_str(), BG_size))
				{
					const ImVec2 end_pos{ ImGui::GetCursorPos() + BG_size };
					auto& charac = instance.ecs.get<tc::Character>(character);
					ImVec2 img_min = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y };
					ImVec2 img_max = { img_min.x + ImGui::GetWindowWidth(), img_min.y + ImGui::GetWindowHeight() };
					const ImVec2 content_region_offset{ ImGui::GetCursorPosX() + ImGui::GetContentRegionAvailWidth() * 0.1f, ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y * 0.15f };

					ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(more_info_BG->GetID()), img_min, img_max);

					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0,0,0,0 });
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0,0,0,0 });
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0,0,0,0 });

					ImGui::BeginGroup();
					render_tabs(TABS_TYPE::STAT, [&]() {
						ImGui::SetCursorPos(content_region_offset);
						CombatModeOverlay::display_unit_stats(*viewport, instance, &charac);
					});

					render_tabs(TABS_TYPE::WEAPON, [&]() {
						ImGui::SetCursorPos(content_region_offset);
						CombatModeOverlay::display_weapon_stats(*viewport, instance, &charac);
					});

					render_tabs(TABS_TYPE::ACTION, [&]() {
						ImGui::SetCursorPos(content_region_offset);
						CombatModeOverlay::display_actions(*viewport, instance, &charac);
					});
					ImGui::EndGroup();

					ImGui::PopStyleColor(3);

					if (UI::UIButton_2("Confirm", "Confirm", ImVec2{ end_pos.x - 160.0f, end_pos.y - 80.0f }, { 0.f, 8.0f }, FONT_BODY))
					{
						display_other_stat = false;
					}
				}
				ImGui::EndChild();

			/*	push_style_color();

				const float infoSize = viewport->Size.y * 0.8f / 3.0f;
				ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.7f, viewport->Size.y * 0.2f });
				render_more_info(instance, *viewport, INFO_TYPE::CHAR, character);
				ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.7f, viewport->Size.y * 0.2f + infoSize });
				render_more_info(instance, *viewport, INFO_TYPE::ACTIONS, character);
				ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.7f, viewport->Size.y * 0.2f + infoSize * 2.0f });
				render_more_info(instance, *viewport, INFO_TYPE::WEAPONS, character);

				pop_style_color();*/
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

				//Service<RenderSystem>::Get().DrawLine(box, color);
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

		placeholder_height = (float)tex_map["Assets/Placeholder_Character.dds"]->GetHeight();
		action_background_size = ImVec2{ tex_map["Assets/ActionBackdrop.dds"]->GetWidth() * 1.0f, tex_map["Assets/ActionBackdrop.dds"]->GetHeight() * 1.0f};

		combat_button_tex[0] = tex_map["Assets/CActionUnselected.dds"];
		combat_button_tex[1] = tex_map["Assets/CItemUnselected.dds"];
		combat_button_tex[2] = tex_map["Assets/CMoveUnselected.dds"];

		battle_state = BATTLE_STATE::CURR_TURN; 
		state = State::MENU;

		auto& cam = Service<RenderSystem>::Get().GetCamera();
		cam_ctrl.set_fixed_camera(cam, 0 , 70);

		selected_action = UNDEFINED;
		other_selected_action = UNDEFINED;
		other_selected_weapon = UNDEFINED;
		selected_weapon = UNDEFINED;
		curr_turn = 0;
		curr_entity = units[curr_turn];

		reset_menu();

		units_results.clear();

		for (const auto this_unit : units)
		{
			units_results.emplace( this_unit, UnitResult{} );
		}
	};

	void CombatModeOverlay::visibility(const Event& e)
	{
		OverlayOpen = event_cast<CombatModeVisibility>(e).isVisible;
		battle_state = BATTLE_STATE::CURR_TURN;
	}

	void CombatModeOverlay::change_turn_order(const Event& e)
	{
		units = event_cast<ChangeTurnOrder>(e).entities;

		/*for (const auto this_unit : units)
		{
			if (!std::any_of(submitted_units.begin(), submitted_units.end(), [&](const auto submitted) {
				return submitted == this_unit;
			}))
			{
				submitted_units.erase(std::remove(submitted_units.begin(), submitted_units.end(), this_unit), submitted_units.end());
			}
		}*/
		curr_entity = units.front();
		curr_turn = 0;

		// add unit to results if it's not inside
		for (const auto this_unit : units)
		{
			if (units_results.find(this_unit) == units_results.end())
				units_results.emplace( this_unit, UnitResult{} );
		}
	}

	void CombatModeOverlay::show(Instance& instance)
	{
		if (!dynamic_cast<RuntimeInstance*>(&instance))
			return;

		{
			float dt = ImGui::GetIO().DeltaTime;

			inter1.update(dt);
			inter2.update(dt);
			inter3.update(dt);
			inter4.update(dt);
			inter5.update(dt);
			inter6.update(dt);
			menu1.update(dt);
			menu2.update(dt);
			for (int i = 0; i < 3; ++i)
			{
				atk_roll_inter[i].update(dt);
				def_roll_inter[i].update(dt);
			}

			for (auto& i : inter_nest)
				i.update(dt);

				banner.update(dt);
			if (banner.is_finished())
				banner.start(1, 0, 10);

			timer += dt;
		}

		//// jankass stuff
		//for (const auto this_unit : units)
		//{
		//	if (!std::any_of(submitted_units.begin(), submitted_units.end(), [&](const auto submitted) {
		//		return submitted == this_unit;
		//	}))
		//	{
		//		Service<RenderSystem>::Get().SubmitModel("../../../Resource/Models/Unit_Idle.fbx", instance.ecs.get<tc::Transform>(this_unit), this_unit);
		//		submitted_units.emplace_back(this_unit);
		//	}
		//}


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

				// just for testing
				if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Z)))
				{
					instance.tutorial_enable = true;
					instance.tutorial_level = 1;
					tutorial_index = 0;

				}

				if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_X)))
				{
					if (tutorial_index++ >= 3)
					{
						instance.tutorial_enable = false;
						tutorial_index = 0;
					}
				}

				if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)))
				{
					Service<EventManager>::Get().instant_dispatch<PauseOverlayTrigger>(battle_state == BATTLE_STATE::CURR_TURN, true);
					Service<EventManager>::Get().instant_dispatch<SendCombatResults>(units_results);
				}

				if (instance.tutorial_enable && !instance.tutorial_temp_exit)
				{
					auto drawlist = ImGui::GetForegroundDrawList();
					//if (instance.tutorial_level != 1) //set Slide to false if not tut level 1
					//	instance.tutorial_slide = false;
					if (instance.tutorial_level == 1)
					{
						switch (tutorial_index)
						{
						case 0:
						{
							ImVec2 pos = { viewport->Size.x * 0.86f, viewport->Size.y * 0.86f };
							ImVec2 size = { 240.f, 130.f };
							UI::TutArea(pos, size);
							string str = string(ICON_FK_EXCLAMATION_CIRCLE) + "Click here to move your unit";
							drawlist->AddText({ pos.x - ImGui::CalcTextSize(str.c_str()).x - 10.f, pos.y + size.y * 0.5f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());

							// drawing the tips
							str = "Moving Units";
							ImGui::PushFont(FONT_BTN);
							drawlist->AddText({ viewport->Size.x * 0.65f,viewport->Size.y * 0.7f }, ImGui::GetColorU32({ 0.98f,0.768f,0.51f,1 }), str.c_str());
							ImGui::PopFont();

							drawlist->AddLine({viewport->Size.x * 0.65f, viewport->Size.y * 0.7f + 20.f }, {viewport->Size.x, viewport->Size.y * 0.7f + 20.f }, ImGui::GetColorU32({ 1,1,1,1 }), 2.f);

							str = "Move this unit next to the other unit in order to get in range for an attack.";
							drawlist->AddText({ viewport->Size.x * 0.65f, viewport->Size.y * 0.7f + 25.f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());
						}
						break;

						case 1:
						{
							// user is moving the unit
							 if (!instance.ecs.get<tc::Unit>(curr_entity).is_moving() && state != State::MOVING)
							 {
								tutorial_index = 0;

								auto cs = instance.ecs.get<tc::Character>(curr_entity);
								auto position = instance.ecs.get<tc::Transform>(curr_entity).position;

								int p_x = (int)std::floor(position.x);
								int p_y = (int)std::floor(position.z);
								calculate_range(runtime, p_x, p_y, cs.get_stat(3) + cs.get_statDelta(3), range_map, visited);

								for (auto& [x, m] : range_map)
									for (auto [y, i] : m)
									{
										if (i >= 3) // player nearby
										{
											tutorial_index = 2;
											break;
										}

									}
							 }
						}
						break;

						case 2:
						{
							ImVec2 pos = { viewport->Size.x * 0.73f, viewport->Size.y * 0.86f };
							ImVec2 size = { 240.f, 130.f };
							UI::TutArea(pos, size);
							string str = string(ICON_FK_EXCLAMATION_CIRCLE) + "Click here to perform an unit action";
							drawlist->AddText({ pos.x - ImGui::CalcTextSize(str.c_str()).x - 10.f, pos.y + size.y * 0.5f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());

							// drawing the tips
							str = "Actions";
							ImGui::PushFont(FONT_BTN);
							drawlist->AddText({ viewport->Size.x * 0.65f,viewport->Size.y * 0.7f }, ImGui::GetColorU32({ 0.98f,0.768f,0.51f,1 }), str.c_str());
							ImGui::PopFont();

							drawlist->AddLine({ viewport->Size.x * 0.65f, viewport->Size.y * 0.7f + 20.f }, { viewport->Size.x, viewport->Size.y * 0.7f + 20.f }, ImGui::GetColorU32({ 1,1,1,1 }), 2.f);

							str = "Move this unit next to the other unit in order to get in range for an attack.";
							drawlist->AddText({ viewport->Size.x * 0.65f, viewport->Size.y * 0.7f + 25.f }, ImGui::GetColorU32({ 1,1,1,1 }), str.c_str());
						}
						break;

						case 3:
						{
							// user is making an action
						}
						break;

						default:
							break;
						}
					}

					//Tutorial Exit Button
					if (tutorial_index <= 3 && instance.tutorial_slide == false)
					{
						auto exitBtn = tex_map["Assets/Tutorial_exit.dds"];
						ImVec2 tut_min = { viewport->Size.x * 0.85f, viewport->Size.y * 0.05f };
						ImVec2 tut_max = { tut_min.x + exitBtn->GetWidth() * 0.7f, tut_min.y + exitBtn->GetHeight() * 0.7f };
						drawlist->AddImage((void*)static_cast<size_t>(exitBtn->GetID()), tut_min, tut_max);

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
					instance.tutorial_temp_exit = false;
					instance.tutorial_enable = false;
				}


				//static int selected = -1;
				//static tvector<Entity> chars(4, INVALID);
				//bool selectable_hovered = false;

				auto bannerTex = tex_map["Assets/PlayModeBanner.dds"];
				ImVec2 min_pos = viewport->WorkPos;
				ImVec2 max_pos = { viewport->WorkPos.x + bannerTex->GetWidth(),viewport->WorkPos.y + bannerTex->GetHeight() };
				ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(bannerTex->GetID()), min_pos, max_pos, { banner.get(), 0 }, { 1 + banner.get(), 1 });

				auto& cam = Service<RenderSystem>::Get().GetCamera();

				if (state != State::CINEMATIC)
				{
					cam_ctrl.controls(cam);
				}
				
				cam_ctrl.update(cam);

				glm::ivec2 world_mouse = calculate_world_mouse(cam);

				if (state != State::CINEMATIC)
				{
					ImGui::Dummy(ImVec2{ 5.0f,40.f });

					// halp (can't scroll)
					if (ImGui::BeginChild("CombatCharTurnDisplay", { turn_tex_size.x, turn_tex_size.y * 3.f + 15.f * 2.0f }, true, ImGuiWindowFlags_NoScrollbar))
					{
						bool first = true;
						float padding = 0.0f;
						ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
						for (auto id : units)
						{
							// check if turn is over
							UI::CharacterTurn(instance, id, { 0.f + menu1.get() * (padding + 400.f) * 2.f, ImGui::GetCursorPosY() + padding }, curr_entity == id);
							padding += 85.0f;
							first = false;
						}
						ImGui::PopItemFlag();
					}
					ImGui::EndChild();
				}
				
			/*	if (!instance.selected || !instance.ecs.valid(instance.selected))
				{
					state = State::NO_SELECTED;
					other_entity = INVALID;
				}*/


				switch (state)
				{
				case State::NO_SELECTED:
					no_selected(runtime, world_mouse);
					break;
				case State::MENU:
					display_selected(runtime, curr_entity);
					menu(runtime, world_mouse);
					break;
				case State::ATTACKING:
					display_selected(runtime, other_entity);
					attacking(runtime, world_mouse);
					break;
				case State::MOVING:
					display_selected(runtime, curr_entity);
					moving(runtime, world_mouse);
					break;
				case State::GLIMPSE:
					display_selected(runtime, curr_entity);
					display_selected_right(runtime, other_entity);
					glimpse(runtime);
					break;
				case State::FIGHT:
					fight(runtime);
					break;
				case State::CINEMATIC:
					show_cinematic(runtime);
					break;
				default:
					break;
				}
			}

			if (state != State::CINEMATIC)
			{
				auto add_units = tex_map["Assets/AddUnitsButton.dds"];

				ImGui::SetCursorPos(ImVec2{ viewport->Size.x * 0.9f - add_units->GetWidth() * 0.5f, viewport->Size.y * 0.06f });
				if (ImGui::ImageButton((void*)static_cast<size_t>(add_units->GetID()), ImVec2{ add_units->GetWidth() * 0.9f, add_units->GetHeight() * 0.9f }))
				{
					Service<EventManager>::Get().instant_dispatch<OpenTurnOrderOverlay>(false, units);
					Service<EventManager>::Get().instant_dispatch<CombatModeVisibility>(false);
				}
			}

			ImGui::End();

			ImGui::PopStyleVar();
			ImGui::PopStyleColor(2);

			if (!ImGui::GetHoveredID())
				HoveredID = 0;
			else if (HoveredID != ImGui::GetHoveredID())
			{
				AudioEngine ae;
				ae.Play("Sounds2D/Button_Highlight.wav", "SFX", 0.8f);
				HoveredID = ImGui::GetHoveredID();
			}
		}
	}

	void CombatModeOverlay::render_more_info([[maybe_unused]]Instance& instance, const ImGuiViewport& viewport, INFO_TYPE type, Entity entity)
	{
		const int identifier = static_cast<int>(type) + static_cast<int>(entity * 2.5f); // this is a scam
		if (ImGui::BeginChild(std::string("Char more info" + std::to_string(identifier)).c_str(), ImVec2{ viewport.Size.x * 0.3f, viewport.Size.y * 0.8f / 3.0f }, true))
		{
			{
				//auto& charac = instance.ecs.get<tc::Character>(entity);

		//ImVec2 winMin = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y };
		//ImVec2 TextMin = { ImGui::GetWindowPos().x + 10.f, ImGui::GetWindowPos().y + 5.f };
		//ImVec2 winMax = { winMin.x + ImGui::GetWindowWidth() * 0.5f, winMin.y + ImGui::GetWindowHeight() * 0.1f };
		//ImVec4 col = { 0.980f, 0.768f, 0.509f, 1.f };
		//ImVec4 textcol = { 0,0,0,1 };
		//if (!ImGui::IsWindowFocused())
		//{
		//	col = { 0.980f, 0.768f, 0.509f, 0.7f };
		//	textcol = { 0,0,0,0.7f };
		//}

		//ImGui::GetWindowDrawList()->AddRectFilled({ winMin.x, winMin.y }, { winMax.x, winMax.y }, ImGui::GetColorU32(col));
		//ImGui::PushFont(FONT_OPEN);
		//switch (type)
		//{
		//case Tempest::CombatModeOverlay::INFO_TYPE::CHAR:
		//	ImGui::GetWindowDrawList()->AddText({ TextMin.x, TextMin.y }, ImGui::GetColorU32({ 0,0,0,1 }), charac.name.c_str());
		//	break;
		//case Tempest::CombatModeOverlay::INFO_TYPE::ACTIONS:
		//	ImGui::GetWindowDrawList()->AddText({ TextMin.x, TextMin.y }, ImGui::GetColorU32({ 0,0,0,1 }), "ACTIONS");
		//	break;
		//case Tempest::CombatModeOverlay::INFO_TYPE::WEAPONS:
		//	ImGui::GetWindowDrawList()->AddText({ TextMin.x, TextMin.y }, ImGui::GetColorU32({ 0,0,0,1 }), "WEAPONS");
		//	break;
		//default:
		//	break;
		//}

		//ImGui::Dummy(ImVec2{0, 30.0f});

		//if (ImGui::BeginChild(std::string("Internal content" + std::to_string(identifier)).c_str(), ImVec2{ ImGui::GetContentRegionAvailWidth() * 0.95f, ImGui::GetContentRegionAvail().y * 0.8f }, true))
		//{
		//	ImVec2 cursor{ ImGui::GetCursorPosX() + 100.0f, ImGui::GetCursorPosY() + 30.0f };
		//	unsigned i = 0; 
		//	unsigned j = 0;
		//	switch (type)
		//	{
		//	case Tempest::CombatModeOverlay::INFO_TYPE::CHAR:
		//	{
		//		auto StatsView = instance.ecs.view<Components::Statline>(exclude_t<tc::Destroyed>());
		//		Entity StateLineId = UNDEFINED;
		//		for (auto id : StatsView)
		//			StateLineId = id;
		//		auto sl = instance.ecs.get_if<tc::Statline>(StateLineId);

		//		ImGui::Dummy(ImVec2{ 0.0f, 10.0f });
		//		ImGui::PushFont(FONT_BODY);
		//		bool NextLine = false;
		//		ImVec2 PrevPos{ 0.f ,0.f };
		//		float frontPadding = 5.f;
		//		for (auto counter = 0; counter < sl->size(); counter++)
		//		{
		//			if ((*sl)(counter))
		//			{
		//				string stat = sl->operator[](counter) + " " + std::to_string(charac.get_stat(counter));

		//				if (!NextLine)
		//				{
		//					ImGui::Dummy({ frontPadding * 15, 0 });
		//					ImGui::SameLine();
		//					PrevPos = ImGui::GetCursorPos();
		//					ImGui::Text(stat.c_str());
		//					ImGui::Dummy({ frontPadding * 15, 0 });
		//					ImGui::SameLine();
		//				}

		//				else
		//				{
		//					ImGui::SetCursorPos(PrevPos);
		//					ImGui::Dummy({ 250.f - frontPadding * 15, 0.f });
		//					ImGui::SameLine();

		//					ImGui::Text(stat.c_str());
		//					ImGui::Dummy({ 250 + frontPadding, 0 });
		//					ImGui::SameLine();

		//					ImGui::SetCursorPos(PrevPos);
		//					ImGui::Dummy({ 0, 50.f });
		//				}

		//				NextLine = !NextLine;

		//			}

		//		}
		//		ImGui::PopFont();
		//	}
		//		// display stas
		//		break;
		//	case Tempest::CombatModeOverlay::INFO_TYPE::ACTIONS:
		//		// display actions
		//		for (auto id : charac.actions)
		//		{
		//			auto& action = instance.ecs.get<tc::Graph>(id);
		//			if (UI::UIButton_2(action.g.name.c_str(), action.g.name.c_str(), { cursor.x + i++ * 190, cursor.y + j * 50 }, { 0,0 }, FONT_PARA))
		//			{
		//				// go edit page
		//			}

		//			if (i / 2)
		//			{
		//				i = 0;
		//				++j;
		//			}
		//		}
		//		break;
		//	case Tempest::CombatModeOverlay::INFO_TYPE::WEAPONS:
		//		// display weapons
		//		for (auto id : charac.weapons)
		//		{
		//			auto& weapon = instance.ecs.get<tc::Weapon>(id);
		//			if (UI::UIButton_2(weapon.name.c_str(), weapon.name.c_str(), { cursor.x + i++ * 190, cursor.y + j * 50 }, { 0,0 }, FONT_PARA))
		//			{
		//				// go edit page
		//			}

		//			if (i / 2)
		//			{
		//				i = 0;
		//				++j;
		//			}
		//		}
		//		break;
		//	default:
		//		break;
		//	}
		//}
		//ImGui::EndChild();
		//
		//ImGui::PopFont();

		//// display the edit here	
			}
		
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

	void CombatModeOverlay::pop_style_color() const
	{
		ImGui::PopStyleColor(2);
		ImGui::PopStyleVar(3);
	}

	Entity CombatModeOverlay::increase_turn()
	{
		if (++curr_turn >= units.size())
			curr_turn = 0;
		// use inter_nest[0] and inter_nest[1]
		

		return units[curr_turn];
	}

	template<typename F>
	void CombatModeOverlay::render_tabs(TABS_TYPE type, F&& func)
	{
		ImVec2 prev_cursor_pos{ 0.f, 10.f };

		if (ImGui::ImageButton(tabs[type].image_id[tabs[type].current_state], tabs[type].size))
		{
			tabs[current_tab].current_state = TabImageData::STATE::UNHOVER;
			tabs[current_tab].is_active = false;
			tabs[type].is_active = true;

			current_tab = type;
		}

		if (ImGui::IsItemHovered() || tabs[type].is_active)
			tabs[type].current_state = TabImageData::STATE::HOVER;
		else
			tabs[type].current_state = TabImageData::STATE::UNHOVER;

		if (type != TABS_TYPE::ACTION)
		{
			ImGui::SameLine();
			prev_cursor_pos = ImGui::GetCursorPos();
		}

		if (tabs[type].is_active)
		{
			func();
		}


		// Get ready for next render of tab
		ImGui::SetCursorPos(prev_cursor_pos);
	}

	void CombatModeOverlay::initialise_tabs()
	{
		tabs[TABS_TYPE::STAT].image_id[TabImageData::STATE::UNHOVER] = (void*)static_cast<size_t>(tex_map["Assets/MIStatsUnselected.dds"]->GetID());
		tabs[TABS_TYPE::STAT].image_id[TabImageData::STATE::HOVER] = (void*)static_cast<size_t>(tex_map["Assets/MIStatsSelected.dds"]->GetID());
		tabs[TABS_TYPE::STAT].size = ImVec2{ static_cast<float>(tex_map["Assets/MIStatsUnselected.dds"]->GetWidth() * 0.9f),
											 static_cast<float>(tex_map["Assets/MIStatsUnselected.dds"]->GetHeight()) * 0.9f };

		tabs[TABS_TYPE::WEAPON].image_id[TabImageData::STATE::UNHOVER] = (void*)static_cast<size_t>(tex_map["Assets/MIWeaponsUnselected.dds"]->GetID());
		tabs[TABS_TYPE::WEAPON].image_id[TabImageData::STATE::HOVER] = (void*)static_cast<size_t>(tex_map["Assets/MIWeaponsSelected.dds"]->GetID());
		tabs[TABS_TYPE::WEAPON].size = ImVec2{ static_cast<float>(tex_map["Assets/MIWeaponsUnselected.dds"]->GetWidth() * 0.9f),
											   static_cast<float>(tex_map["Assets/MIWeaponsUnselected.dds"]->GetHeight() * 0.9f) };

		tabs[TABS_TYPE::ACTION].image_id[TabImageData::STATE::UNHOVER] = (void*)static_cast<size_t>(tex_map["Assets/MIActionsUnselected.dds"]->GetID());
		tabs[TABS_TYPE::ACTION].image_id[TabImageData::STATE::HOVER] = (void*)static_cast<size_t>(tex_map["Assets/MIActionsSelected.dds"]->GetID());
		tabs[TABS_TYPE::ACTION].size = ImVec2{ static_cast<float>(tex_map["Assets/MIActionsUnselected.dds"]->GetWidth() * 0.9f),
											   static_cast<float>(tex_map["Assets/MIActionsUnselected.dds"]->GetHeight() * 0.9f) };
	}

	void CombatModeOverlay::display_unit_stats(const ImGuiViewport& viewport, Instance& instance, Components::Character* cs)
	{
		(void)viewport;
		
		if (!cs)
			return;

		auto StatsView = instance.ecs.view<Components::Statline>(exclude_t<tc::Destroyed>());
		Entity StateLineId = UNDEFINED;
		for (auto id : StatsView)
			StateLineId = id;
		auto sl = instance.ecs.get_if<tc::Statline>(StateLineId);

		ImGui::BeginChild("##UnitsInformationDisplay", { ImGui::GetContentRegionAvailWidth() * 0.85f, ImGui::GetContentRegionAvail().y * 0.85f });

		// display the character info
		float frontPadding = 5.f;
		ImGui::PushFont(FONT_BODY);
		ImGui::Dummy({ frontPadding, 0 });
		ImGui::SameLine();
		ImGui::Text("Name");

		ImGui::Dummy({ frontPadding, 0 });
		ImGui::SameLine();
		ImGui::PushItemWidth(350.f);
		ImGui::InputText("##CharacterName", &cs->name);
		bool NameDisabled = cs->name.size() > 15;
		ImGui::SameLine();
		if (NameDisabled)
		{
			ImGui::Text("15 Char only");
		}
		else
		{
			ImGui::Dummy({ 100.f, 10.f });
		}
		ImGui::Dummy({ 0, 10.f });

		bool NextLine = false;
		ImVec2 PrevPos{ 0.f ,0.f };
		for (auto i = 0; i < sl->size(); i++)
		{
			if ((*sl)(i))
			{
				string stat = sl->operator[](i) + " :";
				string label = "##" + stat;
				string WeaponData = "";

				if (!NextLine)
				{
					ImGui::Dummy({ frontPadding, 0 });
					ImGui::SameLine();
					PrevPos = ImGui::GetCursorPos();
					ImGui::Text(stat.c_str());
					ImGui::Dummy({ frontPadding, 0 });
					ImGui::SameLine();
					ImGui::PushItemWidth(100.f);
					ImGui::InputInt(label.c_str(), &cs->get_stat(i), 0);
					ImGui::PopItemWidth();
					ImGui::SameLine();
					ImGui::Text(WeaponData.c_str());
				}

				else
				{
					ImGui::SetCursorPos(PrevPos);
					ImGui::Dummy({ 250.f - frontPadding, 0.f });
					ImGui::SameLine();

					ImGui::Text(stat.c_str());
					ImGui::Dummy({ 250 + frontPadding, 0 });
					ImGui::SameLine();
					ImGui::PushItemWidth(100.f);
					ImGui::InputInt(label.c_str(), &cs->get_stat(i), 0);
					ImGui::PopItemWidth();
					ImGui::SameLine();
					ImGui::Text(WeaponData.c_str());

					ImGui::SetCursorPos(PrevPos);
					ImGui::Dummy({ 0, 70.f });
				}

				NextLine = !NextLine;

			}

		}
		ImGui::PopFont();
		ImGui::EndChild();
	}

	void CombatModeOverlay::display_weapon_stats(const ImGuiViewport& viewport, Instance& instance, Components::Character* cs)
	{
		(void)viewport;
		if (!cs)
			return;

		unsigned i = 0;
		unsigned j = 0;

		ImGui::BeginChild("##WeaponsInformationDisplay", { ImGui::GetContentRegionAvailWidth() * 0.85f, ImGui::GetContentRegionAvail().y * 0.85f });

		const ImVec2 cursor{ ImGui::GetCursorPosX() + 120.0f, ImGui::GetCursorPosY() + 30.0f };
		for (auto weap_id : cs->weapons)
		{
			if (auto weapon = instance.ecs.get_if<tc::Weapon>(weap_id))
			{
				UI::UIButton_2(weapon->name.c_str(), weapon->name.c_str(), { cursor.x + i++ * 200, cursor.y + j * 100 }, { 0, 5 }, FONT_PARA, false);

				// display in rows of 2
				if (i / 2)
				{
					i = 0;
					j++;
				}
			}
		}

		ImGui::EndChild();
	}

	void CombatModeOverlay::display_actions(const ImGuiViewport& viewport, Instance& instance, Components::Character* cs)
	{
		(void)viewport;
		if (!cs)
			return;

		unsigned i = 0;
		unsigned j = 0;

		ImGui::BeginChild("##ActionsInformationDisplay", { ImGui::GetContentRegionAvailWidth() * 0.85f, ImGui::GetContentRegionAvail().y * 0.85f });

		const ImVec2 cursor{ ImGui::GetCursorPosX() + 120.0f, ImGui::GetCursorPosY() + 30.0f };
		for (auto action_id : cs->actions)
		{
			if (auto action = instance.ecs.get_if<tc::Graph>(action_id))
			{
				UI::UIButton_2(action->g.name.c_str(), action->g.name.c_str(), { cursor.x + i++ * 200, cursor.y + j * 100 }, { 0, 5 }, FONT_PARA, false);

				// display in rows of 2
				if (i / 2)
				{
					i = 0;
					j++;
				}
			}
		}

		ImGui::EndChild();
	}
}