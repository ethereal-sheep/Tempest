#include "RuntimeStartup.h"
#include "Graphics/Basics/RenderSystem.h"

#include "Particles/Particles_3D/EmitterSystem_3D.h"
#include <Util/shape_manip.h>
#include "PlaceUnitCinematic.h"

void Tempest::PlaceUnitCinematic::init(Instance&)
{
	Service<EventManager>::Get().register_listener<PlayPlaceUnitCinematic>(&PlaceUnitCinematic::popup, this);
}

void Tempest::PlaceUnitCinematic::popup(const Event& e)
{
	auto& a = event_cast<PlayPlaceUnitCinematic>(e);
	
	chars = a.entities;
	show_overlay = true;
	should_play = true;

	auto& instance = a.instance;

	vec3 total = vec3{ 0,0,0 };
	vec3 most_right = { INT_MIN, 0, 0 };
	vec3 most_up = { 0, 0, INT_MIN };
	//int i = 0;

	auto& cam = Service<RenderSystem>::Get().GetCamera();

	/*auto view = instance.ecs.view<tc::Transform>();
	for (auto id : view)
	{
		auto& pos = instance.ecs.get<tc::Transform>(id).position;
		total += pos;

		if (pos.x > most_right.x)
			most_right = pos;
		if (pos.z > most_up.z)
			most_up = pos;

		++i;
	}*/

	
	for (auto id : instance.ecs.view<tc::Unit, tc::Transform>())
	{
		auto& unit = instance.ecs.get<tc::Unit>(id);
		auto rand = els::random::uniform_rand(0.f, 1.f);
		unit.drop(5.f + rand, 1.f + rand);
	}

	auto reference_pt = instance.ecs.get<tc::Transform>(chars.front()).position;

	auto p = reference_pt + vec3{ 5, 5, 0 };

	midpt = reference_pt;
	cam.SetPosition(p);
	cam_ctrl.force_look_at(cam, midpt);
	start = cam.GetQuatRotation();

	inter.start(0, 1.f, 1.f, 2.5f);
}


void Tempest::PlaceUnitCinematic::show(Instance& instance)
{
	if (show_overlay)
	{
		// pan for how long before stop
		const float pan_time = 5.f;
		const float how_many_rounds = .5f;

		auto& cam = Service<RenderSystem>::Get().GetCamera();
		auto dt = ImGui::GetIO().DeltaTime;
		dt = std::min(dt, 0.016f);;
		timer += dt;
		inter.update(dt);

		if (should_play)
		{
			float ratio = timer / pan_time;
			bool end = timer > pan_time;

			quat current = glm::rotate(start, ratio * glm::pi<float>() * 2 * how_many_rounds, vec3{ 0,1,0 });

			/*if ((current * vec3{ 0,1,0 }).y < 0)
			{
				current = glm::rotate(current, glm::pi<float>(), vec3{ 1,0,0 });
				current = glm::rotate(current, glm::pi<float>(), vec3{ 0,0,1 });
				LOG_ASSERT((start * vec3{ 0,1,0 }).y > 0);
				cam.SetRotation(current);
			}*/

			/*for (auto id : instance.ecs.view<tc::Transform>())
			{
				auto& pos = instance.ecs.get<tc::Transform>(id).position;
				pos.y += 55.f * dt;
				if (pos.y > 0.f)
					pos.y = 0.f;
			}*/


			/*auto center = ImGui::GetMainViewport()->GetWorkCenter();
			auto vp_min = ImGui::GetMainViewport()->WorkPos;
			auto vp_max = vp_min + ImGui::GetMainViewport()->WorkSize;
			auto vp_ws = ImGui::GetMainViewport()->WorkSize;
			auto extra = ImVec2{ vp_ws.x / 3.f, vp_ws.y / 5.f };
			ImGui::PushFont(FONT_CONTAX_144);
			const char* s = instance.get_current_scene_name().c_str();
			auto text_size = ImGui::CalcTextSize(s);
			auto box_min = center - text_size / 2.f * 1.2f;
			box_min.x = 0.f;
			auto box_max = center + text_size / 2.f * 1.2f;
			box_max.x = vp_max.x;

			ImGui::GetBackgroundDrawList()->AddRectFilled(box_min, box_max, IM_COL32(0x00, 0x00, 0x00, 0x70 * inter.get()), 0.f);
			ImGui::GetBackgroundDrawList()->AddText(center - text_size / 2.f, IM_COL32(0xFF, 0xFF, 0xFF, 0xFF * inter.get()), s);

			ImGui::PopFont();*/

			cam.SetRotation(current);

			// set position
			auto inverse = glm::inverse(current) * glm::vec3(0, 0, 1) * glm::length(cam.GetPosition() - midpt);;
			cam.SetPosition(inverse + midpt);

			if ((end || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape))) && !should_play_ended)
			{
				should_play_ended = true;
				auto fn = [&]()
				{

					Service<EventManager>::Get().instant_dispatch<OpenCombatModeTrigger>(instance, chars);
					should_play = false;
					show_overlay = false;


					//auto& cam = Service<RenderSystem>::Get().GetCamera();
					//auto old_cam = cam.GetPosition();
					//old_cam.y = 5.f;
					//cam.SetPosition(old_cam);
					////cam_ctrl.set_fixed_camera(cam, 0, 70);

					//float yaw = 0;
					//float pitch = 70.f;
					//auto point = midpt;

					//auto rot = glm::angleAxis(glm::radians(yaw), vec3{ 0, 1 ,0 });
					//auto left = glm::conjugate(rot) * vec3 { 1, 0, 0 };

					//rot = rot * glm::angleAxis(glm::radians(pitch), left);
					//cam.SetRotation(rot);

					//auto currentPos = cam.GetPosition();
					//auto now = cam.GetFrontRay();

					//float now_dist = 0.f;
					//bool now_intersect = glm::intersectRayPlane(currentPos, now, glm::vec3{}, glm::vec3{ 0,1,0 }, now_dist);

					//if (now_intersect)
					//{
					//	auto now_pos = currentPos + now * now_dist;
					//	auto v = point - now_pos;

					//	cam.SetPosition(currentPos + v);
					//}

				};
				WipeTrigger trigger(.5f, .5f, 0.f, fn);
				trigger.force = true;
				Service<EventManager>::Get().instant_dispatch<WipeTrigger>(trigger);

			}
		}
	}
	
}
