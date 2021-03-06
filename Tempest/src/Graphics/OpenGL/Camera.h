/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \author		Linus Ng Hao Xuan (haoxuanlinus.ng@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include <TMath.h>


namespace Tempest
{
	enum struct CameraType
	{
		CAMERA_ORTHOGRAPHIC,
		CAMERA_PERSPECTIVE
	};

	enum struct CameraDirection
	{
		CAMERA_UP,
		CAMERA_DOWN,
		CAMERA_LEFT,
		CAMERA_RIGHT,
		CAMERA_FORWARD,
		CAMERA_BACK
	};

	class Camera
	{
	private:

		CameraType camtype = CameraType::CAMERA_PERSPECTIVE;
		int viewport_x = 0;
		int viewport_y = 0;
		int window_width = 1600;
		int window_height = 900;
		
		float aspect_ratio = static_cast<float>(window_width) / static_cast<float>(window_height);
		float fov = 60.f;
		float near_clip_distance = .1f;
		float far_clip_distance = 1000.f;

		vec3 camera_position = { 0.f, 0.f, 1.f };
		vec3 mouse_ray = { 0.f, 0.f, 0.f };
		vec3 front_ray = { 0.f, 0.f, 0.f };
		quat orientation = { 1.f, 0.f, 0.f, 0.f };
		mat4 projection = { };
		mat4 reverseDepthProjection = { };
		mat4 view = { };
		mat4 viewProjection = { };
		mat4 viewReverseDepthProjection = { };
		mat4 ortho = { };

	public:

		void Reset();
		void Update();

		void Pitch(float radians);
		void Yaw(float radians);
		void Roll(float radians);

		void Rotate(const glm::quat& rotation);
		void Rotate(float radians, const glm::vec3& axis);
		void Turn(float radians);

		void Move(CameraDirection dir, float amount = 0.1f);

		void SetMode(CameraType type);
		void SetPosition(glm::vec3 pos);
		void SetRotation(const glm::quat& rotation);
		void SetFOV(float newfov);
		void SetViewport(int x, int y, int width, int height);
		void SetClipping(float near_clip, float far_clip);
		
		void SetMousePosition(int x, int y);

		CameraType GetType() const;
		glm::vec4 GetViewport() const;

		float GetFOV() const;
		float GetNearClipDist() const;
		float GetFarClipDist() const;

		glm::vec3 GetPosition() const;
		glm::vec3 GetMouseRay() const;
		glm::vec3 GetFront() const;
		glm::vec3 GetFrontRay() const;
		glm::vec3 GetUp() const;
		glm::vec3 GetLeft() const;
		glm::quat GetQuatRotation() const;

		glm::mat4 GetViewMatrix() const;
		glm::mat4 GetProjectionMatrix() const;
		glm::mat4 GetReverseProjectionMatrix() const;
		glm::mat4 GetViewProjectionMatrix() const;
		glm::mat4 GetReverseDepthViewProjectionMatrix() const;
		glm::mat4 GetRotationMatrix() const;
		glm::mat4 GetOrthoProjectionMatrix() const;
		glm::mat4 GetInverseViewProjectionMatrix() const;
		glm::vec2 GetResolution() const;

		glm::vec3 ScreenspaceToWorldspace(const glm::vec2&) const;
		glm::vec3 MousePositionToWorldRay(int x, int y) const;
		glm::vec2 WorldspaceToScreenspace(const glm::vec3&) const;

		void OnKeyPress(uint8_t key);
	};
}