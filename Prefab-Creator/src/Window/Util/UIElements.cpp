/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
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
		valueChangeOnEdit |= valueChangeOnEdit = ImGui::DragInt2(ID, v, v_speed, v_min, v_max, format, flags);
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
		ImGui::IsItemDeactivatedAfterEdit();
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

	void UniformScaleFloat2(const char* label, const char* ID, ImVec2 padding, bool* b, float v[2], float resetValue, float v_speed, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
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
		ImGui::Dummy({ padding.x - ImGui::GetItemRectSize().x, padding.y });
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

}

namespace ImGui
{

	// https://github.com/forrestthewoods/lib_fts

	// Forward declarations for "private" implementation
	namespace fuzzy_internal {
		static bool fuzzy_match_recursive(const char* pattern, const char* str, int& outScore, const char* strBegin,
			uint8_t const* srcMatches, uint8_t* newMatches, int maxMatches, int nextMatch,
			int& recursionCount, int recursionLimit);
	}
	// Private implementation
	static bool fuzzy_internal::fuzzy_match_recursive(const char* pattern, const char* str, int& outScore,
		const char* strBegin, uint8_t const* srcMatches, uint8_t* matches, int maxMatches,
		int nextMatch, int& recursionCount, int recursionLimit)
	{
		// Count recursions
		++recursionCount;
		if (recursionCount >= recursionLimit)
			return false;

		// Detect end of strings
		if (*pattern == '\0' || *str == '\0')
			return false;

		// Recursion params
		bool recursiveMatch = false;
		uint8_t bestRecursiveMatches[256];
		int bestRecursiveScore = 0;

		// Loop through pattern and str looking for a match
		bool first_match = true;
		while (*pattern != '\0' && *str != '\0') {

			// Found match
			if (tolower(*pattern) == tolower(*str)) {

				// Supplied matches buffer was too short
				if (nextMatch >= maxMatches)
					return false;

				// "Copy-on-Write" srcMatches into matches
				if (first_match && srcMatches) {
					memcpy(matches, srcMatches, nextMatch);
					first_match = false;
				}

				// Recursive call that "skips" this match
				uint8_t recursiveMatches[256];
				int recursiveScore;
				if (fuzzy_match_recursive(pattern, str + 1, recursiveScore, strBegin, matches, recursiveMatches, sizeof(recursiveMatches), nextMatch, recursionCount, recursionLimit)) {

					// Pick best recursive score
					if (!recursiveMatch || recursiveScore > bestRecursiveScore) {
						memcpy(bestRecursiveMatches, recursiveMatches, 256);
						bestRecursiveScore = recursiveScore;
					}
					recursiveMatch = true;
				}

				// Advance
				matches[nextMatch++] = (uint8_t)(str - strBegin);
				++pattern;
			}
			++str;
		}

		// Determine if full pattern was matched
		bool matched = *pattern == '\0' ? true : false;

		// Calculate score
		if (matched) {
			const int sequential_bonus = 15;            // bonus for adjacent matches
			const int separator_bonus = 30;             // bonus if match occurs after a separator
			const int camel_bonus = 30;                 // bonus if match is uppercase and prev is lower
			const int first_letter_bonus = 15;          // bonus if the first letter is matched

			const int leading_letter_penalty = -5;      // penalty applied for every letter in str before the first match
			const int max_leading_letter_penalty = -15; // maximum penalty for leading letters
			const int unmatched_letter_penalty = -1;    // penalty for every letter that doesn't matter

			// Iterate str to end
			while (*str != '\0')
				++str;

			// Initialize score
			outScore = 100;

			// Apply leading letter penalty
			int penalty = leading_letter_penalty * matches[0];
			if (penalty < max_leading_letter_penalty)
				penalty = max_leading_letter_penalty;
			outScore += penalty;

			// Apply unmatched penalty
			int unmatched = (int)(str - strBegin) - nextMatch;
			outScore += unmatched_letter_penalty * unmatched;

			// Apply ordering bonuses
			for (int i = 0; i < nextMatch; ++i) {
				uint8_t currIdx = matches[i];

				if (i > 0) {
					uint8_t prevIdx = matches[i - 1];

					// Sequential
					if (currIdx == (prevIdx + 1))
						outScore += sequential_bonus;
				}

				// Check for bonuses based on neighbor character value
				if (currIdx > 0) {
					// Camel case
					char neighbor = strBegin[currIdx - 1];
					char curr = strBegin[currIdx];
					if (::islower(neighbor) && ::isupper(curr))
						outScore += camel_bonus;

					// Separator
					bool neighborSeparator = neighbor == '_' || neighbor == ' ';
					if (neighborSeparator)
						outScore += separator_bonus;
				}
				else {
					// First letter
					outScore += first_letter_bonus;
				}
			}
		}

		// Return best result
		if (recursiveMatch && (!matched || bestRecursiveScore > outScore)) {
			// Recursive score is better than "this"
			memcpy(matches, bestRecursiveMatches, maxMatches);
			outScore = bestRecursiveScore;
			return true;
		}
		else if (matched) {
			// "this" score is better than recursive
			return true;
		}
		else {
			// no match
			return false;
		}
	}



