/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "UIElements.h"


namespace Tempest::UI
{

	void PaddedSeparator(float padding)
	{
		ImGui::Dummy({ 0.f, padding });
		ImGui::Separator();
		ImGui::Dummy({ 0.f, padding });
	}
	void PaddedSeparator(float up, float down)
	{
		ImGui::Dummy({ 0.f, up });
		ImGui::Separator();
		ImGui::Dummy({ 0.f, down });
	}

	void HelpMarker(const char* str)
	{
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(str);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

	void Tooltip(const char* label, const char* str, bool enabled)
	{
		if (enabled)
			ImGui::Text(label);
		else
			ImGui::TextDisabled(label);

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(str);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

	void DrawLine()
	{
		ImGuiIO& io = ImGui::GetIO();
		if (ImGui::IsItemActive())
			ImGui::GetForegroundDrawList()->AddLine(io.MouseClickedPos[0], io.MousePos,
			                                        ImGui::GetColorU32(ImGuiCol_Button), 4.0f);
		// Draw a line between the button and the mouse cursor
	}

	bool ConfirmDeletePopup(const char* str)
	{
		bool ret = false;

		if (ImGui::Button(str))
		{
			ImGui::OpenPopup(str);
		}

		if (ImGui::BeginPopupModal(str, nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Confirm Deletion?\n\n");
			ImGui::Separator();

			if (ImGui::Button("OK", ImVec2(120.f, 0.f)))
			{
				ret = true;
				ImGui::CloseCurrentPopup();
			}

			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();

			if (ImGui::Button("Cancel", ImVec2(120.f, 0.f)))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		return ret;
	}

	void ShowLabel(const char* label, ImColor color)
	{
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetTextLineHeight());
		const ImVec2 size = ImGui::CalcTextSize(label);

		const ImVec2& padding = ImGui::GetStyle().FramePadding;
		const ImVec2& spacing = ImGui::GetStyle().ItemSpacing;

		const ImVec2& cursorPos = ImGui::GetCursorPos();
		const ImVec2& cursorScreenPos = ImGui::GetCursorScreenPos();

		ImGui::SetCursorPos(ImVec2(cursorPos.x + spacing.x, cursorPos.y - spacing.y));

		const ImVec2 rectMin = ImVec2(cursorScreenPos.x - padding.x, cursorScreenPos.y - padding.y);
		const ImVec2 rectMax = ImVec2(cursorScreenPos.x + size.x + padding.x, cursorScreenPos.y + size.y + padding.y);

		auto* drawList = ImGui::GetWindowDrawList();
		drawList->AddRectFilled(rectMin, rectMax, color, size.y * 0.15f);
		ImGui::TextUnformatted(label);
	}

	bool Selectable(const char* label, const char* ID, bool selected, ImGuiSelectableFlags flags, ImVec2 size)
	{
		const unsigned BUFFER_SIZE = 64;
		char buffer[BUFFER_SIZE];
		sprintf_s(buffer, BUFFER_SIZE, "%s##%s", label, ID);
		return ImGui::Selectable(buffer, selected, flags, size);
	}

	void RenderText(const std::string& label, float padding)
	{
		ImGui::Text(label.c_str());
		ImGui::SameLine();
		ImGui::Dummy(ImVec2{ padding - ImGui::GetItemRectSize().x, 0.f });
		ImGui::SameLine();
	}
	std::pair<bool, bool> DragFloat3(const char* str, const char* ID, ImVec2 padding, float v[3], float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
	{
		bool valueChangeOnEdit = false;
		bool valueChangeOnRelease = false;
		ImGui::Text(str);
		ImGui::SameLine();
		ImGui::Dummy({ padding.x - ImGui::GetItemRectSize().x, padding.y });
		ImGui::SameLine();
		valueChangeOnEdit |= ImGui::DragFloat3(ID, v, v_speed, v_min, v_max, format, flags);
		valueChangeOnRelease |= ImGui::IsItemDeactivatedAfterEdit();
		return { valueChangeOnEdit, valueChangeOnRelease };
	}
	std::pair<bool, bool> DragFloat2(const char* str, const char* ID, ImVec2 padding, float v[2], float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
	{
		bool valueChangeOnEdit = false;
		bool valueChangeOnRelease = false;
		ImGui::Text(str);
		ImGui::SameLine();
		ImGui::Dummy({ padding.x - ImGui::GetItemRectSize().x, padding.y });
		ImGui::SameLine();
		valueChangeOnEdit |= ImGui::DragFloat2(ID, v, v_speed, v_min, v_max, format, flags);
		valueChangeOnRelease |= ImGui::IsItemDeactivatedAfterEdit();
		return { valueChangeOnEdit, valueChangeOnRelease };
	}
	std::pair<bool, bool> DragFloat(const char* str, const char* ID, ImVec2 padding, float* val, float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
	{
		bool valueChangeOnEdit = false;
		bool valueChangeOnRelease = false;
		ImGui::Text(str);
		ImGui::SameLine();
		ImGui::Dummy({ padding.x - ImGui::GetItemRectSize().x, padding.y });
		ImGui::SameLine();
		valueChangeOnEdit |= ImGui::DragFloat(ID, val, v_speed, v_min, v_max, format, flags);
		valueChangeOnRelease |= ImGui::IsItemDeactivatedAfterEdit();
		return { valueChangeOnEdit, valueChangeOnRelease };
	}
	std::pair<bool, bool> DragInt(const char* label, const char* ID, ImVec2 padding, int* v, float v_speed, int v_min, int v_max, const char* format, ImGuiSliderFlags flags)
	{
		bool valueChangeOnEdit = false;
		bool valueChangeOnRelease = false;
		ImGui::Text(label);
		ImGui::SameLine();
		ImGui::Dummy({ padding.x - ImGui::GetItemRectSize().x, padding.y });
		ImGui::SameLine();
		valueChangeOnEdit |= ImGui::DragInt(ID, v, v_speed, v_min, v_max, format, flags);
		valueChangeOnRelease |= ImGui::IsItemDeactivatedAfterEdit();
		return { valueChangeOnEdit, valueChangeOnRelease };
	}
	std::pair<bool, bool> DragInt2(const char* label, const char* ID, ImVec2 padding, int v[2], float v_speed, int v_min, int v_max, const char* format, ImGuiSliderFlags flags)
	{
		bool valueChangeOnEdit = false;
		bool valueChangeOnRelease = false;
		ImGui::Text(label);
		ImGui::SameLine();
		ImGui::Dummy({ padding.x - ImGui::GetItemRectSize().x, padding.y });
		ImGui::SameLine();
		valueChangeOnEdit |= valueChangeOnEdit =ImGui::DragInt2(ID, v, v_speed, v_min, v_max, format, flags);
		valueChangeOnRelease |= ImGui::IsItemDeactivatedAfterEdit();
		return { valueChangeOnEdit, valueChangeOnRelease };
	}
	std::pair<bool, bool> DragInt3(const char* label, const char* ID, ImVec2 padding, int v[3], float v_speed, int v_min, int v_max, const char* format, ImGuiSliderFlags flags)
	{
		bool valueChangeOnEdit = false;
		bool valueChangeOnRelease = false;
		ImGui::Text(label);
		ImGui::SameLine();
		ImGui::Dummy({ padding.x - ImGui::GetItemRectSize().x, padding.y });
		ImGui::SameLine();
		valueChangeOnEdit |= ImGui::DragInt3(ID, v, v_speed, v_min, v_max, format, flags);
		valueChangeOnRelease |= ImGui::IsItemDeactivatedAfterEdit();
		return { valueChangeOnEdit, valueChangeOnRelease };
	}
	std::pair<bool, bool> DragFloat3ColorBox(const char* str, const char* ID, ImVec2 padding, float v[3], float resetValue, float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 1.f);

		bool valueChangeOnEdit = false;
		bool valueChangeOnRelease = false;
		//Setting Text to the left
		ImGui::Text(str);
		ImGui::SameLine();
		ImGui::Dummy({ padding.x - ImGui::GetItemRectSize().x, padding.y });
		ImGui::SameLine();

		//Calculating avaliable region space
		ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() - 38.f);
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth() - 50);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		// Setting X ======================================================
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		if(ImGui::Button(std::string("X").append(ID).c_str(), buttonSize))
		{
			v[0] = resetValue;
			valueChangeOnRelease |= true;
		}
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();

		ImGui::SameLine();
		valueChangeOnEdit |= ImGui::DragFloat(std::string(ID).append("X").c_str(), &v[0], v_speed, v_min, v_max, format, flags);
		ImGui::IsItemDeactivatedAfterEdit();
		valueChangeOnRelease |= ImGui::IsItemDeactivatedAfterEdit();
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::Dummy({5,0});
		ImGui::SameLine();
		// ================================================================

		// Setting Y ======================================================
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		if (ImGui::Button(std::string("Y").append(ID).c_str(), buttonSize))
		{
			v[1] = resetValue;
			valueChangeOnRelease |= true;
		}
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		valueChangeOnEdit |= ImGui::DragFloat(std::string(ID).append("Y").c_str(), &v[1], v_speed, v_min, v_max, format, flags);
		valueChangeOnRelease |= ImGui::IsItemDeactivatedAfterEdit();
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::Dummy({ 5,0 });
		ImGui::SameLine();
		// ================================================================

		// Setting Z ======================================================
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		if (ImGui::Button(std::string("Z").append(ID).c_str(), buttonSize))
		{
			v[2] = resetValue;
			valueChangeOnRelease |= true;
		}
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		valueChangeOnEdit |= ImGui::DragFloat(std::string(ID).append("Z").c_str(), &v[2], v_speed, v_min, v_max, format, flags);
		ImGui::PopItemWidth();
		// ================================================================
		ImGui::Dummy({ 0,5 });
	
		ImGui::PopStyleVar();
		ImGui::PopItemWidth();
		ImGui::PopStyleVar();
		return { valueChangeOnEdit, valueChangeOnRelease };
	}
	std::pair<bool, bool> DragFloat2ColorBox(const char* label, const char* ID, ImVec2 padding, float v[2], float resetValue, float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
	{

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 1.f);

		bool valueChangeOnEdit = false;
		bool valueChangeOnRelease = false;
		//Setting Text to the left
		ImGui::Text(label);
		ImGui::SameLine();
		ImGui::Dummy({ padding.x - ImGui::GetItemRectSize().x, padding.y });
		ImGui::SameLine();


		//Calculating avaliable region space
		ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() - 11.f);
		ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth() - 50);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		// Setting X ======================================================
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		if (ImGui::Button(std::string("X").append(ID).c_str(), buttonSize))
		{
			v[0] = resetValue;
			valueChangeOnRelease |= true;
		}
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();

		ImGui::SameLine();
		valueChangeOnEdit |= ImGui::DragFloat(std::string(ID).append("X").c_str(), &v[0], v_speed, v_min, v_max, format, flags);
		valueChangeOnRelease |= ImGui::IsItemDeactivatedAfterEdit();
		ImGui::PopItemWidth();
		// ================================================================

		// Setting Y ======================================================
		ImGui::SameLine();
		ImGui::Dummy({ 5,0 });
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		if (ImGui::Button(std::string("Y").append(ID).c_str(), buttonSize))
		{
			v[1] = resetValue;
			valueChangeOnRelease |= true;
		}
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		valueChangeOnEdit |= ImGui::DragFloat(std::string(ID).append("Y").c_str(), &v[1], v_speed, v_min, v_max, format, flags);
		valueChangeOnRelease |= ImGui::IsItemDeactivatedAfterEdit();
		ImGui::PopItemWidth();
		// ================================================================
		ImGui::Dummy({ 0,5 });

		ImGui::PopStyleVar();
		ImGui::PopItemWidth();
		ImGui::PopStyleVar();

		return { valueChangeOnEdit, valueChangeOnRelease };
	}
	std::pair<bool, bool> DragFloat3ColorBox_NoText(const char* ID, float v[3], float resetValue, float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 1.f);
		bool valueChangeOnEdit = false;
		bool valueChangeOnRelease = false;
		//Calculating avaliable region space
		ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() - 37.f);
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth() - 50);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		// Setting X ======================================================
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		if (ImGui::Button(std::string("X").append(ID).c_str(), buttonSize))
		{
			v[0] = resetValue;
			valueChangeOnRelease |= true;
		}
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();

		ImGui::SameLine();
		valueChangeOnEdit |= ImGui::DragFloat(std::string(ID).append("X").c_str(), &v[0], v_speed, v_min, v_max, format, flags);
		valueChangeOnRelease |= ImGui::IsItemDeactivatedAfterEdit();
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::Dummy({ 5,0 });
		ImGui::SameLine();
		// ================================================================

		// Setting Y ======================================================
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		if (ImGui::Button(std::string("Y").append(ID).c_str(), buttonSize))
		{
			v[1] = resetValue;
			valueChangeOnRelease |= true;
		}
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		valueChangeOnEdit |= ImGui::DragFloat(std::string(ID).append("Y").c_str(), &v[1], v_speed, v_min, v_max, format, flags);
		valueChangeOnRelease |= ImGui::IsItemDeactivatedAfterEdit();
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::Dummy({ 5,0 });
		ImGui::SameLine();
		// ================================================================

		// Setting Z ======================================================
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		if (ImGui::Button(std::string("Z").append(ID).c_str(), buttonSize))
		{
			v[2] = resetValue;
			valueChangeOnRelease |= true;
		}
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		valueChangeOnEdit |= ImGui::DragFloat(std::string(ID).append("Z").c_str(), &v[2], v_speed, v_min, v_max, format, flags);
		valueChangeOnRelease |= ImGui::IsItemDeactivatedAfterEdit();
		ImGui::PopItemWidth();
		// ================================================================
		ImGui::Dummy({ 0,5 });

		ImGui::PopStyleVar();
		ImGui::PopItemWidth();
		ImGui::PopStyleVar();
		return { valueChangeOnEdit, valueChangeOnRelease };
	}
	
	std::pair<bool, bool> DragFloat2ColorBox_NoText(const char* ID, float v[2], float resetValue, float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 1.f);
		bool valueChangeOnEdit = false;
		bool valueChangeOnRelease = false;
		//Calculating avaliable region space
		ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() - 11.f);
		ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth() - 50);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		// Setting X ======================================================
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		if (ImGui::Button(std::string("X").append(ID).c_str(), buttonSize))
		{
			v[0] = resetValue;
			valueChangeOnRelease |= true;
		}
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();

		ImGui::SameLine();
		valueChangeOnEdit |= ImGui::DragFloat(std::string(ID).append("X").c_str(), &v[0], v_speed, v_min, v_max, format, flags);
		valueChangeOnRelease |= ImGui::IsItemDeactivatedAfterEdit();
		ImGui::PopItemWidth();
		// ================================================================

		// Setting Y ======================================================
		ImGui::SameLine();
		ImGui::Dummy({ 5,0 });
		ImGui::SameLine();
		
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		if (ImGui::Button(std::string("Y").append(ID).c_str(), buttonSize))
		{
			v[1] = resetValue;
			valueChangeOnRelease |= true;
		}
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		valueChangeOnEdit |= ImGui::DragFloat(std::string(ID).append("Y").c_str(), &v[1], v_speed, v_min, v_max, format, flags);
		valueChangeOnRelease |= ImGui::IsItemDeactivatedAfterEdit();
		ImGui::PopItemWidth();
		// ================================================================
		ImGui::Dummy({ 0,5 });

		ImGui::PopStyleVar();
		ImGui::PopItemWidth();
		ImGui::PopStyleVar();
		return { valueChangeOnEdit, valueChangeOnRelease };
	}

	std::pair<bool, bool> UniformScaleFloat3(const char* label, const char* ID, ImVec2 padding, bool* b, float v[3], float resetValue, float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
	{
		ImGui::Text(label);
		ImGui::SameLine(padding.x - 26.f);
		ImGui::Text(ICON_FA_LINK);
		ImGui::SameLine();
		ImGui::Checkbox(ID, b);
		ImGui::SameLine();

		bool valueChangeOnEdit = false;
		bool valueChangeOnRelease = false;

		if (*b)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 1.f);
			vec3 temp = { v[0], v[1], v[2] };
			ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() - 37.f);
			//ImGui::PushItemWidth((ImGui::CalcItemWidth() / 2 - ImGui::GetStyle().ItemInnerSpacing.x));

			ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth() - 50);
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

			// Setting X ======================================================
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::Button("X", buttonSize);
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::SameLine();
			ImGui::DragScalar(std::string(ID).append("first").c_str(), ImGuiDataType_Float, &v[0], v_speed, &v_min, &v_max, format, flags);
			bool first = ImGui::IsItemDeactivatedAfterEdit();
			ImGui::PopItemWidth();
			// ================================================================
			ImGui::SameLine(0, ImGui::GetStyle().ItemInnerSpacing.x + 1.f);

			// Setting Y ======================================================
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			if (ImGui::Button("Y", buttonSize))
				v[1] = resetValue;
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			//bool second = ImGui::DragScalar("##secondFloatId", ImGuiDataType_Float, &val.y, 1.f, &v_min, NULL, "%.3f");
			ImGui::DragScalar(std::string(ID).append("second").c_str(), ImGuiDataType_Float, &v[1], v_speed, &v_min, &v_max, format, flags);
			bool second = ImGui::IsItemDeactivatedAfterEdit();
			ImGui::PopItemWidth();
			// ================================================================
			ImGui::SameLine(0, ImGui::GetStyle().ItemInnerSpacing.x + 1.f);

			// Setting Z ======================================================
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			if (ImGui::Button("Z", buttonSize))
				v[2] = resetValue;
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			ImGui::DragScalar(std::string(ID).append("third").c_str(), ImGuiDataType_Float, &v[2], v_speed, &v_min, &v_max, format, flags);
			bool third = ImGui::IsItemDeactivatedAfterEdit();
			ImGui::PopItemWidth();
			// ================================================================

			ImGui::Dummy({ 0,5 });
			ImGui::PopStyleVar();
			ImGui::PopItemWidth();
			ImGui::PopStyleVar();

			if (first)
			{
				float ratio = v[0] / temp.x;
				v[1] *= ratio;
				v[2] *= ratio;
				valueChangeOnEdit = true;
				valueChangeOnRelease = true;
			}
			else if (second)
			{
				float ratio = v[1] / temp.y;
				v[0] *= ratio;
				v[2] *= ratio;
				valueChangeOnEdit = true;
				valueChangeOnRelease = true;
			}
			else if (third)
			{
				float ratio = v[2] / temp.z;
				v[0] *= ratio;
				v[1] *= ratio;
				valueChangeOnEdit = true;
				valueChangeOnRelease = true;
			}
			return { valueChangeOnEdit, valueChangeOnRelease };
		}
		else
		{
			return DragFloat3ColorBox_NoText(ID, v, resetValue, v_speed, v_min, v_max, format, flags);
		}
	}

	void UniformScaleFloat2(const char* label, const char* ID, ImVec2 padding, bool* b, float v[2], float resetValue, float v_speed,
		float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 1.f);
		ImGui::Text(label);
		ImGui::SameLine();
		ImGui::Text(ICON_FA_LINK);
		ImGui::SameLine();
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
		ImGui::Checkbox("##uniform_scale", b);
		ImGui::PopStyleVar();
		ImGui::SameLine();
		ImGui::Dummy({ padding.x - ImGui::GetItemRectSize().x, padding.y});
		ImGui::SameLine();

		if (*b)
		{
			//float min = 1.f;
			float ratio = v[0] / v[1];
			ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() - 11.f);
			//ImGui::PushItemWidth((ImGui::CalcItemWidth() / 2 - ImGui::GetStyle().ItemInnerSpacing.x));
			
			ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth() - 50);
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

			// Setting X ======================================================
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			if (ImGui::Button("X", buttonSize))
				v[0] = 1.f / ratio;
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::SameLine();
			ImGui::DragScalar(std::string(ID).append("first").c_str(), ImGuiDataType_Float, &v[0], v_speed, &v_min, &v_max, format, flags);
			bool first = ImGui::IsItemDeactivatedAfterEdit();
			ImGui::PopItemWidth();
			// ================================================================
			ImGui::SameLine(0, ImGui::GetStyle().ItemInnerSpacing.x);
			
			// Setting Y ======================================================
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			if (ImGui::Button("Y", buttonSize))
				v[1] = 1.f / ratio;
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			//bool second = ImGui::DragScalar("##secondFloatId", ImGuiDataType_Float, &val.y, 1.f, &v_min, NULL, "%.3f");
			ImGui::DragScalar(std::string(ID).append("second").c_str(), ImGuiDataType_Float, &v[1], v_speed, &v_min, &v_max, format, flags);
			bool second = ImGui::IsItemDeactivatedAfterEdit();
			ImGui::PopItemWidth();
			// ================================================================
			ImGui::Dummy({ 0,5 });
			ImGui::PopStyleVar();
			ImGui::PopItemWidth();

			if (first)
			{
				v[1] = v[0] / ratio;
			}
			else if (second)
			{
				v[0] = v[1] * ratio;
			}

		}
		else
		{
			DragFloat2ColorBox_NoText(ID, v, resetValue, v_speed, v_min, v_max, format, flags);
		}
		ImGui::PopStyleVar();
	}

	std::pair<bool, bool> SliderInt(const char* label, const char* ID, ImVec2 padding, int* v, int v_min, int v_max, const char* format, ImGuiSliderFlags flags)
	{
		bool valueChangeOnEdit = false;
		bool valueChangeOnRelease = false;
		ImGui::Text(label);
		ImGui::SameLine();
		ImGui::Dummy({ padding.x - ImGui::GetItemRectSize().x, padding.y });
		ImGui::SameLine();
		valueChangeOnEdit |= ImGui::SliderInt(ID, v, v_min, v_max, format, flags);
		valueChangeOnRelease |= ImGui::IsItemDeactivatedAfterEdit();
		return { valueChangeOnEdit, valueChangeOnRelease };
	}

	std::pair<bool, bool> SliderInt2(const char* label, const char* ID, ImVec2 padding, int v[2], int v_min, int v_max, const char* format, ImGuiSliderFlags flags)
	{
		bool valueChangeOnEdit = false;
		bool valueChangeOnRelease = false;
		ImGui::Text(label);
		ImGui::SameLine();
		ImGui::Dummy({ padding.x - ImGui::GetItemRectSize().x, padding.y });
		ImGui::SameLine();
		valueChangeOnEdit |= ImGui::SliderInt2(ID, v, v_min, v_max, format, flags);
		valueChangeOnRelease |= ImGui::IsItemDeactivatedAfterEdit();
		return { valueChangeOnEdit, valueChangeOnRelease };
	}

	std::pair<bool, bool> SliderInt3(const char* label, const char* ID, ImVec2 padding, int v[3], int v_min, int v_max, const char* format, ImGuiSliderFlags flags)
	{
		bool valueChangeOnEdit = false;
		bool valueChangeOnRelease = false;
		ImGui::Text(label);
		ImGui::SameLine();
		ImGui::Dummy({ padding.x - ImGui::GetItemRectSize().x, padding.y });
		ImGui::SameLine();
		valueChangeOnEdit |= ImGui::SliderInt3(ID, v, v_min, v_max, format, flags);
		valueChangeOnRelease |= ImGui::IsItemDeactivatedAfterEdit();
		return { valueChangeOnEdit, valueChangeOnRelease };
	}

	std::pair<bool, bool> SliderFloat(const char* label, const char* ID, ImVec2 padding, float* v, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
	{
		bool valueChangeOnEdit = false;
		bool valueChangeOnRelease = false;
		ImGui::Text(label);
		ImGui::SameLine();
		ImGui::Dummy({ padding.x - ImGui::GetItemRectSize().x, padding.y });
		ImGui::SameLine();
		valueChangeOnEdit |= ImGui::SliderFloat(ID, v, v_min, v_max, format, flags);
		valueChangeOnRelease |= ImGui::IsItemDeactivatedAfterEdit();
		return { valueChangeOnEdit, valueChangeOnRelease };
	}

	std::pair<bool, bool> SliderFloat2(const char* label, const char* ID, ImVec2 padding, float v[2], float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
	{
		bool valueChangeOnEdit = false;
		bool valueChangeOnRelease = false;
		ImGui::Text(label);
		ImGui::SameLine();
		ImGui::Dummy({ padding.x - ImGui::GetItemRectSize().x, padding.y });
		ImGui::SameLine();
		valueChangeOnEdit |= ImGui::SliderFloat2(ID, v, v_min, v_max, format, flags);
		valueChangeOnRelease |= ImGui::IsItemDeactivatedAfterEdit();
		return { valueChangeOnEdit, valueChangeOnRelease };
	}

	std::pair<bool, bool> SliderFloat3(const char* label, const char* ID, ImVec2 padding, float v[3], float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
	{
		bool valueChangeOnEdit = false;
		bool valueChangeOnRelease = false;
		ImGui::Text(label);
		ImGui::SameLine();
		ImGui::Dummy({ padding.x - ImGui::GetItemRectSize().x, padding.y });
		ImGui::SameLine();
		valueChangeOnEdit |= ImGui::SliderFloat3(ID, v, v_min, v_max, format, flags);
		valueChangeOnRelease |= ImGui::IsItemDeactivatedAfterEdit();
		return { valueChangeOnEdit, valueChangeOnRelease };
	}

	std::pair<bool, bool> InputFloat(const char* label, const char* ID, ImVec2 padding, float* v, float step, float step_fast, const char* format, ImGuiInputTextFlags flags)
	{
		bool valueChangeOnEdit = false;
		bool valueChangeOnRelease = false;
		ImGui::Text(label);
		ImGui::SameLine();
		ImGui::Dummy({ padding.x - ImGui::GetItemRectSize().x, padding.y });
		ImGui::SameLine();
		valueChangeOnEdit |= ImGui::InputFloat(ID, v, step, step_fast, format, flags);
		valueChangeOnRelease |= ImGui::IsItemDeactivatedAfterEdit();
		return { valueChangeOnEdit, valueChangeOnRelease };
	}

	std::pair<bool, bool> ColorEdit4(const char* label, const char* ID, ImVec2 padding, float col[4], ImGuiColorEditFlags flags)
	{
		bool valueChangeOnEdit = false;
		bool valueChangeOnRelease = false;
		ImGui::Text(label);
		ImGui::SameLine();
		ImGui::Dummy({ padding.x - ImGui::GetItemRectSize().x, padding.y });
		ImGui::SameLine();
		valueChangeOnEdit |= ImGui::ColorEdit4(ID, col, flags);
		valueChangeOnRelease |= ImGui::IsItemDeactivatedAfterEdit();
		return { valueChangeOnEdit, valueChangeOnRelease };
	}

	std::pair<bool, bool> ColorEdit3(const char* label, const char* ID, ImVec2 padding, float col[3], ImGuiColorEditFlags flags)
	{
		bool valueChangeOnEdit = false;
		bool valueChangeOnRelease = false;
		ImGui::Text(label);
		ImGui::SameLine();
		ImGui::Dummy({ padding.x - ImGui::GetItemRectSize().x, padding.y });
		ImGui::SameLine();
		valueChangeOnEdit |= ImGui::ColorEdit3(ID, col, flags);
		valueChangeOnRelease |= ImGui::IsItemDeactivatedAfterEdit();
		return { valueChangeOnEdit, valueChangeOnRelease };
	}

	bool Checkbox(const char* str, const char* ID, ImVec2 padding, bool* val)
	{
		bool pressed = false;
		ImGui::PushID(str);
		ImGui::Text(str);
		ImGui::SameLine();
		ImGui::Dummy({ padding.x - ImGui::GetItemRectSize().x, padding.y });
		ImGui::SameLine();
		pressed |= ImGui::Checkbox(ID, val);
		ImGui::PopID();
		return pressed;
	}

	bool InputText(const char* label, const char* ID, ImVec2 padding, std::string* str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
	{
		bool pressed = false;
		ImGui::PushID(ID);
		ImGui::Text(label);
		ImGui::SameLine();
		ImGui::Dummy({ padding.x - ImGui::GetItemRectSize().x, padding.y });
		ImGui::SameLine();
		pressed |= ImGui::InputText(ID, str, flags, callback, user_data);
		ImGui::PopID();
		return pressed;
	}
	void SubHeader(const char* str)
	{
		ImGui::PushFont(FONT_HEAD);
		auto curr_tex = tex_map["Assets/SubHeaderStyle.png"];
		auto windowWidth = ImGui::GetWindowSize().x;
		auto textWidth = ImGui::CalcTextSize(str).x;


		ImGui::BeginGroup();
		ImGui::SetCursorPosX((windowWidth - textWidth - (curr_tex->GetWidth()*2)) * 0.5f);
		ImGui::Image((void*)static_cast<size_t>(curr_tex->GetID()), ImVec2(static_cast<float>(curr_tex->GetWidth()), static_cast<float>(curr_tex->GetHeight())));
		ImGui::SameLine();
		ImGui::Text(str);
		ImGui::SameLine();
		ImGui::Image((void*)static_cast<size_t>(curr_tex->GetID()), ImVec2(static_cast<float>(curr_tex->GetWidth()), static_cast<float>(curr_tex->GetHeight())), { 1.f,1.f }, { 0.f,0.f });
		ImGui::EndGroup();
		ImGui::PopFont();

		
	}
	void Header_1(const char* str)
	{
		ImGui::PushFont(FONT_HEAD);
		auto curr_tex = tex_map["Assets/HeaderStyle1.png"];
		auto windowWidth = ImGui::GetWindowSize().x;
		auto textWidth = ImGui::CalcTextSize(str).x;

		
		ImGui::BeginGroup();
		ImGui::SetCursorPosX((windowWidth - curr_tex->GetWidth()) * 0.5f);
		ImGui::Image((void*)static_cast<size_t>(curr_tex->GetID()), ImVec2(static_cast<float>(curr_tex->GetWidth()), static_cast<float>(curr_tex->GetHeight())));
		ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
		ImGui::Text(str);
		ImGui::SetCursorPosX((windowWidth - curr_tex->GetWidth()) * 0.5f);
		ImGui::Image((void*)static_cast<size_t>(curr_tex->GetID()), ImVec2(static_cast<float>(curr_tex->GetWidth()), static_cast<float>(curr_tex->GetHeight())), { 0.f,1.f }, { 1.f, 0.f });
		ImGui::EndGroup();
		ImGui::PopFont();
	}

	void Header_2(const char* str)
	{
		ImGui::PushFont(FONT_HEAD);
		auto curr_tex = tex_map["Assets/HeaderStyle2.png"];
		auto windowWidth = ImGui::GetWindowSize().x;
		auto textWidth = ImGui::CalcTextSize(str).x;
		ImGui::BeginGroup();
		ImGui::SetCursorPosX((windowWidth - curr_tex->GetWidth()) * 0.5f);
		ImGui::Image((void*)static_cast<size_t>(curr_tex->GetID()), ImVec2(static_cast<float>(curr_tex->GetWidth()), static_cast<float>(curr_tex->GetHeight())));
		ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
		ImGui::Text(str);
		ImGui::SetCursorPosX((windowWidth - curr_tex->GetWidth()) * 0.5f);
		ImGui::Image((void*)static_cast<size_t>(curr_tex->GetID()), ImVec2(static_cast<float>(curr_tex->GetWidth()), static_cast<float>(curr_tex->GetHeight())), { 0.f,1.f }, { 1.f,0.f });
		ImGui::EndGroup();
		ImGui::PopFont();
	}
	
	bool UIButton_1(string unselected, string hover, ImVec2 pos, ImVec2 padding, ImFont* font, bool selected)
	{
		const float default_padding_x = 8.f;
		const float default_padding_y = 8.f;
		const float border_size = 1.5f;
		
		const ImVec4 default_border_col = { 1.f, 1.f, 1.f, 1.f };
		const ImVec4 hovered_border_col = { 0.980f, 0.768f, 0.509f, 1.f };
		const ImVec4 button_bg_col = { 0.062f, 0.062f, 0.062f, 1.f };

		static float rounding = 0.f;
		//float center_x = ImGui::GetContentRegionAvailWidth() / 2.f;
		padding.y += 10.f;

		// button shit
		ImGui::PushFont(font);
		ImVec2 text_size = ImGui::CalcTextSize(unselected.c_str(), nullptr, true);
		ImVec2 alt_text_size = ImGui::CalcTextSize(hover.c_str(), nullptr, true);
		ImVec2 act_text_size = {
			std::max(text_size.x, alt_text_size.x),
			std::max(text_size.y, alt_text_size.y)
		};
		ImGui::PopFont();

		ImVec2 button_size = {
			act_text_size.x + default_padding_x + padding.x,
			act_text_size.y + default_padding_y + padding.y
		};

		const ImVec2 new_pos{ pos.x - button_size.x * 0.5f,  pos.y - button_size.y * 0.5f };
		const ImVec2 text_pos{ new_pos.x + button_size.x * 0.5f - text_size.x * 0.5f, new_pos.y + button_size.y * 0.5f - text_size.y * 0.5f };

		ImGui::SetCursorPos(new_pos);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
		ImGui::InvisibleButton("##NiceButton", button_size);
		ImGui::PopStyleVar(1);
		ImGui::SetCursorPos(new_pos);
		bool hovered = ImGui::IsItemHovered();
		if (selected)
		{
			// hovered
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, border_size);
			ImGui::PushStyleColor(ImGuiCol_Border, hovered_border_col);
			ImGui::PushStyleColor(ImGuiCol_Button, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_bg_col);
			ImGui::Button("##NiceButton_Dummy", button_size);
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(4);

			ImGui::SetCursorPos(text_pos);
			ImGui::PushFont(font);
			ImGui::Text(hover.c_str());
			ImGui::PopFont();
			auto io = ImGui::GetIO();
			if (hovered && ImGui::IsMouseClicked(0))
			{
				return true;
			}
		}
		else if (!ImGui::IsItemHovered())
		{
			// default
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, border_size);
			ImGui::PushStyleColor(ImGuiCol_Border, default_border_col);
			ImGui::PushStyleColor(ImGuiCol_Button, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_bg_col);
			ImGui::Button("##NiceButton_Dummy", button_size);
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(4);
			ImGui::SetCursorPos(text_pos);
			/*ImGui::SetCursorPos(
				{
					pos.x + button_size.x / 2.f - text_size.x / 2.f,
					pos.y + 2.f
				});*/
			ImGui::PushFont(font);
			ImGui::Text(unselected.c_str());
			ImGui::PopFont();
		}
		else
		{
			// hovered
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, border_size);
			ImGui::PushStyleColor(ImGuiCol_Border, hovered_border_col);
			ImGui::PushStyleColor(ImGuiCol_Button, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_bg_col);
			ImGui::Button("##NiceButton_Dummy", button_size);
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(4);

			ImGui::SetCursorPos(text_pos);
			ImGui::PushFont(font);
			ImGui::Text(hover.c_str());
			ImGui::PopFont();

			auto io = ImGui::GetIO();
			if (ImGui::IsMouseClicked(0))
			{
				return true;
			}
		}
		return false;
	}

    bool UIButton_2(string unselected, string hover, ImVec2 pos, ImVec2 padding, ImFont* font, bool selected)
	{
		const float default_padding_x = 8.f;
		const float default_padding_y = 8.f;
		const float border_size = 1.5f;
		
		const ImVec4 default_border_col = { 1.f, 1.f, 1.f, 1.f };
		const ImVec4 hovered_border_col = { 0.980f, 0.768f, 0.509f, 1.f };
		const ImVec4 button_bg_col = { 0.062f, 0.062f, 0.062f, 1.f };
		string str = "aaaaaaaaaaaaaaa000000";
		static float rounding = 0.f;
		//float center_x = ImGui::GetContentRegionAvailWidth() / 2.f;
		padding.y += 10.f;

		// button shit
		ImGui::PushFont(font);
		ImVec2 text_size = ImGui::CalcTextSize(str.c_str(), nullptr, true);
		ImVec2 test = ImGui::CalcTextSize(unselected.c_str(), nullptr, true);
		ImVec2 alt_text_size = ImGui::CalcTextSize(str.c_str(), nullptr, true);
		ImVec2 act_text_size = {
			std::max(text_size.x, alt_text_size.x),
			std::max(text_size.y, alt_text_size.y)
		};
		ImGui::PopFont();

		ImVec2 button_size = {
			act_text_size.x + default_padding_x + padding.x,
			act_text_size.y + default_padding_y + padding.y
		};

		const ImVec2 new_pos{ pos.x - button_size.x * 0.5f,  pos.y - button_size.y * 0.5f };
		//const ImVec2 text_pos{ new_pos.x + button_size.x * 0.5f - text_size.x * 0.5f, new_pos.y + button_size.y * 0.5f - text_size.y * 0.5f };
		const ImVec2 text_pos{ new_pos.x + button_size.x * 0.5f - test.x * 0.5f, new_pos.y + button_size.y * 0.5f - test.y * 0.5f };

		ImGui::SetCursorPos(new_pos);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
		ImGui::InvisibleButton("##NiceButton", button_size);
		ImGui::PopStyleVar(1);
		bool hovered = ImGui::IsItemHovered();
		ImGui::SetCursorPos(new_pos);
		if (selected)
		{
			// hovered
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, border_size);
			ImGui::PushStyleColor(ImGuiCol_Border, hovered_border_col);
			ImGui::PushStyleColor(ImGuiCol_Button, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_bg_col);
			ImGui::Button("##NiceButton_Dummy", button_size);
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(4);

			ImGui::SetCursorPos(text_pos);
			ImGui::PushFont(font);
			ImGui::Text(hover.c_str());
			ImGui::PopFont();
			auto io = ImGui::GetIO();
			if (hovered && ImGui::IsMouseClicked(0))
			{
				return true;
			}
		}
		else if (!ImGui::IsItemHovered())
		{
			// default
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, border_size);
			ImGui::PushStyleColor(ImGuiCol_Border, default_border_col);
			ImGui::PushStyleColor(ImGuiCol_Button, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_bg_col);
			ImGui::Button("##NiceButton_Dummy", button_size);
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(4);
			ImGui::SetCursorPos(text_pos);
			/*ImGui::SetCursorPos(
				{
					pos.x + button_size.x / 2.f - text_size.x / 2.f,
					pos.y + 2.f
				});*/
			ImGui::PushFont(font);
			ImGui::Text(unselected.c_str());
			ImGui::PopFont();
		}
		else
		{
			// hovered
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, border_size);
			ImGui::PushStyleColor(ImGuiCol_Border, hovered_border_col);
			ImGui::PushStyleColor(ImGuiCol_Button, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_bg_col);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_bg_col);
			ImGui::Button("##NiceButton_Dummy", button_size);
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(4);

			ImGui::SetCursorPos(text_pos);
			ImGui::PushFont(font);
			ImGui::Text(hover.c_str());
			ImGui::PopFont();

			auto io = ImGui::GetIO();
			if (ImGui::IsMouseClicked(0))
			{
				return true;
			}
		}
		return false;
	}

	void AddUnderline(ImU32 col, ImVec2 min, ImVec2 max)
	{
		//ImVec2 min = ImGui::GetItemRectMin();
		//ImVec2 max = ImGui::GetItemRectMax();
		min.y = max.y;
		ImGui::GetWindowDrawList()->AddLine(
			min, max, col, 2.0f);
	}

	bool UISelectable(const char* label, bool selected, ImGuiSelectableFlags flags, const ImVec2& size_arg)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;

		// Submit label or explicit size to ItemSize(), whereas ItemAdd() will submit a larger/spanning rectangle.
		ImGuiID id = window->GetID(label);
		ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
		ImVec2 size(size_arg.x != 0.0f ? size_arg.x : label_size.x, size_arg.y != 0.0f ? size_arg.y : label_size.y);
		ImVec2 pos = window->DC.CursorPos;

		//Calculating the center position of the text
		float CreateItmCol = ImGui::GetContentRegionAvail().x * 0.5f;
		string CreateItmStr = label;
		float CreateItmCenter = CreateItmCol - label_size.x + (label_size.x * 0.5f);
		pos.x += CreateItmCenter;
		pos.y += window->DC.CurrLineTextBaseOffset;
		ImGui::ItemSize(size, 0.0f);

		// Fill horizontal space
		// We don't support (size < 0.0f) in Selectable() because the ItemSpacing extension would make explicitly right-aligned sizes not visibly match other widgets.
		const bool span_all_columns = (flags & ImGuiSelectableFlags_SpanAllColumns) != 0;
		const float min_x = span_all_columns ? window->ParentWorkRect.Min.x : pos.x;
		const float max_x = span_all_columns ? window->ParentWorkRect.Max.x : window->WorkRect.Max.x;
		if (size_arg.x == 0.0f || (flags & ImGuiSelectableFlags_SpanAvailWidth))
			size.x = ImMax(label_size.x, max_x - min_x);

		// Text stays at the submission position, but bounding box may be extended on both sides
		const ImVec2 text_min = pos;
		const ImVec2 text_max(min_x + size.x, pos.y + size.y);

		// Selectables are meant to be tightly packed together with no click-gap, so we extend their box to cover spacing between selectable.
		ImRect bb(min_x, pos.y, text_max.x, text_max.y);
		if ((flags & ImGuiSelectableFlags_NoPadWithHalfSpacing) == 0)
		{
			const float spacing_x = span_all_columns ? 0.0f : style.ItemSpacing.x;
			const float spacing_y = style.ItemSpacing.y;
			const float spacing_L = IM_FLOOR(spacing_x * 0.50f);
			const float spacing_U = IM_FLOOR(spacing_y * 0.50f);
			bb.Min.x -= spacing_L;
			bb.Min.y -= spacing_U;
			bb.Max.x += (spacing_x - spacing_L);
			bb.Max.y += (spacing_y - spacing_U);
		}
		//if (g.IO.KeyCtrl) { GetForegroundDrawList()->AddRect(bb.Min, bb.Max, IM_COL32(0, 255, 0, 255)); }

		// Modify ClipRect for the ItemAdd(), faster than doing a PushColumnsBackground/PushTableBackground for every Selectable..
		const float backup_clip_rect_min_x = window->ClipRect.Min.x;
		const float backup_clip_rect_max_x = window->ClipRect.Max.x;
		if (span_all_columns)
		{
			window->ClipRect.Min.x = window->ParentWorkRect.Min.x;
			window->ClipRect.Max.x = window->ParentWorkRect.Max.x;
		}

		bool item_add;
		if (flags & ImGuiSelectableFlags_Disabled)
		{
			ImGuiItemFlags backup_item_flags = g.CurrentItemFlags;
			g.CurrentItemFlags |= ImGuiItemFlags_Disabled | ImGuiItemFlags_NoNavDefaultFocus;
			item_add = ImGui::ItemAdd(bb, id);
			g.CurrentItemFlags = backup_item_flags;
		}
		else
		{
			item_add = ImGui::ItemAdd(bb, id);
		}

		if (span_all_columns)
		{
			window->ClipRect.Min.x = backup_clip_rect_min_x;
			window->ClipRect.Max.x = backup_clip_rect_max_x;
		}

		if (!item_add)
			return false;

		// FIXME: We can standardize the behavior of those two, we could also keep the fast path of override ClipRect + full push on render only,
		// which would be advantageous since most selectable are not selected.
		if (span_all_columns && window->DC.CurrentColumns)
			ImGui::PushColumnsBackground();
		else if (span_all_columns && g.CurrentTable)
			ImGui::TablePushBackgroundChannel();

		// We use NoHoldingActiveID on menus so user can click and _hold_ on a menu then drag to browse child entries
		ImGuiButtonFlags button_flags = 0;
		if (flags & ImGuiSelectableFlags_NoHoldingActiveID) { button_flags |= ImGuiButtonFlags_NoHoldingActiveId; }
		if (flags & ImGuiSelectableFlags_SelectOnClick) { button_flags |= ImGuiButtonFlags_PressedOnClick; }
		if (flags & ImGuiSelectableFlags_SelectOnRelease) { button_flags |= ImGuiButtonFlags_PressedOnRelease; }
		if (flags & ImGuiSelectableFlags_Disabled) { button_flags |= ImGuiButtonFlags_Disabled; }
		if (flags & ImGuiSelectableFlags_AllowDoubleClick) { button_flags |= ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnDoubleClick; }
		if (flags & ImGuiSelectableFlags_AllowItemOverlap) { button_flags |= ImGuiButtonFlags_AllowItemOverlap; }

		if (flags & ImGuiSelectableFlags_Disabled)
			selected = false;

		const bool was_selected = selected;
		bool hovered, held;
		bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, button_flags);

		// Update NavId when clicking or when Hovering (this doesn't happen on most widgets), so navigation can be resumed with gamepad/keyboard
		if (pressed || (hovered && (flags & ImGuiSelectableFlags_SetNavIdOnHover)))
		{
			if (!g.NavDisableMouseHover && g.NavWindow == window && g.NavLayer == window->DC.NavLayerCurrent)
			{
				ImGui::SetNavID(id, window->DC.NavLayerCurrent, window->DC.NavFocusScopeIdCurrent, ImRect({ bb.Min.x - window->Pos.x, bb.Min.y - window->Pos.y }, { bb.Max.x - window->Pos.x, bb.Max.y - window->Pos.y }));
				g.NavDisableHighlight = true;
			}
		}
		if (pressed)
			ImGui::MarkItemEdited(id);

		if (flags & ImGuiSelectableFlags_AllowItemOverlap)
			ImGui::SetItemAllowOverlap();

		// In this branch, Selectable() cannot toggle the selection so this will never trigger.
		if (selected != was_selected) //-V547
			window->DC.LastItemStatusFlags |= ImGuiItemStatusFlags_ToggledSelection;

		// Render
		if (held && (flags & ImGuiSelectableFlags_DrawHoveredWhenHeld))
			hovered = true;
		if (hovered || selected)
		{
			//Getting the texture and centering the position of the Image
			auto highlightImg = tex_map["Assets/MainMenuButton.png"];
			const ImU32 col = ImGui::GetColorU32({ 0.980f, 0.768f, 0.509f, 1.f });
			float tex_width = highlightImg->GetWidth() * 0.5f;
			//float tex_height = highlightImg->GetHeight() * 0.5f;
			const ImVec2 hMin = { bb.Min.x - (tex_width * 0.5f) + (label_size.x * 0.5f), bb.Min.y };
			const ImVec2 hMax = { hMin.x + tex_width, bb.Max.y };
			
			//Adding of the Highlighted image when hover
			ImGui::GetWindowDrawList()->AddImage((void*)static_cast<size_t>(highlightImg->GetID()), hMin, hMax);

			//Debugging Purposes
			//ImGui::RenderFrame(hMin, hMax, col, false, 0.0f);
		}

		if (span_all_columns && window->DC.CurrentColumns)
			ImGui::PopColumnsBackground();
		else if (span_all_columns && g.CurrentTable)
			ImGui::TablePopBackgroundChannel();

		if (flags & ImGuiSelectableFlags_Disabled) ImGui::PushStyleColor(ImGuiCol_Text, style.Colors[ImGuiCol_TextDisabled]);
		
		ImGui::RenderTextClipped(text_min, text_max, label, NULL, &label_size, style.SelectableTextAlign, &bb);
		if (flags & ImGuiSelectableFlags_Disabled) ImGui::PopStyleColor();

		// Automatically close popups
		if (pressed && (window->Flags & ImGuiWindowFlags_Popup) && !(flags & ImGuiSelectableFlags_DontClosePopups) && !(g.CurrentItemFlags & ImGuiItemFlags_SelectableDontClosePopup))
			ImGui::CloseCurrentPopup();

		IMGUI_TEST_ENGINE_ITEM_INFO(id, label, window->DC.LastItemStatusFlags);
		return pressed;
	}
	bool UISelectable(const char* label, bool* p_selected, ImGuiSelectableFlags flags, const ImVec2& size_arg)
	{
		if (UISelectable(label, *p_selected, flags, size_arg))
		{
			*p_selected = !*p_selected;
			return true;
		}
		return false;
	}
	bool UICheckBox_1(const char* label, bool* v)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;
		
		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);
		const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
		auto curr_tex = tex_map["Assets/Unselected.png"];
		const float square_sz = ImGui::GetFrameHeight();
		//const float square_sz = curr_tex->GetHeight();
		const ImVec2 pos = window->DC.CursorPos;
		ImVec2 sqSz = ImVec2(square_sz + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), label_size.y + style.FramePadding.y * 2.0f);
		const ImRect total_bb(pos, { pos.x + sqSz.x, pos.y + sqSz.y });
		ImGui::ItemSize(total_bb, style.FramePadding.y);
		if (!ImGui::ItemAdd(total_bb, id))
		{
			IMGUI_TEST_ENGINE_ITEM_INFO(id, label, window->DC.LastItemStatusFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
			return false;
		}

		bool hovered, held;
		bool pressed = ImGui::ButtonBehavior(total_bb, id, &hovered, &held);
		if (pressed)
		{
			*v = !(*v);
			
			ImGui::MarkItemEdited(id);
		}

		const ImRect check_bb(pos, { pos.x + square_sz, pos.y + square_sz });
		ImGui::RenderNavHighlight(total_bb, id);
	
		if (!*v)
		{
			window->DrawList->AddImage((void*)static_cast<size_t>(curr_tex->GetID()), check_bb.Min, check_bb.Max);
			//ImGui::Image((void*)static_cast<size_t>(curr_tex->GetID()), ImVec2(check_bb.GetWidth(), check_bb.GetWidth()));
		}
		
		ImU32 check_col = ImGui::GetColorU32(ImGuiCol_CheckMark);
		bool mixed_value = (g.CurrentItemFlags & ImGuiItemFlags_MixedValue) != 0;
		if (mixed_value)
		{
			// Undocumented tristate/mixed/indeterminate checkbox (#2644)
			// This may seem awkwardly designed because the aim is to make ImGuiItemFlags_MixedValue supported by all widgets (not just checkbox)
			ImVec2 pad(ImMax(1.0f, IM_FLOOR(square_sz / 3.6f)), ImMax(1.0f, IM_FLOOR(square_sz / 3.6f)));
			window->DrawList->AddRectFilled({ check_bb.Min.x + pad.x, check_bb.Min.y + pad.y }, { check_bb.Max.x - pad.x, check_bb.Max.y - pad.y}, check_col, style.FrameRounding);
		}
		else if (*v)
		{
			const float pad = ImMax(1.0f, IM_FLOOR(square_sz / 6.0f));
			curr_tex = tex_map["Assets/Selected.png"];
			window->DrawList->AddImage((void*)static_cast<size_t>(curr_tex->GetID()), check_bb.Min, check_bb.Max);
			//ImGui::RenderCheckMark(window->DrawList, { check_bb.Min.x + pad, check_bb.Min.y + pad }, check_col, square_sz - pad * 2.0f);
		}

		ImVec2 label_pos = ImVec2(check_bb.Max.x + style.ItemInnerSpacing.x, check_bb.Min.y + style.FramePadding.y);
		if (g.LogEnabled)
			ImGui::LogRenderedText(&label_pos, mixed_value ? "[~]" : *v ? "[x]" : "[ ]");
		if (label_size.x > 0.0f)
			ImGui::RenderText(label_pos, label);

		IMGUI_TEST_ENGINE_ITEM_INFO(id, label, window->DC.LastItemStatusFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
		return pressed;
	}

	bool UIMapSelectable(ImTextureID tex, const char* name, const char* date, const char* unitData, bool selected, ImGuiSelectableFlags flags, const ImVec2& size_arg)
	{
		
		 ImGuiWindow* window = ImGui::GetCurrentWindow();
		 if (window->SkipItems)
		 	return false;

		 ImGuiContext& g = *GImGui;
		 const ImGuiStyle& style = g.Style;

		 // Submit label or explicit size to ItemSize(), whereas ItemAdd() will submit a larger/spanning rectangle.
		 ImGuiID id = window->GetID(name);
		 ImVec2 label_size = ImGui::CalcTextSize(name, NULL, true);
		 ImVec2 label_size2 = ImGui::CalcTextSize(date, NULL, true);
		 ImVec2 size(size_arg.x != 0.0f ? size_arg.x : label_size.x, size_arg.y != 0.0f ? size_arg.y : label_size.y);
		 ImVec2 pos = window->DC.CursorPos;
		 pos.y += window->DC.CurrLineTextBaseOffset;
		 ImGui::ItemSize(size, 0.0f);
		
		 // Fill horizontal space
		 // We don't support (size < 0.0f) in Selectable() because the ItemSpacing extension would make explicitly right-aligned sizes not visibly match other widgets.
		 const bool span_all_columns = (flags & ImGuiSelectableFlags_SpanAllColumns) != 0;
		 const float min_x = span_all_columns ? window->ParentWorkRect.Min.x : pos.x;
		 const float max_x = span_all_columns ? window->ParentWorkRect.Max.x : window->WorkRect.Max.x;
		 if (size_arg.x == 0.0f || (flags & ImGuiSelectableFlags_SpanAvailWidth))
		 	size.x = ImMax(label_size.x, max_x - min_x);

		 // Text stays at the submission position, but bounding box may be extended on both sides
		 const ImVec2 text_min = { pos.x +  (size.x * 0.33f) , pos.y - size.y * 0.7f };
		 const ImVec2 text_min2 = { pos.x +  (size.x * 0.33f) , pos.y };
		 const ImVec2 text_min3 = { pos.x +  (size.x * 0.33f) , pos.y + size.y * 0.7f };
		 const ImVec2 text_max(min_x + size.x, pos.y + size.y);

		 // Selectables are meant to be tightly packed together with no click-gap, so we extend their box to cover spacing between selectable.
		 ImRect bb(min_x, pos.y, text_max.x, text_max.y);
		 if ((flags & ImGuiSelectableFlags_NoPadWithHalfSpacing) == 0)
		 {
		 	const float spacing_x = span_all_columns ? 0.0f : style.ItemSpacing.x;
		 	const float spacing_y = style.ItemSpacing.y;
		 	const float spacing_L = IM_FLOOR(spacing_x * 0.50f);
		 	const float spacing_U = IM_FLOOR(spacing_y * 0.50f);
		 	bb.Min.x -= spacing_L;
		 	bb.Min.y -= spacing_U;
		 	bb.Max.x += (spacing_x - spacing_L);
		 	bb.Max.y += (spacing_y - spacing_U);
		 }
		 //if (g.IO.KeyCtrl) { GetForegroundDrawList()->AddRect(bb.Min, bb.Max, IM_COL32(0, 255, 0, 255)); }

		 // Modify ClipRect for the ItemAdd(), faster than doing a PushColumnsBackground/PushTableBackground for every Selectable..
		 const float backup_clip_rect_min_x = window->ClipRect.Min.x;
		 const float backup_clip_rect_max_x = window->ClipRect.Max.x;
		 if (span_all_columns)
		 {
		 	window->ClipRect.Min.x = window->ParentWorkRect.Min.x;
		 	window->ClipRect.Max.x = window->ParentWorkRect.Max.x;
		 }

		 bool item_add;
		 if (flags & ImGuiSelectableFlags_Disabled)
		 {
		 	ImGuiItemFlags backup_item_flags = g.CurrentItemFlags;
		 	g.CurrentItemFlags |= ImGuiItemFlags_Disabled | ImGuiItemFlags_NoNavDefaultFocus;
		 	item_add = ImGui::ItemAdd(bb, id);
		 	g.CurrentItemFlags = backup_item_flags;
		 }
		 else
		 {
		 	item_add = ImGui::ItemAdd(bb, id);
		 }

		 if (span_all_columns)
		 {
		 	window->ClipRect.Min.x = backup_clip_rect_min_x;
		 	window->ClipRect.Max.x = backup_clip_rect_max_x;
		 }

		 if (!item_add)
		 	return false;

		 // FIXME: We can standardize the behavior of those two, we could also keep the fast path of override ClipRect + full push on render only,
		 // which would be advantageous since most selectable are not selected.
		 if (span_all_columns && window->DC.CurrentColumns)
			 ImGui::PushColumnsBackground();
		 else if (span_all_columns && g.CurrentTable)
			 ImGui::TablePushBackgroundChannel();

		 // We use NoHoldingActiveID on menus so user can click and _hold_ on a menu then drag to browse child entries
		 ImGuiButtonFlags button_flags = 0;
		 if (flags & ImGuiSelectableFlags_NoHoldingActiveID) { button_flags |= ImGuiButtonFlags_NoHoldingActiveId; }
		 if (flags & ImGuiSelectableFlags_SelectOnClick) { button_flags |= ImGuiButtonFlags_PressedOnClick; }
		 if (flags & ImGuiSelectableFlags_SelectOnRelease) { button_flags |= ImGuiButtonFlags_PressedOnRelease; }
		 if (flags & ImGuiSelectableFlags_Disabled) { button_flags |= ImGuiButtonFlags_Disabled; }
		 if (flags & ImGuiSelectableFlags_AllowDoubleClick) { button_flags |= ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnDoubleClick; }
		 if (flags & ImGuiSelectableFlags_AllowItemOverlap) { button_flags |= ImGuiButtonFlags_AllowItemOverlap; }

		 if (flags & ImGuiSelectableFlags_Disabled)
		 	selected = false;

		 const bool was_selected = selected;
		 bool hovered, held;
		 bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, button_flags);

		 // Update NavId when clicking or when Hovering (this doesn't happen on most widgets), so navigation can be resumed with gamepad/keyboard
		 if (pressed || (hovered && (flags & ImGuiSelectableFlags_SetNavIdOnHover)))
		 {
		 	if (!g.NavDisableMouseHover && g.NavWindow == window && g.NavLayer == window->DC.NavLayerCurrent)
		 	{
				ImGui::SetNavID(id, window->DC.NavLayerCurrent, window->DC.NavFocusScopeIdCurrent, ImRect({ bb.Min.x - window->Pos.x, bb.Min.y - window->Pos.y }, { bb.Max.x - window->Pos.x, bb.Max.y - window->Pos.y }));
		 		g.NavDisableHighlight = true;
		 	}
		 }
		 if (pressed)
			 ImGui::MarkItemEdited(id);

		 if (flags & ImGuiSelectableFlags_AllowItemOverlap)
			 ImGui::SetItemAllowOverlap();

		 // In this branch, Selectable() cannot toggle the selection so this will never trigger.
		 if (selected != was_selected) //-V547
		 	window->DC.LastItemStatusFlags |= ImGuiItemStatusFlags_ToggledSelection;

		 // Render
		 if (held && (flags & ImGuiSelectableFlags_DrawHoveredWhenHeld))
		 	hovered = true;
		 if (hovered || selected)
		 {
		 	const ImU32 col = ImGui::GetColorU32((held && hovered) ? ImGuiCol_HeaderActive : hovered ? ImGuiCol_HeaderHovered : ImGuiCol_Header);
			ImGui::RenderFrame(bb.Min, bb.Max, col, false, 0.0f);
			ImGui::RenderNavHighlight(bb, id, ImGuiNavHighlightFlags_TypeThin | ImGuiNavHighlightFlags_NoRounding);
		 }

		 if (span_all_columns && window->DC.CurrentColumns)
			 ImGui::PopColumnsBackground();
		 else if (span_all_columns && g.CurrentTable)
			 ImGui::TablePopBackgroundChannel();
		 
		 if (flags & ImGuiSelectableFlags_Disabled) ImGui::PushStyleColor(ImGuiCol_Text, style.Colors[ImGuiCol_TextDisabled]);
		 ImGui::GetWindowDrawList()->AddImage(tex, bb.Min, { bb.Min.x + (bb.Max.x * 0.2f), bb.Max.y });

		 const ImU32 col = ImGui::GetColorU32({ 0.5f,0.5f,0.5f,0.5f });
		 ImGui::GetWindowDrawList()->AddLine({ bb.Min.x + (bb.Max.x * 0.21f), bb.Min.y }, { bb.Min.x + (bb.Max.x * 0.21f), bb.Max.y }, col,2.f);
		 ImGui::PushFont(FONT_BODY);
		 ImGui::RenderTextClipped(text_min, text_max, name, NULL, &label_size, style.SelectableTextAlign, &bb);
		 ImGui::PopFont();
		 ImGui::RenderTextClipped(text_min2, text_max, date, NULL, &label_size, style.SelectableTextAlign, &bb);
		 ImGui::RenderTextClipped(text_min3, text_max, unitData, NULL, &label_size, style.SelectableTextAlign, &bb);
		 if (flags & ImGuiSelectableFlags_Disabled) ImGui::PopStyleColor();

		 // Automatically close popups
		 if (pressed && (window->Flags & ImGuiWindowFlags_Popup) && !(flags & ImGuiSelectableFlags_DontClosePopups) && !(g.CurrentItemFlags & ImGuiItemFlags_SelectableDontClosePopup))
			 ImGui::CloseCurrentPopup();

		 IMGUI_TEST_ENGINE_ITEM_INFO(id, label, window->DC.LastItemStatusFlags);
		 return pressed;
		
	}
	bool UIMapSelectable(ImTextureID tex, const char* name, const char* date, const char* unitData, bool* p_selected, ImGuiSelectableFlags flags, const ImVec2& size_arg)
	{
		if (UIMapSelectable(tex, name, date, unitData, *p_selected, flags, size_arg))
		{
			*p_selected = !*p_selected;
			return true;
		}
		return false;
	}
	
}

