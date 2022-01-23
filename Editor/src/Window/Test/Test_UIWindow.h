/**********************************************************************************
* \author		Lim Ziyi Jean(ziyijean.lim@digipen.edu),
*				Huang Xurong(h.xurong@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include "Instance/Instance.h"
#include "Util/UIElements.h"
#include "rttr/type.h"
#include "rttr/registration.h"
#include "ECS/Rttr_Register.h"
#include "Particles/ParticleSystem_2D.h"

namespace Tempest
{
	class Test_UIWindow : public Window
	{
		unsigned tab = 0;
		const unsigned numOfButtons = 10;
		const float padding = 50.0f;
		const float halfPadding = padding * 0.5f;
		ParticleSystem_2D particleSys;

		const char* window_name() override
		{
			return "Test_UIWindow";
		}

		void init(Instance&) override
		{
			ImGuiStyle* style = &ImGui::GetStyle();

			style->FramePadding = ImVec2(8, 6);
			style->Colors[ImGuiCol_Button] = ImColor(40, 40, 40, 255);
			style->Colors[ImGuiCol_ButtonActive] = ImColor(40, 40, 40, 255);
			style->Colors[ImGuiCol_ButtonHovered] = ImColor(30, 30, 30, 255);
		}
		void RenderBaseType(const std::string& prop_name,
			rttr::type& prop_type,
			rttr::variant& value)
		{
			const auto rt_padding = 80.f;
			if (prop_type == rttr::type::get<float>())
				UI::DragFloat(prop_name.c_str(), ("##" + prop_name).c_str(), ImVec2{ rt_padding , 0.f }, &value.get_value<float>(), 0.01f);
			else if (prop_type == rttr::type::get<int>())
				UI::DragInt(prop_name.c_str(), ("##" + prop_name).c_str(), ImVec2{ rt_padding , 0.f }, &value.get_value<int>(), 1);
		}
		void RenderClassType(const std::string& prop_name,
			rttr::type& prop_type,
			rttr::variant& value)
		{
			if (prop_type == rttr::type::get<vec3>())
				UI::DragFloat3ColorBox(prop_name.c_str(), ("##" + prop_name).c_str(), ImVec2{ padding , 0.f }, glm::value_ptr(value.get_value<vec3>()), 0.f, 0.1f);
			else if (prop_type == rttr::type::get<quat>())
			{
				auto euler = glm::eulerAngles(value.get_value<quat>());
				UI::DragFloat3ColorBox(prop_name.c_str(), ("##" + prop_name).c_str(), ImVec2{ padding , 0.f }, glm::value_ptr(euler), 0.f, 0.1f);
			}
			else if (prop_type == rttr::type::get<rigidbody_config>())
			{
				using namespace rttr;
				type t = type::get_by_name("rigidbody_config");
				auto var = variant(value);

				for (auto property : prop_type.get_properties())
				{
					auto prop_t= property.get_type();
					auto prop_n = property.get_name().to_string();

					auto v = property.get_value(var);
					if (property.get_type().is_arithmetic())
						RenderBaseType(prop_n, prop_t, v);
					else if (property.get_type().is_class())
						RenderClassType(prop_n, prop_t, v);
					else
						LOG("UNKNOWN");

					property.set_value(var, v);
				}
			}

		}
		void RenderProperties(rttr::type instance, rttr::variant var)
		{
			auto instance_name = instance.get_name().to_string();
			//auto var = instance.create();
			ImGui::PushItemWidth(ImGui::GetWindowWidth() - 30.0f);
			ImGui::BeginGroup();
			if (ImGui::CollapsingHeader(instance_name.c_str(), ImGuiTreeNodeFlags_FramePadding & ImGuiTreeNodeFlags_AllowItemOverlap))
			{
				ImGui::Dummy({ 0.0f, 8.0f });
				// Loop through the properties of this object type
				for (auto property : instance.get_properties())
				{
					auto prop_type = property.get_type();
					auto prop_name = property.get_name().to_string();

					auto value = property.get_value(var);
					if (property.get_type().is_arithmetic())
						RenderBaseType(prop_name, prop_type, value);
					else if (property.get_type().is_class())
						RenderClassType(prop_name, prop_type, value);
					else
						LOG("UNKNOWN");

					property.set_value(var, value);
				}
				ImGui::Dummy({ 0.0f, 8.0f });
			}
			ImGui::EndGroup();
			ImGui::PopItemWidth();
		}

		void show(Instance& instance [[maybe_unused]] ) override
		{
			if (ImGui::Begin(window_name(), &visible, window_flags))
			{
				/*UI::UIButton_2("TEST", "TEST", { 100.f,50.f }, { 0,0 }, FONT_PARA, false);
				UI::UIButton_2("TEST", "TEST", { 300.f,50.f }, { 0,0 }, FONT_PARA, true);

				UI::UIButton_Weapon(instance, UNDEFINED, "TEST", "TEST", { 100.f,100.f }, { 0,0 }, FONT_PARA, false);
				UI::UIButton_Weapon(instance, UNDEFINED, "TEST", "TEST", { 300.f,100.f }, { 0,0 }, FONT_PARA, true);

				UI::UIButton_Action(instance, UNDEFINED, "TEST", "TEST", { 100.f,150.f }, { 0,0 }, FONT_PARA, false);
				UI::UIButton_Action(instance, UNDEFINED, "TEST", "TEST", { 300.f,150.f }, { 0,0 }, FONT_PARA, true);

				UI::UIButton_Sequence(instance, UNDEFINED, "TEST", "TEST", { 100.f,200.f }, { 0,0 }, FONT_PARA, false);
				UI::UIButton_Sequence(instance, UNDEFINED, "TEST", "TEST", { 300.f,200.f }, { 0,0 }, FONT_PARA, true);

				UI::UIButton_Simulate("TEST", "TEST", { 100.f,250.f }, { 0,0 }, FONT_PARA, false);
				UI::UIButton_Simulate("TEST", "TEST", { 300.f,250.f }, { 0,0 }, FONT_PARA, true);
				UI::UIButton_EndTurn({ 300.f,300.f }, { 0,0 }, FONT_PARA, true);*/

				
				//UI::UIMapSelectable("CONFLICT RES 1##1", "DATE CREATED",false);
				//auto& io = ImGui::GetIO();
				auto drawlist = ImGui::GetForegroundDrawList();
				
				if (ImGui::IsMouseDown(0))//ImGui::IsMouseClicked(0))
				{
					glm::vec2 tempVec;
					tempVec.x = ImGui::GetMousePos().x;
					tempVec.y = ImGui::GetMousePos().y;

					auto reg = particleSys.Register(tempVec).m_weakEmmitters.lock();
					reg->m_PA.m_type = ParticleType::Circle;
					reg->m_velocity.x = -500.0f;
				}
				if (ImGui::IsMouseDown(1))
				{
					glm::vec2 tempVec;
					tempVec.x = ImGui::GetMousePos().x;
					tempVec.y = ImGui::GetMousePos().y;

					auto reg = particleSys.Register(tempVec).m_weakEmmitters.lock();
					reg->m_PA.m_type = ParticleType::Square;
					reg->m_velocity.x = -500.0f;
					reg->m_lifeTime = 1000.0f;

					glm::vec2 wayPoint_1 = tempVec;
					wayPoint_1.x += 300;

					glm::vec2 wayPoint_2 = wayPoint_1;
					wayPoint_2.y -= 50;

					glm::vec2 wayPoint_3 = wayPoint_2;
					wayPoint_3.x -= 300;

					glm::vec2 wayPoint_4 = wayPoint_3;
					wayPoint_4.y += 50;

					reg->m_wayPoints.push_back(wayPoint_1);
					reg->m_wayPoints.push_back(wayPoint_2);
					reg->m_wayPoints.push_back(wayPoint_3);
					reg->m_wayPoints.push_back(wayPoint_4);
				}

				// Update the emitters
				for (auto& emitter : particleSys.get_emitters())
				{
					emitter->Update(0.016f);

					// Render the particle
					if(emitter->m_active)
						for (auto& particle : emitter->m_particles)
						{
							if (particle.m_isActive)
							{
								//LOG_INFO("Draw Particle");

								ImVec2 pos;
								pos.x = particle.m_position.x;
								pos.y = particle.m_position.y;

								ImVec4 colour;
								colour.x = particle.m_colour.r;
								colour.y = particle.m_colour.g;
								colour.z = particle.m_colour.b;
								colour.w = particle.m_colour.a;

								ImVec2 min, max;
								min.x = pos.x - particle.m_size * 0.5f;
								max.x = pos.x + particle.m_size * 0.5f;

								min.y = pos.y - particle.m_size * 0.5f;
								max.y = pos.y + particle.m_size * 0.5f;

								/*LOG_INFO("Draw Min X");
								LOG_INFO("Draw Max X");
								LOG_INFO(min.x);
								LOG_INFO(max.x);

								LOG_INFO("Draw Min Y");
								LOG_INFO("Draw Max Y");
								LOG_INFO(min.y);
								LOG_INFO(max.y);

								LOG_INFO("Draw Position X");
								LOG_INFO(pos.x);

								LOG_INFO("Draw Position Y");
								LOG_INFO(pos.y);*/

								if (particle.m_type == ParticleType::Circle)
									drawlist->AddCircleFilled(pos, particle.m_size, ImGui::GetColorU32({ colour }));
								else if(particle.m_type == ParticleType::Square)
									drawlist->AddRectFilled(min, max, ImGui::GetColorU32({ colour }));
							}
						}
				}
				
				if (instance.selected == INVALID)
				{
					//ImGui::Text("NO ENTITY SELECTED");
					ImGui::End(); return;
				}
				if (auto example = instance.ecs.get_if<tc::Example>(instance.selected))
				{
					using namespace rttr;
					type t = type::get_by_name(tc::Example::get_type());
					auto var = variant(example);
					RenderProperties(t, var);
				}

				if (auto transform = instance.ecs.get_if<tc::Transform>(instance.selected))
				{
					using namespace rttr;
					type t = type::get_by_name(tc::Transform::get_type());
					auto var = variant(transform);
					RenderProperties(t, var);
				}

				if (auto rb = instance.ecs.get_if<tc::Rigidbody>(instance.selected))
				{
					using namespace rttr;
					type t = type::get_by_name(tc::Rigidbody::get_type());
					auto var = variant(rb);
					RenderProperties(t, var);
				}
			}

			ImGui::End();

		}
	};
}