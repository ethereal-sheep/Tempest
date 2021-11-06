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
#include "Graphics/OpenGL/RenderSystem.h"
#include "Util/UIElements.h"
#include "rttr/type.h"
#include "rttr/registration.h"
//#include "ECS/Rttr_Register.h"

namespace Tempest
{
	class test_window3 : public Window
	{
		unsigned tab = 0;
		const unsigned numOfButtons = 10;
		const float padding = 50.0f;
		const float halfPadding = padding * 0.5f;

		const char* window_name() override
		{
			return "test_window3";
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
			const auto padding = 80.f;
			if (prop_type == rttr::type::get<float>())
				UI::DragFloat(prop_name.c_str(), ("##" + prop_name).c_str(), ImVec2{ padding , 0.f }, &value.get_value<float>(), 0.01f);
			else if (prop_type == rttr::type::get<int>())
				UI::DragInt(prop_name.c_str(), ("##" + prop_name).c_str(), ImVec2{ padding , 0.f }, &value.get_value<int>(), 1);	
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
				/*UI::UIButton_2("TEST", "TEST", { 100.f,50.f }, { 0,0 }, FONT_PARA,false);
				UI::UIButton_2("TEST", "TEST", { 300.f,50.f }, { 0,0 }, FONT_PARA,true);

				UI::UIButton_3("TEST", "TEST", { 100.f,100.f }, { 0,0 }, FONT_PARA,false);
				UI::UIButton_3("TEST", "TEST", { 300.f,100.f }, { 0,0 }, FONT_PARA,true);

				UI::UIButton_4("TEST", "TEST", { 100.f,150.f }, { 0,0 }, FONT_PARA,false);
				UI::UIButton_4("TEST", "TEST", { 300.f,150.f }, { 0,0 }, FONT_PARA,true);

				UI::UIButton_5("TEST", "TEST", { 100.f,200.f }, { 0,0 }, FONT_PARA,false);
				UI::UIButton_5("TEST", "TEST", { 300.f,200.f }, { 0,0 }, FONT_PARA,true);*/
				auto CharIcon = tex_map["Assets/CharacterIcon.png"];
				static bool selected = false;
				static vec2 a = { 100,0.f };

				ImGui::Dummy({ a.x,a.y });
				ImGui::SameLine();
				if (UI::UICharButton((void*)static_cast<size_t>(CharIcon->GetID()), { (float)CharIcon->GetWidth(), (float)CharIcon->GetHeight() },"Char 1", selected, {0,0}, {1,1}).second)
				{
					ImGui::OpenPopup("TT");
					selected = !selected;
					LOG("CHAR CLICKED");
				}
				ImGui::DragFloat2("a", glm::value_ptr(a));
				if (ImGui::Button("Weapon"))
				{
					Service<EventManager>::Get().instant_dispatch<AddingWeaponsTrigger>();
				}
				if (ImGui::Button("Action"))
				{
					Service<EventManager>::Get().instant_dispatch<AddingActionsTrigger>();
				}
				if (ImGui::Button("Unit"))
				{
					Service<EventManager>::Get().instant_dispatch<AddingUnitsTrigger>();
				}
				if (ImGui::Button("Sequence"))
				{
					Service<EventManager>::Get().instant_dispatch<SelectSequenceTrigger>();
				}
				UI::ConfirmDeletePopup("TT", "Delete this character?");
				if(selected)
					

				//ImGui::ImageButton()
				/*if (UI::UIButtonWithDelete("TEST", "TEST", { 100.f,300.f }, { 0,0 }, FONT_PARA, false).second)
				{
					LOG("BRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR");
				}*/
				/*if(ImGui::Button("T"))
				{
					using namespace rttr;
					type t = type::get_by_name("Test");
					rttr::instance var = t.create();
					auto type = var.get_type();
					for (auto prop : t.get_properties())
					{
						auto prop_type = prop.get_type();
						auto prop_name = prop.get_name().to_string();

						LOG("this is {0}", prop_name);
				}
				}*/

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