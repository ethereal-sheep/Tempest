/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "GuizmoController.h"

namespace Tempest
{
	void GuizmoController::Draw()
	{
		if (m_IsEnabled)
		{
			ImGuizmo::Enable(true);

			ImGuizmo::SetRect(m_ViewportPos.x, m_ViewportPos.y, m_ViewportExtends.x, m_ViewportExtends.y);

			//auto check = els::vec3::zero *(m_ViewMatrix* m_ProjMatrix);

			ImGuizmo::Manipulate(
				m_ViewMatrix.data(),
				m_ProjMatrix.data(),
				(ImGuizmo::OPERATION)m_Operation,
				(ImGuizmo::MODE)m_Mode,
				els::value_ptr(m_Transform),
				els::value_ptr(m_DeltaTransform)
			);


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
	void GuizmoController::SetViewportBounds(const els::vec2& pos, const els::vec2& extends)
	{
		m_ViewportPos = pos;
		m_ViewportExtends = extends;
	}
	void GuizmoController::SetTranslateRotationScale(const els::vec3& t, const els::vec3& r, const els::vec3& s)
	{
		m_CurrentPos = t;
		ImGuizmo::RecomposeMatrixFromComponents(t.data(), r.data(), s.data(), els::value_ptr(m_Transform));
	}
	void GuizmoController::SetTransformMatrix(const float* data)
	{
		m_Transform = els::make_mat4(data);
		els::vec3 t, r, s;
		ImGuizmo::DecomposeMatrixToComponents(els::value_ptr(m_Transform), t.data(), r.data(), s.data());
		m_CurrentPos = t;
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
		m_ViewMatrix = els::make_mat4(matrix);
	}
	void GuizmoController::SetProjMatrix(const float* matrix)
	{
		m_ProjMatrix = els::make_mat4(matrix);
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
	void GuizmoController::GetTranslateRotationScale(els::vec3& t, els::vec3& r, els::vec3& s)
	{
		ImGuizmo::DecomposeMatrixToComponents(m_Transform.data(), els::value_ptr(t), els::value_ptr(r), els::value_ptr(s));
	}
	void GuizmoController::GetDelta(els::vec3& t, els::vec3& r, els::vec3& s)
	{
		ImGuizmo::DecomposeMatrixToComponents(m_DeltaTransform.data(), els::value_ptr(t), els::value_ptr(r), els::value_ptr(s));
	}
}