	static bool fuzzy_match(char const* pattern, char const* str, int& outScore, uint8_t* matches, int maxMatches) {
		int recursionCount = 0;
		int recursionLimit = 10;

		return fuzzy_internal::fuzzy_match_recursive(pattern, str, outScore, str, nullptr, matches, maxMatches, 0, recursionCount, recursionLimit);
	}

	// Public interface
	bool fuzzy_match_simple(char const* pattern, char const* str) {
		while (*pattern != '\0' && *str != '\0') {
			if (tolower(*pattern) == tolower(*str))
				++pattern;
			++str;
		}

		return *pattern == '\0' ? true : false;
	}

	bool fuzzy_match(char const* pattern, char const* str, int& outScore) {

		uint8_t matches[256];
		return fuzzy_match(pattern, str, outScore, matches, sizeof(matches));
	}

	static bool sortbysec_desc(const std::pair<int, int>& a, const std::pair<int, int>& b)
	{
		return (b.second < a.second);
	}

	ImVec2 operator+(const ImVec2& rhs, const ImVec2& lhs)
	{
		return ImVec2{ rhs.x + lhs.x, rhs.y + lhs.y };
	}

	bool ComboWithFilter(const char* label, int* current_item, const std::vector<std::string>& items)
	{
		ImGuiContext& g = *GImGui;

		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems)
			return false;

		//const ImGuiStyle& style = g.Style;
		auto items_count = items.size();

		// Call the getter to obtain the preview string which is a parameter to BeginCombo()
		const char* preview_value = NULL;
		if (*current_item >= 0 && *current_item < items_count)
			preview_value = items[*current_item].c_str();

		static char pattern_buffer[256] = { 0 };
		bool isNeedFilter = false;

		char comboButtonName[512] = { 0 };
		ImFormatString(comboButtonName, IM_ARRAYSIZE(comboButtonName), "%s##name_ComboWithFilter_button_%s", preview_value ? preview_value : "", label);

		char name_popup[256 + 10];
		ImFormatString(name_popup, IM_ARRAYSIZE(name_popup), "##name_popup_%s", label);

		// Display items
		// FIXME-OPT: Use clipper (but we need to disable it on the appearing frame to make sure our call to SetItemDefaultFocus() is processed)
		bool value_changed = false;

		const float expected_w = CalcItemWidth();
		ImVec2 item_min = GetItemRectMin();
		bool isNewOpen = false;
		float sz = GetFrameHeight();
		ImVec2 size(sz, sz);
		ImVec2 CursorPos = window->DC.CursorPos;
		ImVec2 pos = CursorPos + ImVec2(expected_w - sz, 0);
		const ImRect bb(pos, pos + size);

