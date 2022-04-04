#include "RuntimeStartup.h"
#include "Graphics/Basics/RenderSystem.h"

#include "Particles/Particles_3D/EmitterSystem_3D.h"
#include <Util/shape_manip.h>

void Tempest::RuntimeStartupOverlay::init(Instance& instance)
{
	
}

void Tempest::RuntimeStartupOverlay::call_first(Instance& instance)
{
	// calculate midpoint

	vec3 total = vec3{ 0,0,0 };
	vec3 most_right = { INT_MIN, 0, 0 };
	vec3 most_up = { 0, 0, INT_MIN };
	int i = 0;

	auto& cam = Service<RenderSystem>::Get().GetCamera();

	auto view = instance.ecs.view<tc::Transform>();
	for (auto id : view)
	{
		auto& pos = instance.ecs.get<tc::Transform>(id).position;
		total += pos;

		if (pos.x > most_right.x)
			most_right = pos;
		if (pos.z > most_up.z)
			most_up = pos;

		++i;
	}

	if (i > 0)
	{
		midpt = (1.f / i) * total;


		// start from most right
		// set angle
		float angle = 20.f;
		float height = 7.5f;

		auto dist = height/glm::sin(angle);
		auto v = glm::normalize(most_right - midpt);

		auto new_pt = v * dist;
		new_pt.y = height;
		cam.SetPosition(new_pt);
		cam_ctrl.force_look_at(cam, midpt);
		start = cam.GetQuatRotation();
		if ((start * vec3{ 0,1,0 }).y < 0)
		{
			// start = glm::rotate(start, glm::pi<float>(), start * vec3{ 0,0,1 });
			//start = glm::rotate(start, glm::pi<float>(), vec3{ 0,0,1 });
			//LOG_ASSERT((start * vec3{ 0,1,0 }).y > 0);
			cam.SetRotation(start);
		}

		auto reference_pt = midpt; // start from where
		auto furthest = 1.f;
		for (auto id : view)
		{
			auto& pos = instance.ecs.get<tc::Transform>(id).position;

			furthest = glm::max(furthest, glm::distance2(reference_pt, pos));
		}
		for (auto id : view)
		{
			auto& pos = instance.ecs.get<tc::Transform>(id).position;
			auto dist2 = glm::distance2(reference_pt, pos);
			auto ratio = dist2 / furthest;
			float a = -90.f;
			float b = -200.f;

			pos.y = ratio * (b-a) + a;

		}
	}
	else
	{
		midpt = vec3{ 0,0,0 };
		cam.SetPosition(vec3{10, 7.5f, 0});
		cam_ctrl.force_look_at(cam, midpt);
		start = cam.GetQuatRotation();
	}

	inter.start(0, 1.f, 1.f, 2.5f);
	first = false;
}

void Tempest::RuntimeStartupOverlay::show(Instance& instance)
{
	if (first) call_first(instance);

	// pan for how long before stop
	const float pan_time = 5.f;
	const float how_many_rounds = .5f;

	auto& cam = Service<RenderSystem>::Get().GetCamera();
	auto dt = ImGui::GetIO().DeltaTime;
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

		for (auto id : instance.ecs.view<tc::Transform>())
		{
			auto& pos = instance.ecs.get<tc::Transform>(id).position;
			pos.y += 55.f * dt;
			if (pos.y > 0.f)
				pos.y = 0.f;
		}


		auto center = ImGui::GetMainViewport()->GetWorkCenter();
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

		ImGui::PopFont();

		cam.SetRotation(current);

		// set position
		auto inverse = glm::inverse(current) * glm::vec3(0, 0, 1) * glm::length(cam.GetPosition() - midpt);;
		cam.SetPosition(inverse + midpt);

		if ((end || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape))) && !should_play_ended)
		{
			should_play_ended = true;
			auto fn = [&]()
			{
				Service<EventManager>::Get().instant_dispatch<OpenTurnOrderOverlay>();
				should_play = false;
			};
			WipeTrigger trigger(.7f, .3f, 1.f, fn);
			trigger.force = true;
			Service<EventManager>::Get().instant_dispatch<WipeTrigger>(trigger);

			// Update the emitter
			for (auto id : instance.ecs.view<tc::Door, tc::Transform, tc::Shape>())
			{
				// Create the emitters if they do not exist
				if (m_map_interactiveEmitter_3D[id].expired())
				{
					auto& transform = instance.ecs.get<tc::Transform>(id);
					//auto& door = ecs.get<tc::Door>(id);
					auto& shape = instance.ecs.get<tc::Shape>(id);

					const int& x = shape.x;
					const int& y = shape.y;

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

					if (box.min.x > box.max.x)
						std::swap(box.min.x, box.max.x);

					if (box.min.y > box.max.y)
						std::swap(box.min.x, box.max.y);

					if (box.min.z > box.max.z)
						std::swap(box.min.z, box.max.z);

					m_map_interactiveEmitter_3D[id] = EmitterSystem_3D::GetInstance().CreateInteractiveParticle(transform.position, box.min, box.max);
				}
			}
		}
	}
}
