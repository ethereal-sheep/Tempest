#include "Font.h"

namespace Tempest
{

	bool check_if_mouse_within(const ImVec2 min, const ImVec2 max)
	{
		auto mousePos = ImGui::GetMousePos();
		if (mousePos.x > min.x && mousePos.x < max.x
			&& mousePos.y > min.y && mousePos.y < max.y)
			return true;


		return false;
	}

	void override_dt(float dt)
	{
		do_override_dt = true;
		overriden_dt = dt;
	}

	float get_overriden_dt()
	{
		return overriden_dt;
	}

	bool must_override_dt()
	{
		return do_override_dt;
	}

	void reset_dt()
	{
		do_override_dt = false;
	}

	void block_input_if_mouse_not_in_bounds(ImVec2 pos, ImVec2 size)
	{
		auto max = pos;
		max.x += size.x;
		max.y += size.y;
		if (!check_if_mouse_within(pos, max))
		{
			block_mouse_down = true;
		}
	}

	void override_mouse_blocking(ImVec2 pos, ImVec2 size)
	{
		auto max = pos;
		max.x += size.x;
		max.y += size.y;
		if (check_if_mouse_within(pos, max))
		{
			override_block_mouse_down = true;
		}
	}

	void reset_blocking()
	{
		block_mouse_down = false;
		override_block_mouse_down = false;
	}

	bool check_mouse_blocking()
	{
		return block_mouse_down && !override_block_mouse_down;
	}

	static bool non_imgui_mouse_click = false;

	void reset_non_imgui_mouseclick(bool b)
	{
		non_imgui_mouse_click = b;
	}

	bool NonImGuiMouseClick()
	{
		return non_imgui_mouse_click;
	}
}