		float ButtonTextAlignX = g.Style.ButtonTextAlign.x;
		g.Style.ButtonTextAlign.x = 0;
		if (ImGui::Button(comboButtonName, ImVec2(expected_w, 0)))
		{
			ImGui::OpenPopup(name_popup);
			isNewOpen = true;
		}
		g.Style.ButtonTextAlign.x = ButtonTextAlignX;
		//bool hovered = IsItemHovered();
		//bool active = IsItemActivated();
		// bool pressed = IsItemClicked();

		// Render
		//const ImU32 bg_col = GetColorU32((active && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
		//RenderFrame(bb.Min, bb.Max, bg_col, true, g.Style.FrameRounding);
		const ImU32 text_col = GetColorU32(ImGuiCol_Text);
		RenderArrow(window->DrawList, bb.Min + ImVec2(ImMax(0.0f, (size.x - g.FontSize) * 0.5f), ImMax(0.0f, (size.y - g.FontSize) * 0.5f)), text_col, ImGuiDir_Down);

		if (isNewOpen)
		{
			memset(pattern_buffer, 0, IM_ARRAYSIZE(pattern_buffer));
		}
		ImVec2 item_max = GetItemRectMax();
		SetNextWindowPos({ CursorPos.x, item_max.y });
		ImGui::SetNextWindowSize({ ImGui::GetItemRectSize().x, 0 });
		if (ImGui::BeginPopup(name_popup))
		{
			ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor(240, 240, 240, 255));
			ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor(0, 0, 0, 255));
			ImGui::PushItemWidth(-FLT_MIN);
			// Filter input
			if (isNewOpen)
				ImGui::SetKeyboardFocusHere();
			InputText("##ComboWithFilter_inputText", pattern_buffer, 256);

			// Search Icon, you can use it if you load IconsFontAwesome5 https://github.com/juliettef/IconFontCppHeaders
			//const ImVec2 label_size = CalcTextSize(ICON_FA_SEARCH, NULL, true);
			//const ImVec2 search_icon_pos(ImGui::GetItemRectMax().x - label_size.x - style.ItemInnerSpacing.x * 2, window->DC.CursorPos.y + style.FramePadding.y + g.FontSize * 0.1f);
			//RenderText(search_icon_pos, ICON_FA_SEARCH);

			ImGui::PopStyleColor(2);
			if (pattern_buffer[0] != '\0')
			{
				isNeedFilter = true;
			}

			std::vector<std::pair<int, int> > itemScoreVector;
			if (isNeedFilter)
			{
				for (int i = 0; i < items_count; i++)
				{
					int score = 0;
					bool matched = fuzzy_match(pattern_buffer, items[i].c_str(), score);
					if (matched)
						itemScoreVector.push_back(std::make_pair(i, score));
				}
				std::sort(itemScoreVector.begin(), itemScoreVector.end(), sortbysec_desc);
			}

			auto show_count = isNeedFilter ? itemScoreVector.size() : items_count;
			if (ImGui::ListBoxHeader("##ComboWithFilter_itemList", (int)show_count))
			{
				for (int i = 0; i < show_count; i++)
				{
					int idx = isNeedFilter ? itemScoreVector[i].first : i;
					PushID((void*)(intptr_t)idx);
					const bool item_selected = (idx == *current_item);
					const char* item_text = items[idx].c_str();
					if (Selectable(item_text, item_selected))
					{
						value_changed = true;
						*current_item = idx;
						CloseCurrentPopup();
					}
					if (item_selected)
						SetItemDefaultFocus();
					PopID();
				}
				ImGui::ListBoxFooter();
			}
			ImGui::PopItemWidth();
			ImGui::EndPopup();
		}
		if (value_changed)
			MarkItemEdited(g.CurrentWindow->DC.LastItemId);

		return value_changed;
	}
}