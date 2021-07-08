#pragma once
#include "Instance/Instance.h"
#include "imgui/imgui.h"
#include "Graphics/OpenGL/RenderSystem.h"

namespace Tempest
{
	class test_window : public Window
	{
		const char* window_name() override
		{
			return "test_window";
		}
		
		void show(Instance& instance) override
		{
			if(ImGui::Begin(window_name(), &visible, window_flags))
			{
				auto& cam = Service<RenderSystem>::Get().GetCamera();
				auto& io = ImGui::GetIO();
				glm::vec4 lRayStart_NDC(
					((float)io.MousePos.x / (float)1600 - 0.5f) * 2.0f, // [0,1024] -> [-1,1]
					((float)io.MousePos.y / (float)900 - 0.5f) * -2.0f, // [0, 768] -> [-1,1]
					-1.0, // The near plane maps to Z=-1 in Normalized Device Coordinates
					1.0f
				);
				glm::vec4 lRayEnd_NDC(
					((float)io.MousePos.x / (float)1600 - 0.5f) * 2.0f,
					((float)io.MousePos.y / (float)900 - 0.5f) * -2.0f,
					0.0,
					1.0f
				);

				glm::mat4 M = glm::inverse(cam.GetViewProjectionMatrix());
				glm::vec4 lRayStart_world = M * lRayStart_NDC; lRayStart_world /= lRayStart_world.w;
				glm::vec4 lRayEnd_world = M * lRayEnd_NDC; lRayEnd_world /= lRayEnd_world.w;
				glm::vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
				lRayDir_world = glm::normalize(lRayDir_world);

				auto start = cam.GetPosition();
				auto end = cam.GetPosition() + lRayDir_world * 1000.0f;

				ImGui::Text("Start: %.3f , %.3f,  %.3f", start.x, start.y, start.z);
				ImGui::Text("End:   %.3f , %.3f,  %.3f", end.x, end.y, end.z);

				auto [id, check] = instance.po.raycast(els::to_vec3(cam.GetPosition()), els::to_vec3(lRayDir_world));
				if (check)
					ImGui::Text("HIT! id: %u", id);
				else
					ImGui::Text("NO HIT!");

			}

			ImGui::End();
		}
	};
}