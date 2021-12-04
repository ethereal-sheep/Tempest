/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include "Instance/Instance.h"
#include "Graphics/Basics/RenderSystem.h"
#include "GuizmoController.h"
#include "CameraControls.h"

namespace Tempest
{
	class ViewportWindow : public Window
	{
		CameraControls cam_ctrl;

		const char* window_name() override
		{
			return "Viewport";
		}

		void init(Instance&) override
		{
			window_flags |=
				ImGuiWindowFlags_NoCollapse |
				ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoDecoration |
				ImGuiWindowFlags_NoBringToFrontOnFocus |
				ImGuiWindowFlags_NoSavedSettings |
				ImGuiWindowFlags_NoBackground
				;

			Service<GuizmoController>::Register();
		}

		void show([[maybe_unused]]Instance& instance) override
		{
			ImGuiIO& io = ImGui::GetIO();
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");

			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::SetNextWindowDockID(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));


			if (ImGui::Begin("Viewport", nullptr, window_flags))
			{
				// DockSpace
				if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
				{
					ImGui::DockSpace(
						dockspace_id,
						ImVec2(0.0f, 0.0f),
						ImGuiDockNodeFlags_PassthruCentralNode);
				}

				auto& cam = Service<RenderSystem>::Get().GetCamera();
				cam_ctrl.show_debug(cam);
				//cam_ctrl.set_orbit_camera(cam, { 0,0,0 });
				cam_ctrl.update(cam);
			}
			ImGui::End();
			ImGui::PopStyleVar(3);


			//if (instance.selected != INVALID &&
			//	instance.ecs.has<tc::Transform>(instance.selected))
			//{

			//	ImGuizmo::SetDrawlist(ImGui::GetBackgroundDrawList());
			//	auto& GC = Service<GuizmoController>::Get();
			//	auto& transform = instance.ecs.get<tc::Transform>(instance.selected);

			//	static ImVec2 Min = { 0, 0 };
			//	static ImVec2 Max = { 1600, 900 };

			//	GC.SetViewportBounds(els::to_vec2(Min), els::vec2{ Max.x - Min.x, Max.y - Min.y });


			//	vec3 tDelta;
			//	vec3 rDelta;
			//	vec3 sDelta;

			//	auto mat =
			//		glm::translate(glm::make_vec3(value_ptr(transform.position)))
			//		* glm::mat4(transform.rotation)
			//		* glm::scale(glm::make_vec3(value_ptr(transform.scale)));

			//	//GC.SetTranslateRotationScale(transform->translation, eulerDeg, transform->scale);
			//	GC.SetTransformMatrix(glm::value_ptr(mat));
			//	GC.SetViewMatrix(glm::value_ptr(cam.GetViewMatrix()));
			//	GC.SetProjMatrix(glm::value_ptr(cam.GetProjectionMatrix()));



			//	GC.Draw();

			//	//GC.GetTranslateRotationScale(transform->translation, eulerDeg, transform->scale);

			//	GC.GetDelta(tDelta, rDelta, sDelta);

			//	if (tDelta.length2() > els::epsilon<float> ||
			//		rDelta.length2() > els::epsilon<float>)
			//	{
			//		//transform
			//		transform.position += tDelta;
			//		//transform->scale += sDelta;
			//	}
			//}




			// change this to instance cam


			// WIP for transparent child window in viewport

			//ImGuiWindowClass window_class;
			//window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_PassthruCentralNode;
			//ImGui::SetNextWindowClass(&window_class);
			/*ImGui::SetNextWindowDockID(dockspace_id);
			ImGui::SetNextWindowBgAlpha(0.f);
			if (ImGui::Begin("123", nullptr, ImGuiWindowFlags_NoBackground))
			{

				ImGuiID internal_dock = ImGui::GetID("JustForFun");
				if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
				{
					ImGui::DockSpace(
						internal_dock,
						ImVec2(0.0f, 0.0f),
						ImGuiDockNodeFlags_PassthruCentralNode |
						ImGuiDockNodeFlags_NoDockingInCentralNode);
				}


				ImGui::SetCursorPos({ 5.f,5.f });
				ImGui::Text("HELLO");

			}*/

			/*ImGui::SetNextWindowDockID(dockspace_id);

			ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
			ImGuiWindowFlags host_window_flags = 0;
			host_window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse  | ImGuiWindowFlags_NoMove;
			host_window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
			if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
				host_window_flags |= ImGuiWindowFlags_NoBackground;

			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::Begin("DockSpace Window", nullptr, host_window_flags);
			ImGui::PopStyleVar(3);

			auto* central_node = ImGui::DockBuilderGetCentralNode(dockspace_id);

			auto x = central_node->Pos.x;
			auto y = central_node->Pos.y;
			auto width = central_node->Size.x;
			auto height = central_node->Size.y;

			ImGui::SetCursorPos({ x + 5.f, y + 5.f });
			ImGui::Text("HELLO");
			ImGui::Text("HELLO");

			ImGuiID internal_id = ImGui::GetID("DockSpace");
			ImGui::DockSpace(internal_id, ImVec2(0.0f, 0.0f), dockspace_flags, nullptr);
			ImGui::End();*/




		}
	};
}