#pragma once

#include "Util/UIElements.h"

namespace Tempest
{
	class LineDrawing
	{
		enum struct State {
			DEAD,
			START,
			LINE,
			END
		};

		State state = State::DEAD;
		string current;

	public:
		template<typename Func1, typename Func2>
		bool draw(const string& id, Func1 start, Func2 end)
		{
			ImGuiIO& io = ImGui::GetIO();
			bool lining = false;
			switch (state)
			{
			case State::DEAD:
				if (ImGui::IsItemActive())
				{
					current = id;
					state = State::START;
				}
				else
					break;
			case State::START:
				start();
				state = State::LINE;
			case State::LINE:

				if (id != current)
					return false;

				ImGui::GetForegroundDrawList()->AddLine(io.MouseClickedPos[0], io.MousePos,
					ImGui::GetColorU32(ImGuiCol_Button), 4.0f);
				lining = true;
				if (!ImGui::IsItemActive())
					state = State::END;
				else
					break;
			case State::END:
				end();
				lining = false;
				current = "";
				state = State::DEAD;
				break;
			default:
				break;
			}

			return lining;
		}


	};
}