/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#include "Util/UIElements.h"

namespace Tempest
{

	enum struct CameraControlMode {
		WORLD,
		ORBIT,
		FIXED,
		FIXED_ORBIT,
	};

	enum struct EasingMode {
		LINEAR,
		INOUTSINE
	};

	struct EasingFunction {

		EasingFunction(EasingMode mode = EasingMode::LINEAR) {
			switch (mode)
			{
			case Tempest::EasingMode::LINEAR:
				ease = [](float x) -> float {
					return x;
				};
				break;
			case Tempest::EasingMode::INOUTSINE:
				ease = [](float x) -> float {
					return glm::sineEaseInOut(x);
				};
				break;
			default:
				break;
			}


		}

		float operator()(float x) const {
			return ease(x);
		}

	private:
		std::function<float(float)> ease;
	};

	class CameraControls
	{
		CameraControlMode mode;
		EasingFunction easing;

		glm::quat start_rotation{1,0,0,0};
		glm::quat end_rotation{1,0,0,0};
		glm::vec3 start_position{};
		glm::vec3 end_position{};
		glm::vec3 curr_position{};

		glm::vec3 orbit_axis{};

		float magnitude = 0.f;
		float speed = 7.f;

		float total_rot_time = 1.f;
		float current_rot_time = 1.f;
		float total_orbit_time = 1.f;
		float current_orbit_time = 1.f;
		float total_pos_time = 1.f;
		float current_pos_time = 1.f;
		float total_shake_time = 1.f;
		float current_shake_time = 1.f;



