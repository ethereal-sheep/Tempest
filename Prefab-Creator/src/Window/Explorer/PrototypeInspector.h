#pragma once
#include "ECS/Prototypes/Prototype_Category.h"
#include "../Util/UIElements.h"

namespace Tempest
{
	class PrototypeInspector
	{
		prototype* _current = nullptr;
		prototype* _next = nullptr;

		string _current_cat;
		string _next_cat;

		enum struct State {
			SAVE,
			UPDATE,
			LOAD
		};

		


	public:

		void show(tpath path)
		{
			if (_current != _next)
			{
				// save the current
				if (_current)
				{
					// check if folder exist
					auto folder = path / _current_cat;
					if (!fs::exists(folder))
						fs::create_directories(folder);

					_current->save(folder);
				}
				_current = _next;
				_current_cat = _next_cat;
			}
			else
			{
				if (!_current)
					return;
				// draw
				const auto padding = 80.f;
				bool p_open = true;
				if (ImGui::Begin("Inspector", &p_open))
				{
					UI::RenderText("Name:", padding);
					ImGui::Text(_current->get_name().c_str());
					ImGui::Dummy({ 0.f, 1.f });
					//UI::PaddedSeparator(1.f);

					if (auto meta = _current->get_if<tc::Meta>())
					{
						bool header = ImGui::CollapsingHeader("Meta##MetaHeader", nullptr, ImGuiTreeNodeFlags_DefaultOpen);
						if (header)
						{
							UI::RenderText("Name:", padding);
							ImGui::InputText("##meta_name", &meta->name, ImGuiInputTextFlags_CharsNoBlank);
							UI::PaddedSeparator(1.f);
						}
					}

					// transform data
					if (auto transform = _current->get_if<tc::Transform>())
					{
						//auto rb = instance.ecs.get_if<tc::Rigidbody>(instance.selected);
						bool header = ImGui::CollapsingHeader("Transform##TransformHeader", nullptr, ImGuiTreeNodeFlags_DefaultOpen);

						if (header)
						{
							UI::DragFloat3ColorBox("Position", "##PositionDrag", ImVec2{ padding , 0.f }, transform->position.data(), 0.f, 0.1f);

							auto vec = glm::eulerAngles(transform->rotation);
							auto [x, y] = UI::DragFloat3ColorBox("Rotation", "##RotationDrag", ImVec2{ padding , 0.f }, &vec.x, 0.f, 0.1f);
							if (x)
							{
								transform->rotation = glm::quat(vec);
							}

							UI::DragFloat3ColorBox("Scale", "##ScaleDrag", ImVec2{ padding , 0.f }, transform->scale.data(), 0.f, 0.1f);
							UI::PaddedSeparator(1.f);

						}
					}
				}

				ImGui::Dummy({ 0.f, 2.f });
				ImGui::TextWrapped("The below are not serialized because they are not components yet");
				ImGui::Separator();
				ImGui::Dummy({ 0.f, 1.f });

				// shape
				if (true)
				{
					//auto rb = instance.ecs.get_if<tc::Rigidbody>(instance.selected);
					bool header = ImGui::CollapsingHeader("Shape##ShapeHeader", nullptr, ImGuiTreeNodeFlags_DefaultOpen);

					if (header)
					{
						static int x = 1;
						static int y = 1;
						const int one = 1;
						ImGui::PushItemWidth(padding);
						if (ImGui::InputScalar("##xby", ImGuiDataType_S32, &x, &one))
						{
							x = std::clamp(x, 1, 3);
						}
						ImGui::Text(ICON_FA_TIMES);
						if (ImGui::InputScalar("##yby", ImGuiDataType_S32, &y, &one))
						{
							y = std::clamp(y, 1, 3);
						}
						ImGui::PopItemWidth();
						UI::PaddedSeparator(1.f);

						AABB box;
						box.min.x = -0.5;
						box.min.y = -0.5;
						box.max.x = 0.5;
						box.max.y = 0.5;
						
						Service<RenderSystem>::Get().DrawLine(box, { 0,1,0,1 });
					}
				}

				// health
				if (true)
				{
					//auto rb = instance.ecs.get_if<tc::Rigidbody>(instance.selected);
					bool header = ImGui::CollapsingHeader("Stats##StatsHeader", nullptr, ImGuiTreeNodeFlags_DefaultOpen);

					if (header)
					{
						static int hp = 1;
						static int def = 1;
						const int one = 1;
						ImGui::PushItemWidth(padding);
						if (ImGui::InputScalar("HP##healthinput", ImGuiDataType_S32, &hp, &one))
						{
							hp = std::max(hp, 1);
						}
						if (ImGui::InputScalar("DEF##healthinput", ImGuiDataType_S32, &def, &one))
						{
							def = std::max(def, 0);
						}
						ImGui::PopItemWidth();


						UI::PaddedSeparator(1.f);
					}
				}
				// decoration
				if (true)
				{
					//auto rb = instance.ecs.get_if<tc::Rigidbody>(instance.selected);
					bool header = ImGui::CollapsingHeader("Decoration##DecorationHeader", nullptr, ImGuiTreeNodeFlags_DefaultOpen);

					if (header)
					{
						const static std::vector<string> resources = { "This", "Will", "Be", "Filled", "With", "Compiled", "Resources" };
						static int current = 0;
						if (ImGui::ComboWithFilter("Decoration", &current, resources))
						{

						}
						ImGui::SameLine();
						ImGui::Text("Decoration");



					}
				}

				// states
				if (true)
				{
					//auto rb = instance.ecs.get_if<tc::Rigidbody>(instance.selected);
					bool header = ImGui::CollapsingHeader("State##StateHeader", nullptr, ImGuiTreeNodeFlags_DefaultOpen);

					if (header)
					{
						const static std::vector<string> states = { "Open", "Close", "Breakable", "Interactable" };
						static std::vector<int> current = { 0,0,0,0 };
						const static std::vector<string> resources = { "This", "Will", "Be", "Filled", "With", "Compiled", "Resources"};

						for (auto i = 0; i < states.size(); ++i)
						{
							if (ImGui::ComboWithFilter(states[i].c_str(), &current[i], resources))
							{

							}
							ImGui::SameLine();
							ImGui::Text(states[i].c_str());
						}



					}
				}

				ImGui::End();

				if (!p_open)
					select(nullptr, "");
			}



		}

		void select(prototype* next_ptr, string category)
		{
			_next = next_ptr;
			_next_cat = std::move(category);
		}

		prototype* current()
		{
			return _current;
		}
	};
}