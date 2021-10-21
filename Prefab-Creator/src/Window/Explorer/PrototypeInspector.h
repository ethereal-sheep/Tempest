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
					UI::PaddedSeparator(1.f);

					if (auto meta = _current->get_if<tc::Meta>())
					{
						bool header = ImGui::CollapsingHeader("Meta##MetaHeader", nullptr, ImGuiTreeNodeFlags_DefaultOpen);
						if (header)
						{
							UI::RenderText("Name:", padding);
							ImGui::InputText("##meta_name", &meta->name, ImGuiInputTextFlags_CharsNoBlank);
						}
					}

					UI::PaddedSeparator(1.f);
					// transform data
					if (auto transform = _current->get_if<tc::Transform>())
					{
						//auto rb = instance.ecs.get_if<tc::Rigidbody>(instance.selected);
						bool header = ImGui::CollapsingHeader("Transform##TransformHeader", nullptr, ImGuiTreeNodeFlags_DefaultOpen);
						ImGui::Dummy({ 0.f, 1.f });

						if (header)
						{
							UI::DragFloat3ColorBox("Position", "##TransformPosDrag", ImVec2{ padding , 0.f }, transform->position.data(), 0.f, 0.1f);

							auto vec = glm::eulerAngles(transform->rotation);
							auto [x, y] = UI::DragFloat3ColorBox("Rotation", "##TransformPosDrag", ImVec2{ padding , 0.f }, &vec.x, 0.f, 0.1f);
							if (x)
							{
								transform->rotation = glm::quat(vec);
							}

							UI::DragFloat3ColorBox("Scale", "##TransformPosDrag", ImVec2{ padding , 0.f }, transform->scale.data(), 0.f, 0.1f);

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