		void world_controls(Camera& cam)
		{
			ImGuiIO& io = ImGui::GetIO();
			if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) && !io.WantCaptureMouse)
			{
				auto direction = els::to_vec2(io.MouseDelta);
				auto yaw_speed = 1.f / 4;
				auto pitch_speed = 1.f / 4;
				auto pan_speed = 1.f / 4.f;
				auto forward_speed = 1.f / 4.f;
				auto scroll_speed = forward_speed * 4.f;

				auto pan_time = .05f;
				auto rotate_time = .05f;
				auto zoom_time = .05f;

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

						auto start_rot = cam.GetQuatRotation();

						// if we are moving
						/*if (current_rot_time < total_rot_time)
						{
							start_rot = end_rotation;
						}*/

						auto yaw = glm::angleAxis(glm::radians(yaw_speed * io.MouseDelta.x), glm::vec3{ 0, 1, 0 });
						auto rot = start_rot * yaw;
						auto left = glm::conjugate(rot)* glm::vec3{ -1.f, 0.f, 0.f };

						current_rot_time = 0.0f;
						total_rot_time = rotate_time;

						start_rotation = start_rot;
						end_rotation = rot;

						auto currentPos = cam.GetPosition();
						auto forward = glm::normalize(glm::cross(glm::vec3{ 0, 1, 0 }, left)) * io.MouseDelta.y;
						auto newPos = currentPos + forward * forward_speed;

						current_pos_time = 0.f;
						total_pos_time = pan_time;

						start_position = currentPos;
						end_position = newPos;

						easing = EasingMode::LINEAR;
					}
				}
				else if (io.MouseDown[1]) // rotate
				{
					if (!els::is_zero(direction))
					{
						auto start_rot = cam.GetQuatRotation();

						// if we are moving
						/*if (current_rot_time < total_rot_time)
						{
							start_rot = end_rotation;
						}*/

						auto yaw = glm::angleAxis(glm::radians(yaw_speed * io.MouseDelta.x), glm::vec3{ 0, 1, 0 });
						auto rot = end_rotation * yaw;
						auto left = glm::conjugate(rot) * glm::vec3{ -1.f, 0.f, 0.f };
						//cam.SetRotation(rot);
						auto pitch = glm::angleAxis(glm::radians(pitch_speed * -io.MouseDelta.y), left);
						rot = rot * pitch;
						//cam.SetRotation(rot);


						current_rot_time = 0.0f;
						total_rot_time = rotate_time;

						start_rotation = start_rot;
						end_rotation = rot;

						easing = EasingMode::LINEAR;
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
						auto newPos = end_position - worldSpaceDirection * pan_speed;


						current_pos_time = 0.f;
						total_pos_time = pan_time;

						start_position = currentPos;
						end_position = newPos;

						easing = EasingMode::LINEAR;
						//cam.SetPosition(newPos);
					}
				}

				else if (abs(io.MouseWheel) > 0.001f)
				{
					auto currentPos = cam.GetPosition();
					auto newPos = end_position + (cam.GetFront() * (io.MouseWheel * scroll_speed));

					current_pos_time = 0.f;
					total_pos_time = zoom_time;

					start_position = currentPos;
					end_position = newPos;

					easing = EasingMode::LINEAR;
				}
				//cam.SetPosition(cam.GetPosition() + cam.GetFront() * (io.MouseWheel * scroll_speed));
			}
		}

		void orbit_controls(Camera& cam)
		{
			ImGuiIO& io = ImGui::GetIO();
			if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) && !io.WantCaptureMouse)
			{
				auto direction = els::to_vec2(io.MouseDelta);
				auto yaw_speed = 1.f / 4;
				auto pitch_speed = 1.f / 4;
				auto forward_speed = 1.f / 4.f;
				auto scroll_speed = forward_speed * 4.f;

				auto rotate_time = .05f;
				auto zoom_time = .05f;

				if (io.MouseDown[0] || io.MouseDown[1] || io.MouseDown[2])
				{
					//ImGui::SetMouseCursor(ImGuiMouseCursor_None);
					//(m_SavedMousePos.x, m_SavedMousePos.y);
				}

				if (io.MouseDown[0] && io.MouseDown[1])
				{

				}
				else if (io.MouseDown[0]) // rotate 
				{
					if (!els::is_zero(direction))
					{
						const float max_angle = glm::radians(270.f);
						auto rel_pos = orbit_axis - cam.GetPosition();

						start_rotation = cam.GetQuatRotation();

						auto yaw = glm::angleAxis(glm::radians(yaw_speed * io.MouseDelta.x), glm::vec3{ 0, 1, 0 });
						auto rot = end_rotation * yaw;

						end_rotation = rot;
						current_orbit_time = 0.f;
						total_orbit_time = rotate_time;

						easing = EasingMode::LINEAR;
					}
				}
				else if (io.MouseDown[1]) // rotate around
				{
					if (!els::is_zero(direction))
					{
						const float max_angle = glm::radians(270.f);
						auto rel_pos = orbit_axis - cam.GetPosition();



						start_rotation = cam.GetQuatRotation();

						auto yaw = glm::angleAxis(glm::radians(yaw_speed * io.MouseDelta.x), glm::vec3{ 0, 1, 0 });
						auto rot = end_rotation * yaw;
						auto left = glm::conjugate(rot) * glm::vec3{ -1.f, 0.f, 0.f };
						auto pitch = glm::angleAxis(glm::radians(pitch_speed * -io.MouseDelta.y), left);
						rot = rot * pitch;

						end_rotation = rot;
						current_orbit_time = 0.f;
						total_orbit_time = rotate_time;

						easing = EasingMode::LINEAR;
					}

				}
				else if (io.MouseDown[2]) // Pan
				{
					if (!els::is_zero(direction))
					{
						const float max_angle = glm::radians(270.f);
						auto rel_pos = orbit_axis - cam.GetPosition();

						start_rotation = cam.GetQuatRotation();
						auto rot = end_rotation;
						auto left = glm::conjugate(rot) * glm::vec3{ -1.f, 0.f, 0.f };
						auto pitch = glm::angleAxis(glm::radians(pitch_speed * -io.MouseDelta.y), left);
						rot = end_rotation * pitch;

						end_rotation = rot;
						current_orbit_time = 0.f;
						total_orbit_time = rotate_time;

						easing = EasingMode::LINEAR;

						//cam.SetPosition(newPos);
					}
				}

				else if (abs(io.MouseWheel) > 0.001f)
				{
					auto currentPos = cam.GetPosition();
					auto front = cam.GetFront();

					auto newPos = currentPos + (front * (io.MouseWheel * scroll_speed));

					const auto max_dolly = 1.f;
					const auto max_dolly2 = max_dolly * max_dolly;
					if (glm::dot(newPos - orbit_axis, -front) < max_dolly)
					{
						newPos = orbit_axis + -front * max_dolly;
					}


					current_pos_time = 0.f;
					total_pos_time = zoom_time;

					start_position = currentPos;
					end_position = newPos;
				}
				//cam.SetPosition(cam.GetPosition() + cam.GetFront() * (io.MouseWheel * scroll_speed));
			}
		}
		
		void fixed_controls(Camera& cam)
		{
			ImGuiIO& io = ImGui::GetIO();
			if (true)
			{
				auto direction = els::to_vec2(io.MouseDelta);
				auto yaw_speed = 1.f / 4;
				//auto pitch_speed = 1.f / 4;
				// auto pan_speed = 1.f / 4.f;
				auto forward_speed = 1.f / 4.f;
				auto scroll_speed = forward_speed * 4.f;

				auto pan_time = .01f;
				[[maybe_unused]] auto other_pan_time = .05f;
				auto rotate_time = .05f;
				auto zoom_time = .05f;

				if (io.MouseDown[0] || io.MouseDown[1] || io.MouseDown[2])
				{
					//ImGui::SetMouseCursor(ImGuiMouseCursor_None);
					//(m_SavedMousePos.x, m_SavedMousePos.y);
				}

				if (io.MouseDown[0] && io.MouseDown[1])
				{

				}
				else if (io.MouseDown[0]) // nothing
				{
					
				}
				else if (io.MouseDown[1]) // pan
				{
					if (!els::is_zero(direction))
					{
						auto currentPos = cam.GetPosition();
						auto now = cam.GetMouseRay();
						auto prev = cam.MousePositionToWorldRay((int)io.MousePosPrev.x, (int)io.MousePosPrev.y);

						float prev_dist = 0, now_dist = 0;

						bool now_intersect = glm::intersectRayPlane(currentPos, now, glm::vec3{}, glm::vec3{ 0,1,0 }, now_dist);
						bool prev_intersect = glm::intersectRayPlane(currentPos, prev, glm::vec3{}, glm::vec3{ 0,1,0 }, prev_dist);

						if (now_intersect && prev_intersect)
						{
							auto now_pos = currentPos + now * now_dist;
							auto prev_pos = currentPos + prev * prev_dist;

							current_pos_time = 0.f;
							total_pos_time = pan_time;

							start_position = currentPos;
							end_position = currentPos + now_pos - prev_pos;

							easing = EasingMode::LINEAR;
						}
					}
				}
				else if (io.MouseDown[2]) // rotate around center
				{
					if (!els::is_zero(direction))
					{
						auto currentPos = cam.GetPosition();
						auto now = cam.GetFront();
						float now_dist = 0;
						bool now_intersect = glm::intersectRayPlane(currentPos, now, glm::vec3{}, glm::vec3{ 0,1,0 }, now_dist);
						auto now_pos = currentPos + now * now_dist;

						if (now_intersect)
						{
							start_rotation = cam.GetQuatRotation();
							auto yaw = glm::angleAxis(glm::radians(yaw_speed * io.MouseDelta.x), glm::vec3{ 0, 1, 0 });
							auto rot = end_rotation * yaw;

							end_rotation = rot;
							current_orbit_time = 0.f;
							total_orbit_time = rotate_time;

							orbit_axis = now_pos;

							easing = EasingMode::LINEAR;
						}

						//cam.SetPosition(newPos);
					}
				}

				else if (abs(io.MouseWheel) > 0.001f) // mouse feel
				{
					auto currentPos = cam.GetPosition();
					auto front = cam.GetFront();

					auto newPos = currentPos + (front * (io.MouseWheel * scroll_speed));

					float now_dist = 0;
					bool now_intersect = glm::intersectRayPlane(currentPos, front, glm::vec3{}, glm::vec3{ 0,1,0 }, now_dist);

					if (now_intersect)
					{
						const auto max_dolly = 1.f;
						const auto max_dolly2 = max_dolly * max_dolly;
						if (now_dist < max_dolly)
						{
							newPos = currentPos + front * now_dist + -front * max_dolly;
						}
					}


					current_pos_time = 0.f;
					total_pos_time = zoom_time;

					start_position = currentPos;
					end_position = newPos;

					easing = EasingMode::LINEAR;
				}
			}
		}

		void fixed_orbit_controls(Camera& cam)
		{
			ImGuiIO& io = ImGui::GetIO();
			if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
			{
				auto direction = els::to_vec2(io.MouseDelta);
				//auto yaw_speed = 1.f / 4;
				auto pitch_speed = 1.f / 4;
				auto forward_speed = 1.f / 4.f;
				auto scroll_speed = forward_speed * 4.f;

				auto rotate_time = .05f;
				auto zoom_time = .05f;

				if (io.MouseDown[0] || io.MouseDown[1] || io.MouseDown[2])
				{
					//ImGui::SetMouseCursor(ImGuiMouseCursor_None);
					//(m_SavedMousePos.x, m_SavedMousePos.y);
				}

				if (io.MouseDown[0] && io.MouseDown[1])
				{

				}
				else if (io.MouseDown[0]) // rotate 
				{
					if (!els::is_zero(direction))
					{
					}
				}
				else if (io.MouseDown[1]) // rotate around
				{
					if (!els::is_zero(direction))
					{
						const float max_angle = glm::radians(270.f);
						auto rel_pos = orbit_axis - cam.GetPosition();

						start_rotation = cam.GetQuatRotation();
						auto rot = end_rotation;
						auto left = glm::conjugate(rot) * glm::vec3{ -1.f, 0.f, 0.f };
						auto pitch = glm::angleAxis(glm::radians(pitch_speed * -io.MouseDelta.y), left);
						rot = end_rotation * pitch;

						end_rotation = rot;
						current_orbit_time = 0.f;
						total_orbit_time = rotate_time;

						easing = EasingMode::LINEAR;
					}

				}
				else if (io.MouseDown[2]) // Pan
				{
					if (!els::is_zero(direction))
					{

						//cam.SetPosition(newPos);
					}
				}

				else if (abs(io.MouseWheel) > 0.001f)
				{
					auto currentPos = cam.GetPosition();
					auto front = cam.GetFront();

					auto newPos = currentPos + (front * (io.MouseWheel * scroll_speed));

					const auto max_dolly = 1.f;
					const auto max_dolly2 = max_dolly * max_dolly;
					if (glm::dot(newPos - orbit_axis, -front) < max_dolly)
					{
						newPos = orbit_axis + -front * max_dolly;
					}


					current_pos_time = 0.f;
					total_pos_time = zoom_time;

					start_position = currentPos;
					end_position = newPos;
				}
				//cam.SetPosition(cam.GetPosition() + cam.GetFront() * (io.MouseWheel * scroll_speed));
			}
		}

		void update_rotation(Camera& cam)
		{
			if (current_rot_time < total_rot_time)
			{
				// slerp
				current_rot_time += ImGui::GetIO().DeltaTime;
				auto t = easing(glm::clamp(current_rot_time / total_rot_time, 0.f, 1.f));
				
				cam.SetRotation(glm::slerp(start_rotation, end_rotation, t));
			}
		}
		
		void update_position(Camera& cam)
		{
			if (current_pos_time < total_pos_time)
			{
				// lerp
				current_pos_time += ImGui::GetIO().DeltaTime;
				auto t = easing(glm::clamp(current_pos_time / total_pos_time, 0.f, 1.f));

				curr_position = glm::mix(start_position, end_position, t);

				cam.SetPosition(curr_position);
			}
		}

		void update_orbit(Camera& cam)
		{
			if (current_orbit_time < total_orbit_time)
			{
				//
				current_orbit_time += ImGui::GetIO().DeltaTime;
				auto t = easing(glm::clamp(current_orbit_time / total_orbit_time, 0.f, 1.f));

				// set rotation
				auto slerped = glm::slerp(start_rotation, end_rotation, t);
				cam.SetRotation(slerped);

				// set position
				auto inverse = glm::inverse(slerped) * glm::vec3(0,0,1) * glm::length(cam.GetPosition() - orbit_axis);;
				cam.SetPosition(inverse + orbit_axis);
			}
		}

		void update_fixed_orbit(Camera& cam)
		{
			if (current_orbit_time < total_orbit_time)
			{
				//
				current_orbit_time += ImGui::GetIO().DeltaTime;
				auto t = easing(glm::clamp(current_orbit_time / total_orbit_time, 0.f, 1.f));

				// get orbit axis based on fixed pos
				auto currentPos = cam.GetPosition();
				auto now = cam.GetMouseRay();
				float now_dist = 0;
				[[maybe_unused]] bool now_intersect = glm::intersectRayPlane(currentPos, now, glm::vec3{}, glm::vec3{ 0,1,0 }, now_dist);
				auto now_pos = currentPos + now * now_dist;

				// set rotation
				auto slerped = glm::slerp(start_rotation, end_rotation, t);
				cam.SetRotation(slerped);

				// set position
				auto inverse = glm::inverse(slerped) * glm::vec3(0, 0, 1) * glm::length(now);
				cam.SetPosition(inverse + now_pos);
			}
		}

		void update_shake(Camera& cam)
		{
			if (current_shake_time < total_shake_time)
			{
				current_shake_time += ImGui::GetIO().DeltaTime;
				auto t = glm::backEaseIn(glm::clamp(current_orbit_time / total_orbit_time, 0.f, 1.f));

				// set rotation

				// set position
				float x = els::noise::normalised_octave_noise2D(1.f, current_shake_time * speed, 4);
				float y = els::noise::normalised_octave_noise2D(10.f, current_shake_time * speed, 4);

				glm::vec3 vec = y * cam.GetUp() + x * cam.GetLeft();
				//auto mag = glm::mix(magnitude, 0.f, t);
				cam.SetPosition(curr_position + vec * magnitude);

				if (current_shake_time >= total_shake_time)
				{
					cam.SetPosition(curr_position);
				}
			}
		}

	public:
		CameraControls(CameraControlMode _mode = CameraControlMode::WORLD) : mode(_mode) {}

		void controls(Camera& cam)
		{
			switch (mode)
			{
			case Tempest::CameraControlMode::WORLD:
				world_controls(cam);
				break;
			case Tempest::CameraControlMode::ORBIT:
				orbit_controls(cam);
				break;
			case Tempest::CameraControlMode::FIXED:
				fixed_controls(cam);
				break;
			case Tempest::CameraControlMode::FIXED_ORBIT:
				fixed_orbit_controls(cam);
				break;
			default:
				break;
			}
		}

		void update(Camera& cam)
		{
			switch (mode)
			{
			case Tempest::CameraControlMode::WORLD:
				// update cam to new rotation or position
				update_rotation(cam);
				update_position(cam);
				break;
			case Tempest::CameraControlMode::ORBIT:
				update_rotation(cam);
				update_position(cam);
				update_orbit(cam);
				break;
			case Tempest::CameraControlMode::FIXED:
				update_rotation(cam);
				update_position(cam);
				update_orbit(cam);
				update_shake(cam);
				break;
			case Tempest::CameraControlMode::FIXED_ORBIT:
				update_rotation(cam);
				update_position(cam);
				update_fixed_orbit(cam);
				break;
			default:
				break;
			}
		}

		void show_debug(Camera& cam)
		{
			if(ImGui::Begin("Camera Control Debug"))
			{
				ImGui::Text("Transform");
				ImGui::Separator();
				const auto padding = ImVec2{ 80.f , 0.f };
				{
					auto pos = cam.GetPosition();

					UI::DragFloat3ColorBox("Position", "##CameraPosDrag", padding, value_ptr(pos), 0.f, 0.1f);
				}

				{
					auto rotation = cam.GetQuatRotation();
					auto eulerDeg = glm::degrees(glm::eulerAngles(rotation));

					UI::DragFloat3ColorBox("Rotation", "##CameraRotDrag", padding, value_ptr(eulerDeg), 0.f, 1.f);
				}

				ImGui::Text("Controls");
				ImGui::Separator();
				{


					bool check = mode == CameraControlMode::WORLD;
					if (check)
					{
						static glm::vec3 axis;
						UI::DragFloat3ColorBox("Orbit Axis", "##Orbit Axis", padding, value_ptr(axis), 0.f, 1.f);
						if (ImGui::Button("Use Orbit Camera"))
						{
							set_orbit_camera(cam, axis);
						}
					}
					else
					{
						if (ImGui::Button("Use World Camera"))
						{
							set_world_camera();
						}
					}
				}
				if (mode == CameraControlMode::WORLD)
				{
					if (ImGui::Button("Look at origin"))
					{
						look_at(cam, { 0,0,0 }, 0.5);
					}
				}
			}
			ImGui::End();
		}

		/*void force_look_at(Camera& cam, glm::vec3 point)
		{

		}*/


		bool is_moving() const
		{
			return current_pos_time < total_pos_time;
		}

		bool is_rotating() const
		{
			return current_rot_time < total_rot_time;
		}

		bool is_orbiting() const
		{
			return current_orbit_time < total_orbit_time;
		}

		bool is_shaking() const
		{
			return current_shake_time < total_shake_time;
		}

		bool is_any_movement() const
		{
			return is_moving() || is_rotating() || is_orbiting() || is_shaking();
		}

		bool is_still() const
		{
			return !is_any_movement();
		}

		void look_at(Camera& cam, glm::vec3 point, float time = 1.f)
		{
			auto pos = point - cam.GetPosition();
			auto front = cam.GetFront();

			glm::vec2 v1{ pos.x, pos.z };
			glm::vec2 v2{ front.x, front.z };

			auto angle = glm::orientedAngle(glm::normalize(v2), glm::normalize(v1));
			auto yaw = glm::angleAxis(angle, glm::vec3{ 0, 1, 0 });
			auto rot = yaw;


			front = glm::conjugate(rot)* glm::vec3{ 0.f, 0.f, -1.f };
			auto left = glm::conjugate(rot) * glm::vec3{ -1.f, 0.f, 0.f };

			angle = glm::orientedAngle(glm::normalize(pos), glm::normalize(front), left);

			auto pitch = glm::angleAxis(angle, left);
			rot = rot * pitch;

			start_rotation = cam.GetQuatRotation();
			end_rotation = rot;

			current_rot_time = 0.f;
			total_rot_time = time;

			easing = EasingMode::INOUTSINE;
		}

		void move(Camera& cam, glm::vec3 point, float time = 1.f)
		{
			start_position = cam.GetPosition();
			end_rotation = point;

			current_pos_time = 0.f;
			total_pos_time = time;
		}

		void set_orbit_camera(Camera& cam, glm::vec3 axis)
		{
			if (mode != CameraControlMode::ORBIT)
			{
				mode = CameraControlMode::ORBIT;
				orbit_axis = axis;
				look_at(cam, axis, 0.5);
			}
		}
		void set_fixed_orbit_camera(Camera& cam, glm::vec3 axis)
		{
			if (mode != CameraControlMode::FIXED_ORBIT)
			{
				mode = CameraControlMode::FIXED_ORBIT;
				orbit_axis = axis;
				look_at(cam, axis, 0.5);
			}
		}

		void set_fixed_camera(Camera& cam, float yaw = 0.f, float pitch = 45.f)
		{
			if (mode != CameraControlMode::FIXED)
			{
				auto rot = glm::angleAxis(glm::radians(yaw), vec3{ 0, 1 ,0 });
				auto left = glm::conjugate(rot) * vec3 { 1, 0, 0 };

				rot = rot * glm::angleAxis(glm::radians(pitch), left);

				start_rotation = cam.GetQuatRotation();
				end_rotation = rot;

				current_rot_time = 0.f;
				total_rot_time = 1.f;

				mode = CameraControlMode::FIXED;
			}
		}

		void shake(Camera& cam, float time = 1.f, float _magnitude = .5f, float sample_speed = 7.f)
		{
			current_shake_time = 0.f;
			total_shake_time = time;

			magnitude = _magnitude;
			speed = sample_speed;

			curr_position = cam.GetPosition();
		}


		void set_world_camera()
		{
			mode = CameraControlMode::WORLD;
		}

		CameraControlMode get_mode() const
		{
			return mode;
		}

		void set_orbit_axis(glm::vec3 axis)
		{
			orbit_axis = axis;
		}

		glm::vec3 get_orbit_axis() const
		{
			return orbit_axis;
		}

		void reset(Camera& cam, vec3 pos, vec3 look)
		{
			set_world_camera();

			auto ideal_pos = pos;
			auto ideal_look_at = look - ideal_pos;

			auto front = cam.GetFront();

			glm::vec2 v1{ ideal_look_at.x, ideal_look_at.z };
			glm::vec2 v2{ front.x, front.z };

			auto angle = glm::orientedAngle(glm::normalize(v2), glm::normalize(v1));
			auto yaw = glm::epsilonEqual(angle, 0.f, 0.01f) ? quat{ 1,0,0,0 } : glm::angleAxis(angle, glm::vec3{ 0, 1, 0 });
			auto rot = cam.GetQuatRotation() * yaw;

			front = glm::conjugate(rot) * glm::vec3{ 0.f, 0.f, -1.f };
			auto left = glm::conjugate(rot) * glm::vec3{ -1.f, 0.f, 0.f };

			angle = glm::orientedAngle(glm::normalize(ideal_look_at), glm::normalize(front), left);

			auto pitch = glm::epsilonEqual(angle, 0.f, 0.01f) ? quat{ 1,0,0,0 } : glm::angleAxis(angle, left);
			rot = rot * pitch;

			start_rotation = cam.GetQuatRotation();
			end_rotation = rot;

			current_rot_time = 0.f;
			total_rot_time = 1.f;

			start_position = cam.GetPosition();
			end_position = ideal_pos;

			current_pos_time = 0.f;
			total_pos_time = 1.f;


			easing = EasingMode::INOUTSINE;
		}

		void reset(Camera& cam)
		{
			reset(cam, vec3{ 7,7,-7 }, vec3{});
		}

		void preview_controls(Camera& cam)
		{
			ImGuiIO& io = ImGui::GetIO();

			auto direction = els::to_vec2(io.MouseDelta);
			auto yaw_speed = 1.f / 4;
			auto pitch_speed = 1.f / 4;
			auto forward_speed = 1.f / 4.f;
			auto scroll_speed = forward_speed * 4.f;

			auto rotate_time = .05f;
			auto zoom_time = .05f;

			if (io.MouseDown[0]) // rotate 
			{
				if (!els::is_zero(direction))
				{
					const float max_angle = glm::radians(270.f);
					auto rel_pos = orbit_axis - cam.GetPosition();

					start_rotation = cam.GetQuatRotation();

					auto yaw = glm::angleAxis(glm::radians(yaw_speed * io.MouseDelta.x), glm::vec3{ 0, 1, 0 });
					auto rot = end_rotation * yaw;

					end_rotation = rot;
					current_orbit_time = 0.f;
					total_orbit_time = rotate_time;

					easing = EasingMode::LINEAR;
				}
			}
			else if (io.MouseDown[1]) // rotate around
			{
				if (!els::is_zero(direction))
				{
					const float max_angle = glm::radians(270.f);
					auto rel_pos = orbit_axis - cam.GetPosition();



					start_rotation = cam.GetQuatRotation();

					auto yaw = glm::angleAxis(glm::radians(yaw_speed * io.MouseDelta.x), glm::vec3{ 0, 1, 0 });
					auto rot = end_rotation * yaw;
					auto left = glm::conjugate(rot) * glm::vec3{ -1.f, 0.f, 0.f };
					auto pitch = glm::angleAxis(glm::radians(pitch_speed * -io.MouseDelta.y), left);
					rot = rot * pitch;

					end_rotation = rot;
					current_orbit_time = 0.f;
					total_orbit_time = rotate_time;

					easing = EasingMode::LINEAR;
				}

			}
			else if (io.MouseDown[2]) // Pan
			{
				if (!els::is_zero(direction))
				{
					const float max_angle = glm::radians(270.f);
					auto rel_pos = orbit_axis - cam.GetPosition();

					start_rotation = cam.GetQuatRotation();
					auto rot = end_rotation;
					auto left = glm::conjugate(rot) * glm::vec3{ -1.f, 0.f, 0.f };
					auto pitch = glm::angleAxis(glm::radians(pitch_speed * -io.MouseDelta.y), left);
					rot = end_rotation * pitch;

					end_rotation = rot;
					current_orbit_time = 0.f;
					total_orbit_time = rotate_time;

					easing = EasingMode::LINEAR;

					//cam.SetPosition(newPos);
				}
			}

			else if (abs(io.MouseWheel) > 0.001f)
			{
				auto currentPos = cam.GetPosition();
				auto front = cam.GetFront();

				auto newPos = currentPos + (front * (io.MouseWheel * scroll_speed));

				const auto max_dolly = 1.f;
				const auto max_dolly2 = max_dolly * max_dolly;
				if (glm::dot(newPos - orbit_axis, -front) < max_dolly)
				{
					newPos = orbit_axis + -front * max_dolly;
				}


				current_pos_time = 0.f;
				total_pos_time = zoom_time;

				start_position = currentPos;
				end_position = newPos;
			}
		}
	};

}