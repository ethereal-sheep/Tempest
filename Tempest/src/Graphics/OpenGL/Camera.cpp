#include "Camera.h"

namespace Tempest
{
	void Camera::Reset()
	{
		orientation = { 1, 0, 0, 0 };
	}

	void Camera::Update()
	{
		switch (camtype)
		{
		case CameraType::CAMERA_PERSPECTIVE:
		{
			const float f = 1.0f / tan(to_rad(fov) / 2.0f);
			const float r = to_rad(fov);
			projection = glm::perspective(r, aspect_ratio, near_clip_distance, far_clip_distance);
			reverseDepthProjection = {
				f / aspect_ratio,	0.0f,		0.0f,					0.0f,
				0.0f,				f,			0.0f,					0.0f,
				0.0f,				0.0f,		0.0f,					-1.0f,
				0.0f,				0.0f,		near_clip_distance,		0.0f
			};
			break;
		}
		case CameraType::CAMERA_ORTHOGRAPHIC:
		{
			projection = reverseDepthProjection = glm::ortho(camera_position.z * -1.0f * static_cast<float>(aspect_ratio),
				camera_position.z * 1.0f * static_cast<float>(aspect_ratio),
				camera_position.z * -1.0f,
				camera_position.z * 1.0f,
				static_cast<float>(far_clip_distance),
				static_cast<float>(near_clip_distance));
			break;
		}
		}

		ortho = glm::ortho(0.f, (float)window_width, 0.f, (float)window_height);
		view = mat4_cast(orientation);
		view = translate(view, -camera_position);
		viewProjection = projection * view;
		viewReverseDepthProjection = reverseDepthProjection * view;
	}

	void Camera::Pitch(float radians)
	{
		Rotate(radians, { 1.f, 0.f, 0.f });
	}

	void Camera::Yaw(float radians)
	{
		Rotate(radians, { 0.f, 1.f, 0.f });
	}

	void Camera::Roll(float radians)
	{
		Rotate(radians, { 0.f, 0.f, 1.f });
	}

	void Camera::Rotate(const glm::quat& rotation)
	{
		orientation = rotation * orientation;
	}

	void Camera::Rotate(float radians, const glm::vec3& axis)
	{
		const glm::quat q = angleAxis(radians, axis);
		Rotate(q);
	}

	void Camera::Turn(float radians)
	{
		const glm::quat q = angleAxis(radians, orientation * glm::vec3{ 0.f, 1.f, 0.f });
		Rotate(q);
	}

	void Camera::Move(CameraDirection dir, float amount)
	{
		switch (dir)
		{
		case CameraDirection::CAMERA_UP:
			camera_position += GetUp() * amount;
			break;

		case CameraDirection::CAMERA_DOWN:
			camera_position -= GetUp() * amount;
			break;

		case CameraDirection::CAMERA_LEFT:
			camera_position += GetLeft() * amount;
			break;

		case CameraDirection::CAMERA_RIGHT:
			camera_position -= GetLeft() * amount;
			break;

		case CameraDirection::CAMERA_FORWARD:
			camera_position += GetFront() * amount;
			break;

		case CameraDirection::CAMERA_BACK:
			camera_position -= GetFront() * amount;
			break;
		}
	}

	void Camera::SetMode(CameraType type)
	{
		camtype = type;
	}

	void Camera::SetPosition(glm::vec3 pos)
	{
		camera_position = pos;
	}

	void Camera::SetRotation(const glm::quat& rotation)
	{
		orientation = rotation;
	}

	void Camera::SetFOV(float newfov)
	{
		fov = newfov;
	}

	void Camera::SetViewport(int x, int y, int width, int height)
	{
		viewport_x = x;
		viewport_y = y;
		window_width = width;
		window_height = height;
		aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
	}

	void Camera::SetClipping(float near_clip, float far_clip)
	{
		near_clip_distance = near_clip;
		far_clip_distance = far_clip;
	}

	CameraType Camera::GetType() const
	{
		return camtype;
	}

	glm::vec4 Camera::GetViewport() const
	{
		return { viewport_x, viewport_y, window_width, window_height };
	}

	float Camera::GetFOV() const
	{
		return fov;
	}

	float Camera::GetNearClipDist() const
	{
		return near_clip_distance;
	}

	float Camera::GetFarClipDist() const
	{
		return far_clip_distance;
	}

	glm::vec3 Camera::GetPosition() const
	{
		return camera_position;
	}

	glm::vec3 Camera::GetFront() const
	{
		return conjugate(orientation) * glm::vec3{ 0.f, 0.f, -1.f };
	}

	glm::vec3 Camera::GetUp() const
	{
		return conjugate(orientation) * glm::vec3{ 0.f, 1.f, 0.f };
	}

	glm::vec3 Camera::GetLeft() const
	{
		return conjugate(orientation) * glm::vec3{ -1.f, 0.f, 0.f };
	}

	glm::quat Camera::GetQuatRotation() const
	{
		return orientation;
	}

	glm::mat4 Camera::GetViewMatrix() const
	{
		return view;
	}

	glm::mat4 Camera::GetProjectionMatrix() const
	{
		return projection;
	}

	glm::mat4 Camera::GetReverseProjectionMatrix() const
	{
		return reverseDepthProjection;
	}

	glm::mat4 Camera::GetViewProjectionMatrix() const
	{
		return viewProjection;
	}

	glm::mat4 Camera::GetReverseDepthViewProjectionMatrix() const
	{
		return viewReverseDepthProjection;
	}

	glm::mat4 Camera::GetRotationMatrix() const
	{
		return glm::mat4_cast(orientation);
	}

	glm::mat4 Camera::GetOrthoProjectionMatrix() const
	{
		return ortho;
	}

	glm::vec3 Camera::ScreenspaceToWorldspace(const glm::vec2& normalized_screenspace) const
	{
		glm::vec4 vector = { normalized_screenspace.x,
							normalized_screenspace.y,
			GetNearClipDist(),
			1
		};
		vector = glm::inverse(viewProjection) * vector;

		vector.x /= vector.w;
		vector.y /= vector.w;
		vector.z /= vector.w;

		return glm::vec3{ vector };
	}

	glm::vec2 Camera::WorldspaceToScreenspace(const glm::vec3& test) const
	{
		glm::vec4 vector = glm::vec4{ test.x, test.y, test.z, 1.0 };
		vector = viewProjection * vector;

		vector.x /= vector.w;
		vector.y /= vector.w;
		vector.z /= vector.w;

		return glm::vec2(vector.x, vector.y);
	}
}