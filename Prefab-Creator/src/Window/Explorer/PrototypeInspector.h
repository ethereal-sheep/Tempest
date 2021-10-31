#pragma once
#include "ECS/Prototypes/Prototype_Category.h"
#include "../Util/UIElements.h"

namespace Tempest
{
	class PrototypeInspector
	{
		tuptr<prototype> _current = nullptr;

		tpath _current_path;
		tpath _next_path;

		string _current_cat;
		string _next_cat;


	public:

		void show()
		{
			if (_current_path != _next_path)
			{
				// save the current
				if (_current)
				{
					// check if folder exist

					auto folder = _current_path.parent_path();
					if (!fs::exists(folder))
						fs::create_directories(folder);

					_current->save(folder / (_current->name + ".json"));
				}


				// if next file exists, create new prototype from file if possible
				if(fs::exists(_next_path))
				{
					try
					{
						_current = make_uptr<prototype>(_next_path);
					}
					catch (const std::exception&)
					{
						LOG_WARN("Could not load prototype file: {}", _next_path.string());
						_next_path = "";
						_next_cat = "";
					}
				}
				else if(_next_path != "")
				{
					LOG_WARN("Prototype file cannot be found: {}", _next_path.string());
					_next_path = "";
					_next_cat = "";
					_current.reset();
				}
				else
				{
					_current.reset();
				}

				_current_path = _next_path;
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

							ImGuizmo::SetDrawlist(ImGui::GetBackgroundDrawList());
							auto& GC = Service<GuizmoController>::Get();
							auto& cam = Service<RenderSystem>::Get().GetCamera();

							static ImVec2 Min = { 0, 0 };
							static ImVec2 Max = { 1600, 900 };

							GC.SetViewportBounds(els::to_vec2(Min), els::vec2{ Max.x - Min.x, Max.y - Min.y });


							vec3 tDelta;
							vec3 rDelta;
							vec3 sDelta;

							auto mat =
								glm::translate(glm::make_vec3(value_ptr(transform->position)))
								* glm::mat4(transform->rotation)
								* glm::scale(glm::make_vec3(value_ptr(transform->scale)));

							//GC.SetTranslateRotationScale(transform->translation, eulerDeg, transform->scale);
							GC.SetTransformMatrix(glm::value_ptr(mat));
							GC.SetViewMatrix(glm::value_ptr(cam.GetViewMatrix()));
							GC.SetProjMatrix(glm::value_ptr(cam.GetProjectionMatrix()));



							GC.Draw();

							//GC.GetTranslateRotationScale(transform->translation, eulerDeg, transform->scale);

							GC.GetDelta(tDelta, rDelta, sDelta);

							if (tDelta.length2() > els::epsilon<float> ||
								rDelta.length2() > els::epsilon<float>)
							{
								//transform
								transform->position += tDelta;
								//transform->scale += sDelta;
							}


						}
					}
					if (auto mesh = _current->get_if<tc::Mesh>())
					{
						auto transform = _current->get_if<tc::Transform>();
						if(transform)
							Service<RenderSystem>::Get().Submit(mesh->code, *transform);
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
						if (ImGui::InputScalar("X##xby", ImGuiDataType_S32, &x, &one))
						{
							x = std::clamp(x, 1, 3);
						}
						ImGui::Text(ICON_FA_TIMES);
						if (ImGui::InputScalar("Y##yby", ImGuiDataType_S32, &y, &one))
						{
							y = std::clamp(y, 1, 3);
						}
						ImGui::PopItemWidth();

						static auto a = 0;
						bool check = a != 0;
						
						if (ImGui::Checkbox("Bounding" "##ShowBoundingBoxSHape", &check))
						{
							if (a != 0)
								a = 0;
							else
								a = 2;
						}


						AABB box;
						box.min.x = -.5f - (x-1)/2.f;
						box.min.z = -.5f - (y-1)/2.f;
						box.min.y = 0;

						box.max.x = .5f + (x-1)/2.f;
						box.max.z = .5f + (y-1)/2.f;
						box.max.y = (float)a;

						Line l;
						l.p0 = glm::vec3(-.1, 0, -.1);
						l.p1 = glm::vec3(.1, 0, .1);

						Line r;
						r.p0 = glm::vec3(-.1, 0, .1);
						r.p1 = glm::vec3(.1, 0, -.1);
						
						Service<RenderSystem>::Get().DrawLine(box, { 0,1,0,1 });
						Service<RenderSystem>::Get().DrawLine(l, { 0,1,0,1 });
						Service<RenderSystem>::Get().DrawLine(r, { 0,1,0,1 });
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
					select("", "");
			}



		}

		void select(tpath path, string category)
		{
			_next_path = std::move(path);
			_next_cat = std::move(category);
		}

		bool is_selected(tpath path)
		{
			if(!_current)
				return false;

			auto folder = _current_path.parent_path();
			if (path == folder / (_current->name + ".json"))
				return true;

			return false;
		}

		prototype* current()
		{
			return _current.get();
		}
	};
}