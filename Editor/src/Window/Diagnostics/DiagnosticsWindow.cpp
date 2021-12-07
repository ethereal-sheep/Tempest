/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "DiagnosticsWindow.h"
#include "Font.h"
#include "Events/EventManager.h"
#include "Triggers/Triggers.h"
#include "Graphics/Basics/RenderSystem.h"
#include "Audio/AudioEngine.h"


namespace Tempest
{
	void DiagnosticsWindow::init(Instance&)
	{

	}
	void DiagnosticsWindow::show(Instance& instance)
	{
		if (ImGui::Begin(window_name(), &visible, window_flags))
		{
			if (ImGui::BeginTabBar("MyTabBar", ImGuiTabBarFlags_None))
			{
				if (ImGui::BeginTabItem("Runtime"))
				{
					Runtime(instance);
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("ECS Usage"))
				{
					ECSUsage(instance);
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Memory Usage"))
				{
					MemoryUsage(instance);
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Camera"))
				{
					Camera(instance);
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Audio"))
				{
					Audio(instance);
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Textures"))
				{
					Textures(instance);
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Mouse"))
				{
					Mouse(instance);
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Light"))
				{
					Light(instance);
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
		}
		ImGui::End();

	}

	void DiagnosticsWindow::Runtime(Instance& instance)
	{
		UI::RenderText("Project Name:", 100.f); ImGui::Text(instance.get_name().c_str());
		UI::RenderText("Project Path:", 100.f); ImGui::Text(instance.get_path().string().c_str());
		ImGui::Dummy(ImVec2{ 0, 0.25f });
		ImGui::Separator();
		ImGui::Dummy(ImVec2{ 0, 0.25f });

		ImGuiIO& io = ImGui::GetIO();

		ImGui::Text("Running FPS: ");
		ImGui::SameLine();
		ImGui::Text(std::to_string(io.Framerate).c_str());
		ShowFPSGraph();

		ImGui::Dummy(ImVec2{ 0, 0.25f });
		ImGui::Separator();
		ImGui::Dummy(ImVec2{ 0, 0.25f });

		if (ImGui::Button("Trigger Error"))
			Service<EventManager>::Get().instant_dispatch<ErrorTrigger>("Error triggered by Diagnostics!");
		if (ImGui::Button("Trigger TEvent<string>"))
			Service<EventManager>::Get().instant_dispatch<TEvent<string>>("Error triggered by Diagnostics!");

	}

	void DiagnosticsWindow::ShowFPSGraph()
	{
		const int max = 144;
		const int min = 0;
		ImGuiIO& io = ImGui::GetIO();

		// dummy data
		float newValue = io.Framerate;
		auto& fps_ = Service<RenderSystem>::Get().fps;
		fps_ = newValue;

		fps_values.push_back(newValue);

		if (fps_values.size() > graphsize)
			fps_values.erase(fps_values.begin());


		const float mod = (max - min) * 0.15f;

		// hard coding the min max
		const std::string& text{
			std::to_string(max).c_str() + std::string{"\n\n\n\n\n\n"} + std::to_string(min).c_str()
		};
		ImGui::PlotLines(
			text.c_str(),
			fps_values.data(),
			static_cast<int>(fps_values.size()),
			0,
			nullptr,
			min - mod,
			max + mod,
			ImVec2(0, 100.0f));
	}

	void DiagnosticsWindow::ECSUsage(Instance& instance)
	{
		ImGui::Text("Total entities created:       %u", instance.ecs.size());
	}


	void DiagnosticsWindow::MemoryUsage(Instance& instance)
	{
		if (!instance.has_debug())
		{
			ImGui::Text("MEMORY DEBUG NOT ENABLED");
			return;
		}

		ImGui::Dummy(ImVec2(0, 0.25));

		ImGui::Text(instance.get_debug()->get_name().c_str());
		ImGui::SameLine();

		ImGui::Dummy(ImVec2(0, 0.25));
		ImGui::Separator();
		ImGui::Dummy(ImVec2(0, 0.25));

		if (ImGui::CollapsingHeader("Statistical Data", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Dummy(ImVec2(0, 0.25));
			StatisticalData(instance);
			ImGui::Dummy(ImVec2(0, 0.75));
		}
		if (ImGui::CollapsingHeader("Allocation History", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Dummy(ImVec2(0, 0.25));
			AllocationHistory(instance);
			ImGui::Dummy(ImVec2(0, 0.75));
		}
	}

	void DiagnosticsWindow::StatisticalData(Instance& instance)
	{
		auto& mr = *instance.get_debug();

		ImGui::Text("Total bytes allocated:        %u", mr.bytes_allocated());
		ImGui::Text("Total bytes deallocated:      %u", mr.bytes_deallocated());
		ImGui::Text("Highest memory usage:         %u", mr.bytes_highwater());
		ImGui::Text("Outstanding Bytes:            %u", mr.bytes_outstanding());
		ImGui::Text("Outstanding Blocks:           %u", mr.blocks_outstanding());
	}

	void DiagnosticsWindow::AllocationHistory(Instance& instance)
	{
		auto& mr = *instance.get_debug();

		ImGui::Text("Last allocated address:       %p", mr.last_allocated_address());
		ImGui::Text("Last allocated bytes:         %u", mr.last_allocated_num_bytes());
		ImGui::Text("Last allocated alignment:     %u", mr.last_allocated_alignment());

		ImGui::Dummy(ImVec2(0, 0.25));
		ImGui::Dummy(ImVec2(0, 0.25));

		ImGui::Text("Last deallocated address:     %p", mr.last_deallocated_address());
		ImGui::Text("Last deallocated bytes:       %u", mr.last_deallocated_num_bytes());
		ImGui::Text("Last deallocated alignment:   %u", mr.last_deallocated_alignment());
	}

	void DiagnosticsWindow::Camera(Instance&)
	{
		auto& cam = Service<RenderSystem>::Get().GetCamera();
		const auto padding = 80.f;
		{

			auto pos = cam.GetPosition();

			if (UI::DragFloat3ColorBox("Position", "##CameraPosDrag", ImVec2{ padding , 0.f }, value_ptr(pos), 0.f, 0.1f).first)
				cam.SetPosition(pos);
		}

		{
			auto rotation = cam.GetQuatRotation();
			auto eulerDeg = glm::degrees(glm::eulerAngles(rotation));

			if (UI::DragFloat3ColorBox("Rotation", "##CameraRotDrag", ImVec2{ padding , 0.f }, value_ptr(eulerDeg), 0.f, 1.f).first)
			{
				//cam.SetRotation(eulerDeg);
			}
		}
		{
			auto q = cam.GetQuatRotation();

			float roll = (float)atan2(2.0f * (q.y * q.z + q.w * q.x), q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z);
			float pitch = (float)asin(-2.0f * (q.x * q.z - q.w * q.y));
			float yaw = (float)atan2(2.0f * (q.x * q.y + q.w * q.z), q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z);

			ImGui::Selectable("Yaw", false, ImGuiSelectableFlags_Disabled, ImVec2{ 100.f, 0 });
			ImGui::SameLine();
			ImGui::PushID("Yaw");
			if (ImGui::DragFloat(" ", &yaw, 1.f, 0.f, 180.f)) {}
			ImGui::PopID();
			ImGui::Selectable("Pitch", false, ImGuiSelectableFlags_Disabled, ImVec2{ 100.f, 0 });
			ImGui::SameLine();
			ImGui::PushID("Pitch");
			if (ImGui::DragFloat(" ", &pitch, 1.f, 0.f, 180.f)) {}
			ImGui::PopID();
			ImGui::Selectable("Roll", false, ImGuiSelectableFlags_Disabled, ImVec2{ 100.f, 0 });
			ImGui::SameLine();
			ImGui::PushID("Roll");
			if (ImGui::DragFloat(" ", &roll, 1.f, 0.f, 180.f)) {}
			ImGui::PopID();
		}
		{


			auto up = cam.GetUp();
			auto front = cam.GetFront();
			auto left = cam.GetLeft();
			if (UI::DragFloat3ColorBox("up", "##CameraRotDrag", ImVec2{ padding , 0.f }, value_ptr(up), 0.f, 1.f).first) {}
			if (UI::DragFloat3ColorBox("front", "##CameraRotDrag", ImVec2{ padding , 0.f }, value_ptr(front), 0.f, 1.f).first) {}
			if (UI::DragFloat3ColorBox("left", "##CameraRotDrag", ImVec2{ padding , 0.f }, value_ptr(left), 0.f, 1.f).first) {}

		}
	}

	void DiagnosticsWindow::Audio(Instance& )
	{
		if (ImGui::CollapsingHeader("2D Sounds", ImGuiTreeNodeFlags_DefaultOpen))
		{
			for (auto entry : fs::directory_iterator(R"(Sounds2D/)"))
			{
				auto path = entry.path();
				auto selected = entry.path() == player.GetCurrentTrack();
				if (ImGui::Selectable(path.string().c_str(), selected))
				{
					player.SelectNewTrack(path);
				}
			}
		}
		if (ImGui::CollapsingHeader("3D Sounds", ImGuiTreeNodeFlags_DefaultOpen))
		{
			for (auto entry : fs::directory_iterator(R"(Sounds3D/)"))
			{
				auto path = entry.path();
				auto selected = entry.path() == player.GetCurrentTrack();
				if (ImGui::Selectable(path.string().c_str(), selected))
				{
					player.SelectNewTrack(path);
				}
			}
		}

		UI::PaddedSeparator(0.5f);

		ImGui::Text("Current: %s", player.GetCurrentTrack().string().c_str());

		player.Draw();

		UI::PaddedSeparator(0.5f);

		static float a = 0.f;
		ImGui::DragFloat("Test 3D Pos (x)", &a, 0.01f);
		if (ImGui::Button("Play 3D Sound"))
		{
			AudioEngine ae;
			SoundDefinition sd;

			sd.m_Pos = vec3{ a, 0.f, 0.f };
			ae.Play("Sounds3D/Hit.wav", "SFX", sd);
		}
	}

	void DiagnosticsWindow::Mouse(Instance& instance)
	{
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

			ImGui::Text("Start:       %.3f , %.3f,  %.3f", start.x, start.y, start.z);
			ImGui::Text("End:         %.3f , %.3f,  %.3f", end.x, end.y, end.z);
			ImGui::Text("MousePos:         %.3f , %.3f", io.MousePos.x, io.MousePos.y);

			float dist = 0;
			if (glm::intersectRayPlane(start, lRayDir_world, glm::vec3{}, glm::vec3{ 0,1,0 }, dist))
			{
				auto inter = cam.GetPosition() + lRayDir_world * dist;
				ImGui::Text("Intersect:   %.3f , %.3f,  %.3f", inter.x, inter.y, inter.z);
			}
			else
			{
				ImGui::Text("Not facing plane!");
			}

			//auto [id, check] = instance.po.raycast(cam.GetPosition(), lRayDir_world);
			//if (check)
			//{
			//	ImGui::Text("HIT! id: %u", id);
			//	if (ImGui::IsMouseClicked(0))
			//	{
			//		instance.selected = id;
			//	}
			//}
			//
			//else
			//	ImGui::Text("NO HIT!");

			if (instance.selected != NULL)
			{
				
				auto& test = Service<RenderSystem>::Get().m_Pipeline;
				if (test.m_ModelLibrary.size() > 0)
				{
					int count = 0;
					for (auto mdl : test.m_ModelLibrary)
					{						
						for (uint32_t j = 0; j < mdl.second->colours.size(); ++j)
						{
							std::string label = "Model" + std::to_string(count) + "Color" + std::to_string(j);
							vec3 color = mdl.second->colours[j];
							std::string id = "##Model" + std::to_string(count) + "Color" + std::to_string(j);
							UI::DragFloat3ColorBox(label.data(), id.data(), ImVec2{ 80.f , 0.f }, value_ptr(mdl.second->colours[j]), 0.f, 0.01f, 0.f, 1.f);
						}
						count++;
					}
				}
			}
			else
				ImGui::Text("None Selected!");
		}
	}

	void DiagnosticsWindow::Light(Instance& )
	{
		/// TEST
		if (ImGui::CollapsingHeader("New Graphics"))
		{
			auto& m_pos = Service<RenderSystem>::Get().modelPosition;
			const auto padding = 80.f;
			{
				UI::DragFloat3ColorBox("Position", "##ModelPosDrag", ImVec2{ padding , 0.f }, value_ptr(m_pos), 0.f, 0.1f).first;
			}

			auto& dirShadowBool = Service<RenderSystem>::Get().dirShadowBool;
			ImGui::Checkbox("Directional shadow", &dirShadowBool);
			auto& pointShadowBool = Service<RenderSystem>::Get().pointShadowBool;
			ImGui::Checkbox("Point shadow", &pointShadowBool);
			auto& pbrBool = Service<RenderSystem>::Get().pbrMode;
			ImGui::Checkbox("PBR Mode", &pbrBool);
			auto& envMapShow = Service<RenderSystem>::Get().envMapShow;
			ImGui::Checkbox("Env Map Show", &envMapShow);
			
			auto& TestPBR = Service<RenderSystem>::Get().TestPBR;
			ImGui::Checkbox("Test PBR", &TestPBR);

			auto& AAgridShow = Service<RenderSystem>::Get().AAgridShow;
			ImGui::Checkbox("AA grid Show ", &AAgridShow);

			auto& materialF0 = Service<RenderSystem>::Get().materialF0;
			UI::DragFloat3ColorBox("MaterialF0", "##MaterialF0", ImVec2{ padding , 0.f }, value_ptr(materialF0), 0.f, 0.1f).first;
			
			auto& ambientStrength = Service<RenderSystem>::Get().ambientStrength;
			ImGui::SliderFloat("Global Ambient", &ambientStrength, 0.0f, 3.0f);

			if(ImGui::TreeNode("Gbuffer Mode"))
			{
				auto& gBufferView = Service<RenderSystem>::Get().gBufferView;
				ImGui::RadioButton("Final", &gBufferView, 1);
				ImGui::RadioButton("Position", &gBufferView, 2);
				ImGui::RadioButton("Normal", &gBufferView, 3);
				ImGui::RadioButton("Color", &gBufferView, 4);
				ImGui::RadioButton("Roughness", &gBufferView, 5);
				ImGui::RadioButton("Metalness", &gBufferView, 6);
				ImGui::RadioButton("Depth", &gBufferView, 7);
				ImGui::RadioButton("SAO", &gBufferView, 8);
				ImGui::RadioButton("Velocity", &gBufferView, 9);
				ImGui::TreePop();
			}


			if (ImGui::TreeNode("Lighting"))
			{
				auto& pointMode = Service<RenderSystem>::Get().pointMode;
				auto& directionalMode = Service<RenderSystem>::Get().directionalMode;
				auto& iblMode = Service<RenderSystem>::Get().iblMode;

				if (ImGui::TreeNode("Mode"))
				{
					ImGui::Checkbox("Point", &pointMode);
					ImGui::Checkbox("Directional", &directionalMode);
					ImGui::Checkbox("Image-Based Lighting(Make sure Env Map Show is ON)", &iblMode);

					ImGui::TreePop();
				}
				

				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Point Lights"))
			{
				auto& pt_lights = Service<RenderSystem>::Get().pt_lights;

				if (ImGui::TreeNode("Variables"))
				{
					for(int pt_light_num = 0; pt_light_num < pt_lights.size(); pt_light_num++)
					{

						if (pt_lights[pt_light_num].hide)
						{
							if (ImGui::Button( (std::string("Turn on Point Light ") + std::to_string(pt_light_num)).c_str()))
							{
								pt_lights[pt_light_num].hide = !pt_lights[pt_light_num].hide;
							}

						}
						else
						{
							if (ImGui::Button((std::string("Turn off Point Light ") + std::to_string(pt_light_num)).c_str()))
							{
								pt_lights[pt_light_num].hide = !pt_lights[pt_light_num].hide;
							}
							std::string PointLightPosition = "Point Light" + std::to_string(pt_light_num);
							std::string PointLightPositionID = "##Pos" + std::to_string(pt_light_num);
							UI::DragFloat3ColorBox(PointLightPosition.data(), PointLightPositionID.data(), ImVec2{ padding , 0.f }, value_ptr(pt_lights[pt_light_num].Position), 0.f, 0.1f, -10.f, 10.f);
							
							PointLightPosition = "Point Light" + std::to_string(pt_light_num);
							PointLightPositionID = "##Color" + std::to_string(pt_light_num);
							UI::DragFloat3ColorBox(PointLightPosition.data(), PointLightPositionID.data(), ImVec2{ padding , 0.f }, value_ptr(pt_lights[pt_light_num].Color), 0.f, 0.1f, -10.f, 10.f);


							ImGui::SliderFloat((std::string("Point Light Radius ") + std::to_string(pt_light_num)).c_str(), &pt_lights[pt_light_num].radius, 0.0f, 10.0f);
						}
					}

					ImGui::TreePop();
				}

				if (ImGui::TreeNode("Attenuation"))
				{
					auto& attenuationMode = Service<RenderSystem>::Get().attenuationMode;
					ImGui::RadioButton("Quadratic", &attenuationMode, 1);
					ImGui::RadioButton("UE4", &attenuationMode, 2);

					ImGui::TreePop();
				}

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Directional Light"))
			{
				auto& dir_lights = Service<RenderSystem>::Get().dir_lights;
				
				if (ImGui::TreeNode("Direction"))
				{
					for (int dir_lights_num = 0; dir_lights_num < dir_lights.size(); dir_lights_num++)
					{
						if (!dir_lights[dir_lights_num].hide)
						{
							if (ImGui::Button((std::string("Turn off Directional Light ") + std::to_string(dir_lights_num)).c_str()))
							{
								dir_lights[dir_lights_num].hide = !dir_lights[dir_lights_num].hide;
							}
							std::string PointLightPosition = "Directional Light" + std::to_string(dir_lights_num);
							std::string PointLightPositionID = "##Direction" + std::to_string(dir_lights_num);
							UI::DragFloat3ColorBox(PointLightPosition.data(), PointLightPositionID.data(), ImVec2{ padding , 0.f }, value_ptr(dir_lights[dir_lights_num].Direction), 0.f, 0.1f, -10.f, 10.f);

						}
						else
						{
							if (ImGui::Button((std::string("Turn on Directional Light ") + std::to_string(dir_lights_num)).c_str()))
							{
								dir_lights[dir_lights_num].hide = !dir_lights[dir_lights_num].hide;
							}
						}
					}

					ImGui::TreePop();
				}

				if (ImGui::TreeNode("Color"))
				{
					for (int dir_lights_num = 0; dir_lights_num < dir_lights.size(); dir_lights_num++)
					{
						if (!dir_lights[dir_lights_num].hide)
						{
							std::string PointLightPosition = "Directional Light" + std::to_string(dir_lights_num);
							std::string PointLightPositionID = "##Color" + std::to_string(dir_lights_num);
							UI::DragFloat3ColorBox(PointLightPosition.data(), PointLightPositionID.data(), ImVec2{ padding , 0.f }, value_ptr(dir_lights[dir_lights_num].Color), 0.f, 0.1f, -10.f, 10.f);
						}
					}
					

					ImGui::TreePop();
				}

				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Environment map"))
			{
				auto& envMapHDR = Service<RenderSystem>::Get().envMapHDR;
				if (ImGui::Button("Appartment"))
				{
					envMapHDR.setTextureHDR("textures/hdr/appart.hdr", "appartHDR", true);
					Service<RenderSystem>::Get().iblSetup();
				}

				if (ImGui::Button("Pisa"))
				{
					envMapHDR.setTextureHDR("textures/hdr/pisa.hdr", "pisaHDR", true);
					Service<RenderSystem>::Get().iblSetup();
				}

				if (ImGui::Button("Canyon"))
				{
					envMapHDR.setTextureHDR("textures/hdr/canyon.hdr", "canyonHDR", true);
					Service<RenderSystem>::Get().iblSetup();
				}

				if (ImGui::Button("Loft"))
				{
					envMapHDR.setTextureHDR("textures/hdr/loft.hdr", "loftHDR", true);
					Service<RenderSystem>::Get().iblSetup();
				}

				if (ImGui::Button("Path"))
				{
					envMapHDR.setTextureHDR("textures/hdr/path.hdr", "pathHDR", true);
					Service<RenderSystem>::Get().iblSetup();
				}

				if (ImGui::Button("Circus"))
				{
					envMapHDR.setTextureHDR("textures/hdr/circus.hdr", "circusHDR", true);
					Service<RenderSystem>::Get().iblSetup();
				}

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Post processing"))
			{
				if (ImGui::TreeNode("SAO"))
				{

					auto& saoMode = Service<RenderSystem>::Get().saoMode;
					ImGui::Checkbox("Enable", &saoMode);

					auto& saoSamples = Service<RenderSystem>::Get().saoSamples;
					ImGui::SliderInt("Samples", &saoSamples, 0, 64);
					auto& saoRadius = Service<RenderSystem>::Get().saoRadius;
					ImGui::SliderFloat("Radius", &saoRadius, 0.0f, 3.0f);
					auto& saoTurns = Service<RenderSystem>::Get().saoTurns;
					ImGui::SliderInt("Turns", &saoTurns, 0, 16);
					auto& saoBias = Service<RenderSystem>::Get().saoBias;
					ImGui::SliderFloat("Bias", &saoBias, 0.0f, 0.1f);
					auto& saoScale = Service<RenderSystem>::Get().saoScale;
					ImGui::SliderFloat("Scale", &saoScale, 0.0f, 3.0f);
					auto& saoContrast = Service<RenderSystem>::Get().saoContrast;
					ImGui::SliderFloat("Contrast", &saoContrast, 0.0f, 3.0f);
					auto& saoBlurSize = Service<RenderSystem>::Get().saoBlurSize;
					ImGui::SliderInt("Blur Size", &saoBlurSize, 0, 8);

					ImGui::TreePop();
				}

				if (ImGui::TreeNode("FXAA"))
				{

					auto& fxaaMode = Service<RenderSystem>::Get().fxaaMode;
					ImGui::Checkbox("Enable", &fxaaMode);

					ImGui::TreePop();
				}

				if (ImGui::TreeNode("Motion Blur"))
				{

					auto& motionBlurMaxSamples = Service<RenderSystem>::Get().motionBlurMaxSamples;

					auto& motionBlurMode = Service<RenderSystem>::Get().motionBlurMode;
					ImGui::Checkbox("Enable", &motionBlurMode);
					ImGui::SliderInt("Max Samples", &motionBlurMaxSamples, 1, 128);

					ImGui::TreePop();
				}

				
				if (ImGui::TreeNode("Tonemapping"))
				{
					auto& tonemappingMode = Service<RenderSystem>::Get().tonemappingMode;
					ImGui::RadioButton("Reinhard", &tonemappingMode, 1);
					ImGui::RadioButton("Filmic", &tonemappingMode, 2);
					ImGui::RadioButton("Uncharted", &tonemappingMode, 3);

					ImGui::TreePop();
				}
				
				if (ImGui::TreeNode("Tilt Shift"))
				{
					auto& tiltShiftMode = Service<RenderSystem>::Get().tiltShiftMode;
					ImGui::Checkbox("Enable", &tiltShiftMode);

					ImGui::TreePop();
				}

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Object"))
			{

				auto& modelPosition = Service<RenderSystem>::Get().modelPosition;

				auto& modelRotationSpeed = Service<RenderSystem>::Get().modelRotationSpeed;

				auto& modelRotationAxis = Service<RenderSystem>::Get().modelRotationAxis;

				ImGui::SliderFloat3("Position", (float*)&modelPosition, -5.0f, 5.0f);
				ImGui::SliderFloat("Rotation Speed", &modelRotationSpeed, 0.0f, 50.0f);
				ImGui::SliderFloat3("Rotation Axis", (float*)&modelRotationAxis, 0.0f, 1.0f);

				if (ImGui::TreeNode("Model"))
				{

					auto& objectModel = Service<RenderSystem>::Get().objectModel;
					auto& modelScale = Service<RenderSystem>::Get().modelScale;
					if (ImGui::Button("Sphere"))
					{
						objectModel.~ModelPBR();
						objectModel.loadModel("models/sphere/sphere.a");
						modelScale = glm::vec3(0.5f);
					}

					if (ImGui::Button("Teapot"))
					{
						objectModel.~ModelPBR();
						objectModel.loadModel("models/teapot/teapot.a");
						modelScale = glm::vec3(0.5f);
					}

					if (ImGui::Button("Shader ball"))
					{
						objectModel.~ModelPBR();
						objectModel.loadModel("models/shaderball/shaderball.a");
						modelScale = glm::vec3(0.1f);
					}

					ImGui::TreePop();
				}
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Material"))
			{
				auto& objectAlbedo = Service<RenderSystem>::Get().objectAlbedo;
				auto& objectNormal = Service<RenderSystem>::Get().objectNormal;
				auto& objectRoughness = Service<RenderSystem>::Get().objectRoughness;
				auto& objectMetalness = Service<RenderSystem>::Get().objectMetalness;
				auto& objectAO = Service<RenderSystem>::Get().objectAO;

				if (ImGui::Button("Rusted iron"))
				{
					objectAlbedo.setTexture("textures/pbr/rustediron/rustediron_albedo.png", "ironAlbedo", true);
					objectNormal.setTexture("textures/pbr/rustediron/rustediron_normal.png", "ironNormal", true);
					objectRoughness.setTexture("textures/pbr/rustediron/rustediron_roughness.png", "ironRoughness", true);
					objectMetalness.setTexture("textures/pbr/rustediron/rustediron_metalness.png", "ironMetalness", true);
					objectAO.setTexture("textures/pbr/rustediron/rustediron_ao.png", "ironAO", true);

					materialF0 = glm::vec3(0.04f);
				}

				if (ImGui::Button("Gold"))
				{
					objectAlbedo.setTexture("textures/pbr/gold/gold_albedo.png", "goldAlbedo", true);
					objectNormal.setTexture("textures/pbr/gold/gold_normal.png", "goldNormal", true);
					objectRoughness.setTexture("textures/pbr/gold/gold_roughness.png", "goldRoughness", true);
					objectMetalness.setTexture("textures/pbr/gold/gold_metalness.png", "goldMetalness", true);
					objectAO.setTexture("textures/pbr/gold/gold_ao.png", "goldAO", true);

					materialF0 = glm::vec3(1.0f, 0.72f, 0.29f);
				}

				if (ImGui::Button("Woodfloor"))
				{
					objectAlbedo.setTexture("textures/pbr/woodfloor/woodfloor_albedo.png", "woodfloorAlbedo", true);
					objectNormal.setTexture("textures/pbr/woodfloor/woodfloor_normal.png", "woodfloorNormal", true);
					objectRoughness.setTexture("textures/pbr/woodfloor/woodfloor_roughness.png", "woodfloorRoughness", true);
					objectMetalness.setTexture("textures/pbr/woodfloor/woodfloor_metalness.png", "woodfloorMetalness", true);
					objectAO.setTexture("textures/pbr/woodfloor/woodfloor_ao.png", "woodfloorAO", true);

					materialF0 = glm::vec3(0.04f);
				}

				if (ImGui::Button("Terrazzo"))
				{
					objectAlbedo.setTexture("textures/pbr/terrazzo/terrazzo_albedo.png", "terrazzoAlbedo", true);
					objectNormal.setTexture("textures/pbr/terrazzo/terrazzo_normal.png", "terrazzoNormal", true);
					objectRoughness.setTexture("textures/pbr/terrazzo/terrazzo_roughness.png", "terrazzoRoughness", true);
					objectMetalness.setTexture("textures/pbr/terrazzo/terrazzo_metalness.png", "terrazzoMetalness", true);
					objectAO.setTexture("textures/pbr/terrazzo/terrazzo_ao.png", "terrazzoAO", true);

					materialF0 = glm::vec3(0.04f);
				}


				if (ImGui::Button("Porcelain"))
				{
					objectAlbedo.setTexture("textures/pbr/porcelain/porcelain_albedo.png", "porcelainAlbedo", true);
					objectNormal.setTexture("textures/pbr/porcelain/porcelain_normal.png", "porcelainNormal", true);
					objectRoughness.setTexture("textures/pbr/porcelain/porcelain_roughness.png", "porcelainRoughness", true);
					objectMetalness.setTexture("textures/pbr/porcelain/porcelain_metalness.png", "porcelainMetalness", true);
					objectAO.setTexture("textures/pbr/porcelain/porcelain_ao.png", "porcelainAO", true);


					materialF0 = glm::vec3(0.04f);
				}
				ImGui::TreePop();
			}
		



		}
		if (ImGui::CollapsingHeader("Old Lighting"))
		{
			auto& shininess = Service<RenderSystem>::Get().shininess;
			auto& ambientStrength = Service<RenderSystem>::Get().ambientStrength;
			auto& specularStrength = Service<RenderSystem>::Get().specularStrength;
			std::string shininessID = "Shininess";
			std::string ambientStrengthID = "Ambient Strength";
			std::string specularStrengthID = "Specular Stregth";
			ImGui::Selectable(shininessID.data(), false, ImGuiSelectableFlags_Disabled, ImVec2{ 90.f, 0 });
			ImGui::SameLine();
			ImGui::PushID(shininessID.data());
			if (ImGui::DragFloat(" ", &shininess, 1.f, 1.f, 50.f)) {}
			ImGui::PopID();
			ImGui::Selectable(ambientStrengthID.data(), false, ImGuiSelectableFlags_Disabled, ImVec2{ 90.f, 0 });
			ImGui::SameLine();
			ImGui::PushID(ambientStrengthID.data());
			if (ImGui::DragFloat(" ", &ambientStrength, 0.01f, 0.f, 1.f)) {}
			ImGui::PopID();
			ImGui::Selectable(specularStrengthID.data(), false, ImGuiSelectableFlags_Disabled, ImVec2{ 90.f, 0 });
			ImGui::SameLine();
			ImGui::PushID(specularStrengthID.data());
			if (ImGui::DragFloat(" ", &specularStrength, 0.01f, 0.f, 1.f)) {}
			ImGui::PopID();
			if (ImGui::Button("Toggle Gamma Correction"))
			{
				int gamma = Service<RenderSystem>::Get().GetGammaCorrection();
				gamma = (gamma) ? 0 : 1;
				Service<RenderSystem>::Get().SetGammaCorrection(gamma);
			}
			ImGui::Dummy(ImVec2{ 0, 0.25f });
			ImGui::Separator();
			ImGui::Dummy(ImVec2{ 0, 0.25f });
			if (Service<RenderSystem>::Get().dir_lights.size())
			{
				auto& light = Service<RenderSystem>::Get().dir_lights[0];
				if(light.hide)
				{
					if (ImGui::Button("Turn on Directional Light"))
					{
						light.hide = !light.hide;
					}
				}
				else
				{
					if (ImGui::Button("Turn off Directional Light"))
					{
						light.hide = !light.hide;
					}
					const auto padding = 80.f;
					UI::DragFloat3ColorBox("Direction", "##LightDirection", ImVec2{ padding , 0.f }, value_ptr(light.Direction), 0.f, 0.1f, -10.f, 10.f);
					UI::DragFloat3ColorBox("Color", "##LightColor", ImVec2{ padding , 0.f }, value_ptr(light.Color), 0.f, 0.01f, 0.f, 1.f);

					ImGui::Selectable("Intensity", false, ImGuiSelectableFlags_Disabled, ImVec2{ 90.f, 0 });
					ImGui::SameLine();
					ImGui::PushID("Intensity");
					if (ImGui::DragFloat(" ", &light.Intensity, 0.01f, 0.f, 1.f)) {}
					ImGui::PopID();
				}

			}
			/*ImGui::Dummy(ImVec2{ 0, 0.25f });
			ImGui::Separator();
			ImGui::Dummy(ImVec2{ 0, 0.25f });
			if (Service<RenderSystem>::Get().pt_lights.size() < 10)
			{			
				if (ImGui::Button("Add Point Light"))
				{
					int index = Service<RenderSystem>::Get().pt_lights.size();
					Service<RenderSystem>::Get().pt_lights.emplace_back(Point_Light{});
					Service<RenderSystem>::Get().pt_lights[index].Position = glm::vec3(0.5f, 0.5f, 0.5f);
				}
			}*/
			if (Service<RenderSystem>::Get().pt_lights.size())
			{
				/*if (Service<RenderSystem>::Get().pt_lights.size() > 1)
				{
					if (ImGui::Button("Remove Point Light"))
					{
						Service<RenderSystem>::Get().pt_lights.pop_back();
					}
				}*/
				const auto padding = 80.f;
				for (unsigned int i = 0; i < Service<RenderSystem>::Get().pt_lights.size(); ++i)
				{
					ImGui::Dummy(ImVec2{ 0, 0.25f });
					ImGui::Separator();
					ImGui::Dummy(ImVec2{ 0, 0.25f });
					auto& ptlight = Service<RenderSystem>::Get().pt_lights[i];
					if (ptlight.hide)
					{
						if (ImGui::Button(("Turn on Point Light" + std::to_string(i)).c_str()))
						{
							ptlight.hide = !ptlight.hide;
						}
					}
					else
					{
						if (ImGui::Button(("Turn off Point Light" + std::to_string(i)).c_str()))
						{
							ptlight.hide = !ptlight.hide;
						}
						std::string PointLightPosition = "PLightPos" + std::to_string(i);
						std::string PointLightPositionID = "##Pos" + std::to_string(i);
						std::string PointLightColor = "Color" + std::to_string(i);
						std::string PointLightColorID = "##Color" + std::to_string(i);
						std::string PointLightIntensity = "Intensity" + std::to_string(i);
						std::string PointLightConstant = "Constant" + std::to_string(i);
						std::string PointLightLinear = "Linear" + std::to_string(i);
						std::string PointLightQuadratic = "Quadratic" + std::to_string(i);
						UI::DragFloat3ColorBox(PointLightPosition.data(), PointLightPositionID.data(), ImVec2{ padding , 0.f }, value_ptr(ptlight.Position), 0.f, 0.1f, -10.f, 10.f);
						UI::DragFloat3ColorBox(PointLightColor.data(), PointLightColorID.data(), ImVec2{ padding , 0.f }, value_ptr(ptlight.Color), 0.f, 0.01f, 0.f, 1.f);
						ImGui::Selectable(PointLightIntensity.data(), false, ImGuiSelectableFlags_Disabled, ImVec2{ 90.f, 0 });
						ImGui::SameLine();
						ImGui::PushID(PointLightIntensity.data());
						if (ImGui::DragFloat(" ", &ptlight.Intensity, 0.01f, 0.f, 1.f)) {}
						ImGui::PopID();
						ImGui::Selectable(PointLightConstant.data(), false, ImGuiSelectableFlags_Disabled, ImVec2{ 90.f, 0 });
						ImGui::SameLine();
						ImGui::PushID(PointLightConstant.data());
						if (ImGui::DragFloat(" ", &ptlight.pointLightConsts, 0.01f, 0.f, 1.f)) {}
						ImGui::PopID();
						ImGui::Selectable(PointLightLinear.data(), false, ImGuiSelectableFlags_Disabled, ImVec2{ 90.f, 0 });
						ImGui::SameLine();
						ImGui::PushID(PointLightLinear.data());
						if (ImGui::DragFloat(" ", &ptlight.pointLightLinears, 0.01f, 0.f, 1.f)) {}
						ImGui::PopID();
						ImGui::Selectable(PointLightQuadratic.data(), false, ImGuiSelectableFlags_Disabled, ImVec2{ 90.f, 0 });
						ImGui::SameLine();
						ImGui::PushID(PointLightQuadratic.data());
						if (ImGui::DragFloat(" ", &ptlight.pointLightQuads, 0.01f, 0.f, 1.f)) {}
						ImGui::PopID();
					}
			
				}
			}
		}
	}

	void DiagnosticsWindow::Textures(Instance&)
	{
		if (ImGui::CollapsingHeader("Textures", ImGuiTreeNodeFlags_DefaultOpen))
		{
			for (auto entry : fs::directory_iterator(R"(Assets/)"))
			{
				auto path = entry.path();
				auto selected = curr_tex ? curr_tex->GetName() == path.string() : false;
				if (ImGui::Selectable(path.string().c_str(), selected))
				{
					curr_tex = tex_map[path];
				}
			}
		}
		
		// example
		// everything in Resources/Assets will load into this map
		// tex_map["Assets/test_photo.png"] (gets shared_ptr to texture, interface might change in future)
		// if doesn't exist, its nullptr

		UI::PaddedSeparator(0.5f);

		if(curr_tex)
			ImGui::Image((void*)static_cast<size_t>(curr_tex->GetID()), ImVec2((float)curr_tex->GetWidth(), (float)curr_tex->GetHeight()));


	}

}