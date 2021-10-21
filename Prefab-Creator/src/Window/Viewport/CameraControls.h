#pragma once
#include "Extern/imgui/imgui.h"

namespace Tempest::UI
{
	template<typename Camera>
	inline void world_camera_controls(Camera& cam)
	{
		ImGuiIO& io = ImGui::GetIO();
		if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) && !io.WantCaptureMouse)
		{
			auto direction = els::to_vec2(io.MouseDelta);
			auto yaw_speed = 1.f / 4.f;
			auto pitch_speed = 1.f / 4.f;
			auto pan_speed = 1.f / 4.f;
			auto forward_speed = 1.f / 4.f;
			auto scroll_speed = forward_speed * 2.f;

			if (io.MouseDown[0] || io.MouseDown[1] || io.MouseDown[2])
			{
				//ImGui::SetMouseCursor(ImGuiMouseCursor_None);
				//(m_SavedMousePos.x, m_SavedMousePos.y);
			}

			if (io.MouseDown[0] && io.MouseDown[1])
			{

			}
			else if (io.MouseDown[0]) // rotate translate
			{
				if (!els::is_zero(direction))
				{
					auto rot = cam.GetQuatRotation();
					auto yaw = glm::angleAxis(to_rad(yaw_speed * io.MouseDelta.x), glm::vec3{ 0, 1, 0 });
					rot = rot * yaw;
					cam.SetRotation(rot);

					auto currentPos = cam.GetPosition();
					auto forward = glm::normalize(glm::cross(glm::vec3{ 0, 1, 0 }, cam.GetLeft())) * io.MouseDelta.y;
					auto newPos = currentPos + forward * forward_speed;
					cam.SetPosition(newPos);
				}
			}
			else if (io.MouseDown[1]) // rotate
			{
				if (!els::is_zero(direction))
				{
					auto rot = cam.GetQuatRotation();

					auto yaw = glm::angleAxis(to_rad(yaw_speed * io.MouseDelta.x), glm::vec3{ 0, 1, 0 });
					rot = rot * yaw;
					cam.SetRotation(rot);
					auto pitch = glm::angleAxis(to_rad(pitch_speed * -io.MouseDelta.y), cam.GetLeft());
					rot = rot * pitch;
					cam.SetRotation(rot);
				}

			}
			else if (io.MouseDown[2]) // Pan
			{
				if (!els::is_zero(direction))
				{

					auto up = glm::vec3{ 0, 1, 0 };
					auto right = cam.GetLeft();

					auto currentPos = cam.GetPosition();
					auto worldSpaceDirection = glm::normalize(up * direction.y + right * direction.x);
					auto newPos = currentPos - worldSpaceDirection * pan_speed;

					cam.SetPosition(newPos);
				}
			}

			cam.SetPosition(cam.GetPosition() + cam.GetFront() * (io.MouseWheel * scroll_speed));
		}
	}


	template<typename Camera>
	inline void orbit_camera_controls(Camera& cam, vec3 pos)
	{
		ImGuiIO& io = ImGui::GetIO();

		// if not looking at orbit, look at
		

		if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) && !io.WantCaptureMouse)
		{
			auto direction = els::to_vec2(io.MouseDelta);
			auto yaw_speed = 1.f / 4.f;
			auto pitch_speed = 1.f / 4.f;
			auto pan_speed = 1.f / 4.f;
			auto forward_speed = 1.f / 4.f;
			auto scroll_speed = forward_speed * 2.f;

			if (io.MouseDown[0] || io.MouseDown[1] || io.MouseDown[2])
			{
				//ImGui::SetMouseCursor(ImGuiMouseCursor_None);
				//(m_SavedMousePos.x, m_SavedMousePos.y);
			}

			if (io.MouseDown[0] && io.MouseDown[1])
			{

			}
			else if (io.MouseDown[0]) // rotate translate
			{
				if (!els::is_zero(direction))
				{
					auto rot = cam.GetQuatRotation();
					auto yaw = glm::angleAxis(to_rad(yaw_speed * io.MouseDelta.x), glm::vec3{ 0, 1, 0 });
					rot = rot * yaw;
					cam.SetRotation(rot);

					auto currentPos = cam.GetPosition();
					auto forward = glm::normalize(glm::cross(glm::vec3{ 0, 1, 0 }, cam.GetLeft())) * io.MouseDelta.y;
					auto newPos = currentPos + forward * forward_speed;
					cam.SetPosition(newPos);
				}
			}
			else if (io.MouseDown[1]) // rotate
			{
				if (!els::is_zero(direction))
				{
					auto rot = cam.GetQuatRotation();

					auto yaw = glm::angleAxis(to_rad(yaw_speed * io.MouseDelta.x), glm::vec3{ 0, 1, 0 });
					rot = rot * yaw;
					cam.SetRotation(rot);
					auto pitch = glm::angleAxis(to_rad(pitch_speed * -io.MouseDelta.y), cam.GetLeft());
					rot = rot * pitch;
					cam.SetRotation(rot);
				}

			}
			else if (io.MouseDown[2]) // Pan
			{
				if (!els::is_zero(direction))
				{

					auto up = glm::vec3{ 0, 1, 0 };
					auto right = cam.GetLeft();

					auto currentPos = cam.GetPosition();
					auto worldSpaceDirection = glm::normalize(up * direction.y + right * direction.x);
					auto newPos = currentPos - worldSpaceDirection * pan_speed;

					cam.SetPosition(newPos);
				}
			}

			cam.SetPosition(cam.GetPosition() + cam.GetFront() * (io.MouseWheel * scroll_speed));
		}
	}

}