#pragma once
#include "UIElements.h"
//#include "rttr/registration.h"


//struct MyStruct { MyStruct() {}; void func(double) {}; int data; };
//
//RTTR_REGISTRATION
//{
//	rttr::registration::class_<MyStruct>("MyStruct")
//		 .constructor<>()
//		 .property("data", &MyStruct::data)
//		 .method("func", &MyStruct::func);
//}


namespace Tempest
{
	class RenameUtil {

		string current;
		string name;

		enum struct State
		{
			RENAME_START,
			RENAME,
			TEXT
		};

		State state;
	public:
		template<typename RenameCallback>
		inline void show(const string& s, const string& id, float wrapping, RenameCallback cb)
		{
			if (state == State::RENAME_START && current == id)
			{
				ImGui::SetKeyboardFocusHere();
				ImGui::InputText("", &name);
				state = State::RENAME;
			}
			else if(state == State::RENAME && current == id)
			{
				ImGui::PushID(id.c_str());
				ImGui::PushItemWidth(wrapping);

				if (ImGui::InputText("", &name))
				{
					if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)))
					{
						cb(name);
						state = State::TEXT;
					}
				}
				ImGui::PopItemWidth();

				ImGui::PopID();

				if (!ImGui::IsItemActive())
				{
					cb(name);
					state = State::TEXT;
					

				}
			}
			else
			{
				ImGui::PushID(id.c_str());
				ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + wrapping);
				ImGui::Text(s.c_str());
				ImGui::PopTextWrapPos();
				ImGui::PopID();
			}
		}

		inline void rename(const string& s, const string& id)
		{
			current = id;
			name = s;
			state = State::RENAME_START;
		}
	};
}