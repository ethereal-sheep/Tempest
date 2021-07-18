#include "InspectorWindow.h"
#include "Util/GuizmoController.h"

namespace Tempest
{
	void InspectorWindow::init()
	{
		window_flags |= ImGuiWindowFlags_MenuBar;
	}
	void InspectorWindow::show(Instance& instance)
	{
		// window
		if (ImGui::Begin(window_name(), &visible, window_flags))
		{
			if(instance.selected == INVALID)
			{
				ImGui::Text("NO ENTITY SELECTED");
				ImGui::End(); return;
			}

			// menu bar
			if (ImGui::BeginMenuBar())
			{
				UI::Tooltip(ICON_FA_QUESTION_CIRCLE, "Simple UI for inspecting objects. We should improve the UI once the UI/UX for it is done.", false);

				ImGui::EndMenuBar();
			}
			
			const auto padding = 80.f;
			// Meta data
			if (auto meta = instance.ecs.get_if<tc::Meta>(instance.selected))
			{
				UI::RenderText("Id:", padding);
				ImGui::Text("%u", instance.selected);

				UI::RenderText("Name:", padding);
				ImGui::InputText("##meta_name", &meta->name, ImGuiInputTextFlags_CharsNoBlank);
			}

			UI::PaddedSeparator(1.f);
			// transform data
			if (auto transform = instance.ecs.get_if<tc::Transform>(instance.selected))
			{
				auto rb = instance.ecs.get_if<tc::Rigidbody>(instance.selected);
				ImGui::PushFont(FONT_BOLD);
				bool header = ImGui::CollapsingHeader("Transform##TransformHeader", nullptr, ImGuiTreeNodeFlags_DefaultOpen);
				ImGui::PopFont();
				ImGui::Dummy({ 0.f, 1.f });

				if (header)
				{
					UI::DragFloat3ColorBox("Position", "##TransformPosDrag", ImVec2{ padding , 0.f }, transform->position.data(), 0.f, 0.1f);

					auto& GC = Service<GuizmoController>::Get();
					if (GC.GetOperation() != GuizmoController::Operation::SCALE)
					{

						ImGui::Dummy({ padding + 5.f, 0.f });
						ImGui::SameLine();

						if (ImGui::RadioButton("Local##TransfromGizmoLoc", GC.GetMode() == GuizmoController::Mode::LOCAL))
							GC.SetMode(GuizmoController::Mode::LOCAL);
						ImGui::SameLine();
						if (ImGui::RadioButton("World##TransfromGizmoWorld", GC.GetMode() == GuizmoController::Mode::WORLD))
							GC.SetMode(GuizmoController::Mode::WORLD);
					}

					{
						auto vec = glm::degrees(glm::eulerAngles(transform->rotation));
						if (UI::DragFloat3ColorBox("Rotation", "##TransformRotDrag", ImVec2{ padding , 0.f }, glm::value_ptr(vec), 0.f, 0.1f))
							transform->rotation = glm::quat(glm::radians(vec));
					}
					static bool uniformScale = false;
					rb->isDirty |= UI::UniformScaleFloat3("Scale", "##TransformScaDrag", ImVec2{ padding , 0.f }, &uniformScale, transform->scale.data(), 1.f, 1.f, 1.f, 1000.f);
				}
			}

			//RigidBody
			if (auto rb = instance.ecs.get_if<tc::Rigidbody>(instance.selected))
			{
				auto& rbConfig = rb->rb_config;
				auto transform = instance.ecs.get_if<tc::Transform>(instance.selected);
				ImGui::PushFont(FONT_BOLD);
				bool header = ImGui::CollapsingHeader("RigidBody##RigidBody", nullptr, ImGuiTreeNodeFlags_DefaultOpen);
				ImGui::PopFont();
				ImGui::Dummy({ 0.f, 1.f });
				if (header)
				{
					UI::DragFloat3ColorBox("Linear Vel", "##RbLVel", ImVec2{ padding , 0.f }, rbConfig.linear_velocity.data(), 0.f, 0.1f);
					UI::DragFloat("L_Damping", "##RbLDamping", ImVec2{ padding , 0.f }, &rbConfig.linear_damping, 0.1f, 0.f);
					UI::DragFloat3ColorBox("Angular Vel", "##RbAVel", ImVec2{ padding , 0.f }, rbConfig.angular_velocity.data(), 0.f, 0.1f);
					UI::DragFloat("A_Damping", "##RbADamping", ImVec2{ padding , 0.f }, &rbConfig.angular_damping, 0.1f, 0.f);
					UI::DragFloat("Mass", "##RbMass", ImVec2{ padding , 0.f }, &rbConfig.mass, 0.1f, 0.f);
					UI::DragFloat("Density", "##RbDensity", ImVec2{ padding , 0.f }, &rbConfig.density, 0.1f, 0.f);
					UI::Checkbox("Is_Static", "##RbStatic", ImVec2{ padding , 0.f }, &rbConfig.is_static);
					UI::Checkbox("Gravity", "##RbGravity", ImVec2{ padding , 0.f }, &rbConfig.gravity);
					UI::DragFloat3("Physics Mat", "##RbPhysMat", ImVec2{ padding , 0.f }, rbConfig.material.data(), 0.f, 0.1f);
					UI::Tooltip(ICON_FA_QUESTION_CIRCLE, "Physics Material: Static Friction , Dynamic Friction, restitution(bounciness)");
					
					//bool changed = false;
					ImGui::PushID("Collider Type");
					int collider_current = static_cast<int>(rb->shape_data.type);
					const char* Colliders[] = { "NONE", "SPHERE", "BOX", "CAPSULE" };
					ImGui::Text("ShapeT");
					ImGui::SameLine();
					ImGui::Dummy(ImVec2{ 60.f - ImGui::GetItemRectSize().x, 0.f });
					ImGui::SameLine();
					ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() - 20.f);
					if (ImGui::Combo("", &collider_current, Colliders, IM_ARRAYSIZE(Colliders)))
					{
						rb->shape_data.type = static_cast<SHAPE_TYPE>(collider_current);
						rb->isDirty = true;
					}
					ImGui::PopID();
					
					switch (rb->shape_data.type)
					{
					case SHAPE_TYPE::SPHERE:
						rb->isDirty |= UI::DragFloat("Radius", "##RbRadius", ImVec2{ padding , 0.f }, &rb->shape_data.shapeData.x, 0.1f, 0.5f);
						break;
					case SHAPE_TYPE::CAPSULE:
					{
						vec2 temp = { rb->shape_data.shapeData.x, rb->shape_data.shapeData.y };
						rb->isDirty |= UI::DragFloat2("Radius&Height", "##RbRadius&Height", ImVec2{ padding , 0.f }, temp.data(), 0.1f, 0.5f);
						rb->shape_data.shapeData.x = temp.x ;
						rb->shape_data.shapeData.y = temp.y;
						break;
					}
					break;
					case SHAPE_TYPE::BOX:
					{
						rb->isDirty |= UI::DragFloat3("X,Y,Z", "##RbRadius&Height", ImVec2{ padding , 0.f }, rb->shape_data.shapeData.data(), 0.1f, 0.5f);
					}
					break;
					case SHAPE_TYPE::NONE:
						LOG_ERROR("No shape data");
						break;
					}

					//IF Change reattached a new shape
					if (rb->isDirty)
					{
						LOG("Value Changed");

						switch (rb->shape_data.type)
						{
						case SHAPE_TYPE::SPHERE:
							rb->shape_data.shapeData.x = transform->scale.x + (rb->shape_data.shapeData.x - 1.f);
							break;
						case SHAPE_TYPE::CAPSULE:
						{
							rb->shape_data.shapeData.x = transform->scale.x + (rb->shape_data.shapeData.x - 1.f);
							rb->shape_data.shapeData.y = 0.5f * transform->scale.y + (rb->shape_data.shapeData.y - 0.5f);
							break;
						}
						break;
						case SHAPE_TYPE::BOX:
						{
							rb->shape_data.shapeData.x = (rb->shape_data.shapeData.x * transform->scale.x) - ((rb->shape_data.shapeData.x- 0.5f) * transform->scale.x);
							rb->shape_data.shapeData.y = (rb->shape_data.shapeData.x * transform->scale.y) - ((rb->shape_data.shapeData.y - 0.5f) * transform->scale.y);
							rb->shape_data.shapeData.z = (rb->shape_data.shapeData.z * transform->scale.z) - ((rb->shape_data.shapeData.z - 0.5f) * transform->scale.z);
						}
						break;
						case SHAPE_TYPE::NONE:
							LOG_ERROR("No shape data");
							break;
						}

						physx::PxShape* newShape = instance.po.CreateActorShape(rbConfig, rb->shape_data);
						physx::PxShape** curShape = nullptr;
						const physx::PxU32 numShapes = rb->internal_rb.get()->getNbShapes();
						for (physx::PxU32 i = 0; i < numShapes; i++)
						{
							physx::PxShape* CurShape = NULL;
							rb->internal_rb.get()->getShapes(&CurShape, 1, i);
							rb->internal_rb.get()->detachShape(*CurShape);
						}
						rb->internal_rb.get()->attachShape(*newShape);
						newShape->release();
						rb->isDirty = false;
					}
				}
			}
		}

		ImGui::End();
	}
}
