#pragma once
#include "ECS/Prototypes/Prototype_Category.h"
#include "../Util/UIElements.h"
#include "Util/shape_manip.h"

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

		void show(Instance& instance)
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
						// create from category
						_current = make_uptr<prototype>(create_new_prototype(_next_cat));
						_current->load_override(_next_path);
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
					if (auto transform = _current->get_if<tc::Local>())
					{
						//auto rb = instance.ecs.get_if<tc::Rigidbody>(instance.selected);
						bool header = ImGui::CollapsingHeader("Transform##TransformHeader", nullptr, ImGuiTreeNodeFlags_DefaultOpen);

						if (header)
						{

							ImVec2 selectableSize = { 100.f, 0 };
							static bool uniformScale = false;

							{
								UI::DragFloat3ColorBox("Position", "##PositionDrag", ImVec2{ padding , 0.f }, glm::value_ptr(transform->local_position), 0.f, 0.1f);
							}
							{
								auto vec = glm::degrees(glm::eulerAngles(transform->local_rotation));
								auto [x, y] = UI::DragFloat3ColorBox("Rotation", "##RotationDrag", ImVec2{ padding , 0.f }, &vec.x, 0.f, 0.1f);
								if (x)
								{
									transform->local_rotation = glm::quat(glm::radians(vec));
								}
							}
							{
								auto vec = transform->local_scale;
								static bool uniform = false;
								auto [x, y] = UI::UniformScaleFloat3("Scale", "##ScaleDrag", ImVec2{ padding , 0.f }, &uniform, glm::value_ptr(vec), 1.f, 0.001f, 0.001f, 5.f);
								transform->local_scale.x = std::max(0.001f, vec.x);
								transform->local_scale.y = std::max(0.001f, vec.y);
								transform->local_scale.z = std::max(0.001f, vec.z);
							}
							{
								if (ImGui::Button(".1x"))
								{
									transform->local_scale.x = 
									transform->local_scale.y = 
									transform->local_scale.z = .1f;
								}
								ImGui::SameLine();
								if (ImGui::Button(".01x"))
								{
									transform->local_scale.x =
										transform->local_scale.y =
										transform->local_scale.z = .01f;
								}
								ImGui::SameLine();
								if (ImGui::Button(".02x"))
								{
									transform->local_scale.x =
										transform->local_scale.y =
										transform->local_scale.z = .02f;
								}
								ImGui::SameLine();
								if (ImGui::Button(".001x"))
								{
									transform->local_scale.x =
										transform->local_scale.y =
										transform->local_scale.z = .001f;
								}
							}


							ImGuizmo::SetDrawlist(ImGui::GetBackgroundDrawList());

							auto& GC = Service<GuizmoController>::Get();
							auto& cam = Service<RenderSystem>::Get().GetCamera();

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
							// edit mode selector
							if (ImGui::RadioButton("Translate##TransformTranslate", GC.GetOperation() == GuizmoController::Operation::TRANSLATE))
								GC.SetOperation(GuizmoController::Operation::TRANSLATE);
							ImGui::SameLine();
							if (ImGui::RadioButton("Rotate##TransformRotate", GC.GetOperation() == GuizmoController::Operation::ROTATE))
								GC.SetOperation(GuizmoController::Operation::ROTATE);
							ImGui::SameLine();
							if (ImGui::RadioButton("Scale##TransformScale", GC.GetOperation() == GuizmoController::Operation::SCALE))
								GC.SetOperation(GuizmoController::Operation::SCALE);


							GC.Enable();
							GC.Draw(cam, *transform);
							vec3 eulerDeg = glm::degrees(glm::eulerAngles(transform->local_rotation));
							GC.GetTranslateRotationScale(transform->local_position, eulerDeg, transform->local_scale);
							transform->local_rotation = glm::radians(eulerDeg);

							UI::PaddedSeparator(1.f);
						}
					}
					if (auto door = _current->get_if<tc::Door>())
					{
						int index = static_cast<int>(door->curr);
						std::vector<string> cover_types = { };
						for (auto i = 0; i < static_cast<int>(tc::Door::State::End); ++i)

							cover_types.push_back(magic_enum::enum_name(static_cast<tc::Door::State>(i)).data());

						if (ImGui::ComboWithFilter("Door States", &index, cover_types))
						{
							door->curr = static_cast<tc::Door::State>(index);
						}

						auto transform = &door->states[(int)door->curr];

						{
							UI::DragFloat3ColorBox("Position", "##DoorPositionDrag", ImVec2{ padding , 0.f }, glm::value_ptr(transform->local_position), 0.f, 0.1f);
						}
						{
							auto vec = glm::degrees(glm::eulerAngles(transform->local_rotation));
							auto [x, y] = UI::DragFloat3ColorBox("Rotation", "##DoorRotationDrag", ImVec2{ padding , 0.f }, &vec.x, 0.f, 0.1f);
							if (x)
							{
								transform->local_rotation = glm::quat(glm::radians(vec));
							}
						}
						{
							auto vec = transform->local_scale;
							static bool uniform = false;
							auto [x, y] = UI::UniformScaleFloat3("Scale", "##DoorScaleDrag", ImVec2{ padding , 0.f }, &uniform, glm::value_ptr(vec), 1.f, 0.001f, 0.001f, 5.f);
							transform->local_scale.x = std::max(0.001f, vec.x);
							transform->local_scale.y = std::max(0.001f, vec.y);
							transform->local_scale.z = std::max(0.001f, vec.z);
						}

					}
					if (auto mesh = _current->get_if<tc::Mesh>())
					{
					}
					if (auto shape = _current->get_if<tc::Shape>())
					{
						//auto rb = instance.ecs.get_if<tc::Rigidbody>(instance.selected);
						bool header = ImGui::CollapsingHeader("Shape##ShapeHeader", nullptr, ImGuiTreeNodeFlags_DefaultOpen);

						if (header)
						{
							int& x = shape->x;
							int& y = shape->y;
							const int one = 1;
							ImGui::PushItemWidth(padding);
							if (ImGui::InputScalar("X##xby", ImGuiDataType_S32, &x, &one))
							{
								x = std::clamp(x, 0, 4);
							}
							ImGui::Text(ICON_FA_TIMES);
							if (ImGui::InputScalar("Y##yby", ImGuiDataType_S32, &y, &one))
							{
								y = std::clamp(y, 0, 4);
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

							auto [a_x, a_y, b_x, b_y] = shape_bounding(x, y);

							float extra = -.5f;
							if (x == 0 || y == 0)
								extra = 0;

							box.min.x = a_x + extra;
							box.min.z = a_y + extra;
							box.min.y = 0;

							box.max.x = b_x + extra;
							box.max.z = b_y + extra;
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
							UI::PaddedSeparator(1.f);
						}
					}
					if (auto model = _current->get_if<tc::Model>())
					{
						//auto rb = instance.ecs.get_if<tc::Rigidbody>(instance.selected);
						bool header = ImGui::CollapsingHeader("Decoration##DecorationHeader", nullptr, ImGuiTreeNodeFlags_DefaultOpen);
						if (header)
						{

							int index = 0;
							std::vector<string> resources = { "None" };
							for (auto entry : fs::directory_iterator(instance.get_full_path() / "Models"))
							{
								// only get .a file
								if (entry.path().extension() != ".a")
									continue;
								auto rel = fs::relative(entry.path(), instance.get_full_path()).string();
								if (model->path == rel) {
									index = (int)resources.size();
								}
								resources.emplace_back(rel);
							}

							if (ImGui::ComboWithFilter("Decoration", &index, resources))
							{
								model->path = resources[index];
							}
							ImGui::SameLine();
							ImGui::Text("Decoration");



							if (auto door = _current->get_if<tc::Door>())
							{

								const auto& l = door->states[(int)door->curr];
								tc::Transform t;

								t.position = l.local_position;
								t.rotation = l.local_rotation;
								t.scale = l.local_scale;

								auto transform = &t;

								auto local = &_current->get<tc::Local>();


								auto mat = glm::translate(transform->position)
									* glm::mat4(transform->rotation)
									* glm::translate(local->local_position)
									* glm::mat4(local->local_rotation)
									* glm::scale(local->local_scale)
									* glm::scale(transform->scale);

								std::filesystem::path p{ model->path };
								if (strcmp(p.extension().string().c_str(), ".a"))
								{
									p.replace_extension(".a");
								}
								Service<RenderSystem>::Get().SubmitModel((instance.get_full_path() / p.string()).string(), mat);
							}
							else
							{
								const auto& local = _current->get<tc::Local>();
								tc::Transform transform;

								transform.position = local.local_position;
								transform.rotation = local.local_rotation;
								transform.scale = local.local_scale;

								std::filesystem::path p{ model->path };
								if (strcmp(p.extension().string().c_str(), ".a"))
								{
									p.replace_extension(".a");
								}
								Service<RenderSystem>::Get().SubmitModel((instance.get_full_path() / p.string()).string(), transform);
							}




							UI::PaddedSeparator(1.f);
						}
					}
					if (auto collider = _current->get_if<tc::Collision>())
					{
						//auto rb = instance.ecs.get_if<tc::Rigidbody>(instance.selected);
						bool header = ImGui::CollapsingHeader("Collider##ColliderHeader", nullptr, ImGuiTreeNodeFlags_DefaultOpen);
						if (header)
						{
							int index = static_cast<int>(collider->cover);
							std::vector<string> cover_types = { };
							for (auto i = 0; i < static_cast<int>(tc::Collision::Cover::End); ++i)
								cover_types.push_back(magic_enum::enum_name(static_cast<tc::Collision::Cover>(i)).data());

							if (ImGui::ComboWithFilter("Collider Cover", &index, cover_types))
							{
								collider->cover = static_cast<tc::Collision::Cover>(index);
							}
						}
					}
					if (auto wall = _current->get_if<tc::Wall>())
					{
						//auto rb = instance.ecs.get_if<tc::Rigidbody>(instance.selected);
						bool header = ImGui::CollapsingHeader("Wall##WallHeader", nullptr, ImGuiTreeNodeFlags_DefaultOpen);
						if (header)
						{
							int index = static_cast<int>(wall->cover);
							std::vector<string> cover_types = { };
							for (auto i = 0; i < static_cast<int>(tc::Wall::Cover::End); ++i)
								cover_types.push_back(magic_enum::enum_name(static_cast<tc::Wall::Cover>(i)).data());

							if (ImGui::ComboWithFilter("Wall Cover", &index, cover_types))
							{
								wall->cover = static_cast<tc::Wall::Cover>(index);
							}
						}
					}


					//if (auto rb = _current->get_if<tc::Rigidbody>())
					//{
					//	//auto rb = instance.ecs.get_if<tc::Rigidbody>(instance.selected);
					//	bool header = ImGui::CollapsingHeader("ObjectPicking##OP", nullptr, ImGuiTreeNodeFlags_DefaultOpen);

					//	if (header)
					//	{
					//		auto& rbConfig = rb->rb_config;
					//		static tc::Transform transform;

					//		ImGui::PushID("Collider Type");
					//		int collider_current = static_cast<int>(rb->shape_data.type);
					//		const char* Colliders[] = { "NONE", "SPHERE", "BOX", "CAPSULE" };
					//		ImGui::Text("ShapeT");
					//		ImGui::SameLine();
					//		ImGui::Dummy(ImVec2{ 60.f - ImGui::GetItemRectSize().x, 0.f });
					//		ImGui::SameLine();
					//		ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() - 20.f);
					//		if (ImGui::Combo("", &collider_current, Colliders, IM_ARRAYSIZE(Colliders)))
					//		{
					//			rb->shape_data.type = static_cast<SHAPE_TYPE>(collider_current);
					//			rb->isDirty = true;
					//		}
					//		ImGui::PopID();

					//		//Collider Size
					//		switch (rb->shape_data.type)
					//		{
					//		case SHAPE_TYPE::SPHERE:
					//			rb->isDirty |= UI::DragFloat("Radius", "##RbRadius", ImVec2{ padding , 0.f }, &rb->shape_data.shapeData.x, 0.1f, 0.5f).second;
					//			break;
					//		case SHAPE_TYPE::CAPSULE:
					//		{
					//			vec2 temp = { rb->shape_data.shapeData.x, rb->shape_data.shapeData.y };
					//			rb->isDirty |= UI::DragFloat2("Radius&Height", "##RbRadius&Height", ImVec2{ padding , 0.f }, glm::value_ptr(temp), 0.1f, 0.5f).second;
					//			rb->shape_data.shapeData.x = temp.x;
					//			rb->shape_data.shapeData.y = temp.y;
					//			break;
					//		}
					//		break;
					//		case SHAPE_TYPE::BOX:
					//		{
					//			rb->isDirty |= UI::DragFloat3("X,Y,Z", "##RbRadius&Height", ImVec2{ padding , 0.f }, glm::value_ptr(rb->shape_data.shapeData), 0.1f, 0.5f).second;
					//		}
					//		break;
					//		case SHAPE_TYPE::NONE:
					//			LOG_ERROR("No shape data");
					//			break;
					//		}
					//	}
					//}

					ImGui::Dummy({ 0.f, 2.f });
					ImGui::TextWrapped("The below are not serialized because they are not components yet");
					ImGui::Separator();
					ImGui::Dummy({ 0.f, 1.f });

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