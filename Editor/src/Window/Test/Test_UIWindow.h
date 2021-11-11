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
#include "ECS/Rttr_Register.h"

namespace Tempest
{
	class Test_UIWindow : public Window
	{
		unsigned tab = 0;
		const unsigned numOfButtons = 10;
		const float padding = 50.0f;
		const float halfPadding = padding * 0.5f;

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
				UI::UIButton_2("TEST", "TEST", { 100.f,50.f }, { 0,0 }, FONT_PARA, false);
				UI::UIButton_2("TEST", "TEST", { 300.f,50.f }, { 0,0 }, FONT_PARA, true);

				UI::UIButton_Weapon(instance, UNDEFINED, "TEST", "TEST", { 100.f,100.f }, { 0,0 }, FONT_PARA, false);
				UI::UIButton_Weapon(instance, UNDEFINED, "TEST", "TEST", { 300.f,100.f }, { 0,0 }, FONT_PARA, true);

				UI::UIButton_Action(instance, UNDEFINED, "TEST", "TEST", { 100.f,150.f }, { 0,0 }, FONT_PARA, false);
				UI::UIButton_Action(instance, UNDEFINED, "TEST", "TEST", { 300.f,150.f }, { 0,0 }, FONT_PARA, true);

				UI::UIButton_Sequence(instance, UNDEFINED, "TEST", "TEST", { 100.f,200.f }, { 0,0 }, FONT_PARA, false);
				UI::UIButton_Sequence(instance, UNDEFINED, "TEST", "TEST", { 300.f,200.f }, { 0,0 }, FONT_PARA, true);

				UI::UIButton_Simulate("TEST", "TEST", { 100.f,250.f }, { 0,0 }, FONT_PARA, false);
				UI::UIButton_Simulate("TEST", "TEST", { 300.f,250.f }, { 0,0 }, FONT_PARA, true);
				UI::UIButton_EndTurn({ 300.f,300.f }, { 0,0 }, FONT_PARA, true);


				//UI::CharacterTurn(instance, UNDEFINED, { 0.f,400.f });
				//UI::CharacterTurn(instance, UNDEFINED, { 0.f,500.f },true);
				/*UI::AttackSuccessUI("Defend",{ 300.f, 400.f }, 50);
				UI::CharacterTurnData(instance, 3, { 0.f,500.f }, false, true);
				if (UI::CharacterTurnData(instance, 3, { ImGui::GetWindowWidth(),500.f }, true, true))
				{
					LOG("CLICKED");
				}*/
				static bool selected = false;
				/*auto pos = ImGui::GetCurrentWindow()->Pos;

				auto defaultImg = tex_map["Assets/SkillUnselected.png"];
				auto selectedImg = tex_map["Assets/SkillSelected.png"];
				auto xpos = 100.f;
				if (selected)
				{
					auto diff = selectedImg->GetWidth() - defaultImg->GetWidth();
					xpos -= diff;
				}

				ImGui::SetCursorPosX(xpos);
				ImGui::SetCursorPosY(400);
				if (UI::UIActionButton("ATTACK", "##TESTID", selected))
				{
					selected = !selected;
				}
				ImGui::SetCursorPosX(100.f);
				UI::UIActionButton("TEST", "TEST", true);

				UI::ActionUI({ ImGui::GetWindowWidth(),ImGui::GetWindowHeight()*0.5f }, "SELECT AN ACTION");*/
				auto CharIcon = tex_map["Assets/CharacterIcon.png"];
				auto CharIcon2 = tex_map["Assets/Unit_Black.png"];
				//instance.ecs.get_if<tc::Character>(3);
				if (UI::UICharTurnButton((void*)static_cast<size_t>(CharIcon2->GetID()), { (float)CharIcon2->GetWidth(), (float)CharIcon2->GetHeight() }, "CHAR","##TESTAT", selected))
				{
					selected = !selected;
				}

				if (UI::UICharTurnButton((void*)static_cast<size_t>(CharIcon2->GetID()), { (float)CharIcon2->GetWidth(), (float)CharIcon2->GetHeight() }, "CHAR", "##TESTAT2", selected,true))
				{
					selected = !selected;
					std::vector<Entity> t;
					/*t.push_back(3);
					t.push_back(4);
					t.push_back(5);*/
					Service<EventManager>::Get().instant_dispatch<OpenPlaceUnitsOverlay>(t);
				}
				static vec2 a = { 100,0.f };

				//if (UI::UICharButton_Arrow((void*)static_cast<size_t>(CharIcon->GetID()), { (float)CharIcon->GetWidth() * 0.7f, (float)CharIcon->GetHeight() * 0.7f }, "Char 1", "##TESTINGCA", selected))
				//{
				//	selected = !selected;
				//}
				/*
				ImGui::BeginChild("TESTING CHILD", { 700, 500 });
				for (auto i = 0; i < 20; i++)
				{
					if (UI::UICharButton_WithDelete((void*)static_cast<size_t>(CharIcon->GetID()), { (float)CharIcon->GetWidth(), (float)CharIcon->GetHeight() }, "Char 1", "##"+std::to_string(i), selected, {0,0}, {1,1}).first)
					{
						ImGui::OpenPopup((string("TT##") + std::to_string(i)).c_str());
						selected = !selected;
						LOG("CHAR CLICKED");
					}
					UI::ConfirmDeletePopup((string("TT##") + std::to_string(i)).c_str(), "Delete this character?");
				}
				ImGui::EndChild();
				ImGui::Dummy({ 0,a.y });
				ImGui::Dummy({ a.x ,0});
				ImGui::SameLine();
				if (UI::UICharButton_WithDelete((void*)static_cast<size_t>(CharIcon->GetID()), { (float)CharIcon->GetWidth(), (float)CharIcon->GetHeight() }, "Char 1","##TESTINGCA", selected, {0,0}, {1,1}).first)
				{
					ImGui::OpenPopup("TT");
					selected = !selected;
					LOG("CHAR CLICKED");
				}*/

				UI::ConfirmDeletePopup("TT", "Delete this character?");


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