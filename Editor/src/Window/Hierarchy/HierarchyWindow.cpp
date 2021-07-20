#include "HierarchyWindow.h"
#include "Events/EventManager.h"

namespace Tempest
{
	void HierarchyWindow::init()
	{
		window_flags |= ImGuiWindowFlags_MenuBar;
	}

	void HierarchyWindow::show(Instance& instance)
	{
		const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
		const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();


		// window
		if (ImGui::Begin(window_name(), &visible, window_flags))			
		{
			// menu bar
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu(ICON_FA_PLUS))
				{
					if (ImGui::MenuItem("Add Cube"))
					{
						// we can do factories for entities here
						auto entity = instance.ecs.create();
						auto meta = instance.ecs.emplace<tc::Meta>(entity);
						meta->name = "Cube";

						auto transform = instance.ecs.emplace<tc::Transform>(entity);
						auto rb = instance.ecs.emplace<tc::Rigidbody>(entity);
						instance.ecs.emplace<tc::Mesh>(entity, MeshCode::CUBE);
		
						rb->shape_data = SHAPE_TYPE::BOX;
						rb->shape_data.shapeData = { 0.5f, 0.5f, 0.5f };
						rigidbody_config staticBody;
						staticBody.is_static = true;
						rb->internal_rb = instance.po.create_actor(staticBody, rb->shape_data, transform->position, transform->rotation, entity);
						instance.po.AddActorToScene(rb->internal_rb.get());
						/*rb.internal_rb = instance.po.createRigidbody(rb.rb_config, rb.shape_data, position);
						instance.po.AddActorToScene(rb.internal_rb.get());*/
						
					}
					if (ImGui::MenuItem("Add Sphere"))
					{
						auto entity = instance.ecs.create();
						auto meta = instance.ecs.emplace<tc::Meta>(entity);
						meta->name = "Sphere";
						auto transform = instance.ecs.emplace<tc::Transform>(entity);
						auto rb = instance.ecs.emplace<tc::Rigidbody>(entity);
						instance.ecs.emplace<tc::Mesh>(entity, MeshCode::SPHERE);

						
						//auto& transform = instance.ecs.get<Components::Transform>(entity);
						rb->shape_data = SHAPE_TYPE::SPHERE;
						rb->shape_data.shapeData = { 1.f, 1.f, 1.f };
						rigidbody_config staticBody;
						staticBody.is_static = true;
						rb->internal_rb = instance.po.create_actor(staticBody, rb->shape_data, transform->position, transform->rotation, entity);
						instance.po.AddActorToScene(rb->internal_rb.get());
					}
					if (ImGui::MenuItem("Add Capsule", "", false))
					{
						auto entity = instance.ecs.create();
						auto meta = instance.ecs.emplace<tc::Meta>(entity);
						meta->name = "Capsule";
						instance.ecs.emplace<tc::Transform>(entity);
						instance.ecs.emplace<tc::Mesh>(entity, MeshCode::CUBE);
						instance.ecs.emplace<tc::Rigidbody>(entity);

						/*auto& rb = instance.ecs.get<Components::Rigidbody>(entity);
						auto& position = instance.ecs.get<Components::Transform>(entity).position;
						rb.shape_data = SHAPE_TYPE::SPHERE;
						rb.shape_data.shapeData = { 1, 1, 1 };
						rb.internal_rb = instance.po.createRigidbody(rb.rb_config, rb.shape_data, position);
						instance.po.AddActorToScene(rb.internal_rb.get());*/
					}

					ImGui::EndMenu();
				}
				UI::Tooltip(ICON_FA_QUESTION_CIRCLE, "Simple UI for selecting objects. We should improve the UI once the UI/UX for it is done.", false);

				ImGui::EndMenuBar();
			}
			// filter
			{
				filter.Draw("##hierarchy_filter");
				ImGui::SameLine();
				UI::Tooltip(ICON_FA_QUESTION_CIRCLE, "Filter usage:\n"
					"  \"\"         display all lines\n"
					"  \"xxx\"      display lines containing \"xxx\"\n"
					"  \"xxx,yyy\"  display lines containing \"xxx\" or \"yyy\"\n"
					"  \"-xxx\"     hide lines containing \"xxx\"", false);

			}

			UI::PaddedSeparator(1.f);
			// show entities
			{
				ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY;
				ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(5.f, 5.f));
				ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(.5f, .5f));
				if (ImGui::BeginTable("HierarchyTable", 5, flags, {0.f, ImGui::GetContentRegionAvail().y}))
				{
					ImGui::TableSetupScrollFreeze(0, 1);
					ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed);
					ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed);
					ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed);
					ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
					ImGui::TableSetupColumn("Actions ", ImGuiTableColumnFlags_WidthFixed);
					ImGui::TableHeadersRow();

					
					std::vector<Entity> destroyed_list;
					auto view = instance.ecs.view<tc::Meta>();
					for (auto id : view)
					{
						auto& meta = instance.ecs.get<tc::Meta>(id);
						if (!filter.PassFilter(meta.name.c_str())) continue;

						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);

						auto c = UI::CreateLabelWithId("", id);
						if (ImGui::Selectable(c.c_str(), instance.selected == id,
							ImGuiSelectableFlags_SpanAllColumns |
							ImGuiSelectableFlags_AllowItemOverlap))
						{
							if (instance.selected == id)
								instance.selected = INVALID;
							else
								instance.selected = id;
						}

						ImGui::TableSetColumnIndex(1);
						ImGui::Selectable(std::to_string(id).c_str(), false, ImGuiSelectableFlags_Disabled);
						ImGui::TableSetColumnIndex(2);
						ImGui::Selectable(ICON_FA_BOWLING_BALL, false, ImGuiSelectableFlags_Disabled);
						ImGui::TableSetColumnIndex(3);
						ImGui::Text("%s", meta.name.c_str());
						ImGui::TableSetColumnIndex(4);
						ImGui::SmallButton(ICON_FA_CROSSHAIRS);
						ImGui::SameLine();
						if (ImGui::SmallButton((std::string(ICON_FA_TIMES_CIRCLE) + "##destroyme" + std::to_string(id)).c_str() ))
						{
							destroyed_list.emplace_back(id);
						}
					}

					for(auto id : destroyed_list)
						instance.ecs.destroy(id);


					ImGui::EndTable();
				}
				ImGui::PopStyleVar(2);
			}

			{

			}
		}

		ImGui::End();


	}
}
