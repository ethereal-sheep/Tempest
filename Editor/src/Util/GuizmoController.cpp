/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "GuizmoController.h"

namespace Tempest
{
	void GuizmoController::Draw(Camera& cam, tc::Transform& transform)
	{
		if (m_IsEnabled)
		{
			Set(cam, transform);

			ImGuizmo::Enable(true);
			ImGuizmo::SetRect(m_ViewportPos.x, m_ViewportPos.y, m_ViewportExtends.x, m_ViewportExtends.y);

			//auto check = els::vec3::zero *(m_ViewMatrix* m_ProjMatrix);

			float f = 1.f;
			switch (m_Operation)
			{
			case Tempest::GuizmoController::Operation::TRANSLATE:
				f = t_snap;
				break;
			case Tempest::GuizmoController::Operation::ROTATE:
				f = r_snap;
				break;
			case Tempest::GuizmoController::Operation::SCALE:
				f = s_snap;
			default:
				break;
			}

			ImGuizmo::Manipulate(
				glm::value_ptr(m_ViewMatrix),
				glm::value_ptr(m_ProjMatrix),
				(ImGuizmo::OPERATION)m_Operation,
				(ImGuizmo::MODE)m_Mode,
				glm::value_ptr(m_Transform),
				glm::value_ptr(m_DeltaTransform),
				&f
			);

			GetDelta(m_TDelta, m_RDelta, m_SDelta);

			switch (m_State)
			{
			case Tempest::GuizmoController::State::NOT_IN_USE:
			{
				if (ImGuizmo::IsUsing())
					m_State = State::START_USE;
				break;
			}
			case Tempest::GuizmoController::State::START_USE:
				m_State = State::IN_USE;
				m_Initial = transform;
				break;
			case Tempest::GuizmoController::State::IN_USE:
			{
				if (!ImGuizmo::IsUsing())
					m_State = State::END_USE;
				break;
			}
			case Tempest::GuizmoController::State::END_USE:
				m_State = State::NOT_IN_USE;
				break;
			}
		}
	}

	void GuizmoController::Set(Camera& cam, tc::Transform& transform)
	{
		auto vp = cam.GetViewport();
		vec2 Min = { 0, 0 };
		vec2 Max = { vp.z, vp.w };

		SetViewportBounds(Min, vec2{ Max.x - Min.x, Max.y - Min.y });

		auto mat =
			glm::translate(glm::make_vec3(value_ptr(transform.position)))
			* glm::mat4(transform.rotation)
			* glm::scale(glm::make_vec3(value_ptr(transform.scale)));

		SetTransformMatrix(glm::value_ptr(mat));
		SetViewMatrix(glm::value_ptr(cam.GetViewMatrix()));
		SetProjMatrix(glm::value_ptr(cam.GetProjectionMatrix()));

	}
	void GuizmoController::SetRotateSnapping(float s)
	{
		r_snap = s;
	}

	void GuizmoController::SetTranslateSnapping(float s)
	{
		t_snap = s;
	}

	void GuizmoController::SetScaleSnapping(float s)
	{
		s_snap = s;
	}

	void GuizmoController::SetViewportBounds(const vec2& pos, const vec2& extends)
	{
		m_ViewportPos = pos;
		m_ViewportExtends = extends;
	}

	void GuizmoController::SetTranslateRotationScale(const vec3& t, const vec3& r, const vec3& s)
	{
		m_CurrentPos = t;
		ImGuizmo::RecomposeMatrixFromComponents(glm::value_ptr(t), glm::value_ptr(r), glm::value_ptr(s), glm::value_ptr(m_Transform));
	}

	void GuizmoController::SetTransformMatrix(const float* data)
	{
		m_Transform = glm::make_mat4(data);
	}

	void GuizmoController::SetOperation(Operation op)
	{
		m_Operation = op;
	}

	void GuizmoController::SetMode(Mode mode)
	{
		m_Mode = mode;
	}

	void GuizmoController::SetViewMatrix(const float* matrix)
	{
		m_ViewMatrix = glm::make_mat4(matrix);
	}

	void GuizmoController::SetProjMatrix(const float* matrix)
	{
		m_ProjMatrix = glm::make_mat4(matrix);
	}

	void GuizmoController::SetEnabled(bool b) 
	{
		m_IsEnabled = b;
	}
	void GuizmoController::Enable()
	{
		m_IsEnabled = true;
	}
	void GuizmoController::Disable()
	{
		m_IsEnabled = false;
	}
	void GuizmoController::GetTranslateRotationScale(vec3& t, vec3& r, vec3& s)
	{
		ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(m_Transform), glm::value_ptr(t), glm::value_ptr(r), glm::value_ptr(s));
	}
	void GuizmoController::GetDelta(vec3& t, vec3& r, vec3& s)
	{
		ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(m_DeltaTransform), glm::value_ptr(t), glm::value_ptr(r), glm::value_ptr(s));
	}

	vec3 GuizmoController::GetTDelta() const
	{
		return m_TDelta;
	}
	vec3 GuizmoController::GetRDelta() const
	{
		return m_RDelta;
	}
	vec3 GuizmoController::GetSDelta() const
	{
		return m_SDelta;
	}
	const tc::Transform& GuizmoController::GetInitial() const
	{
		return m_Initial;
	}
}