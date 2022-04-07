/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu),
*				Lim Ziyi Jean(ziyijean.lim@digipen.edu),
*				Huang Xurong(h.xurong@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include "Core.h"

#include "Fonts/IconsFontaudio.h"
#include "Fonts/IconsFontAwesome5.h"
#include "Fonts/IconsForkAwesome.h"
#include "Fonts/IconsKenney.h"

#include "Extern/imgui/imgui.h"
#include "Extern/imgui/imgui_internal.h"
#include "Extern/imgui/imgui_stdlib.h"
#include "Extern/imgui/ImFileDialog.h"
#include "Extern/imgui/implot.h"
#include "Extern/imgui/imgui_node_editor.h"
//#include "Extern/imgui/builders.h"
#include "Extern/imgui/widgets.h"
#include "Graphics/OpenGL/Texture.h"


//static const char* default_font = "FiraMono-Regular.ttf";
//static const char* bold_font = "FiraMono-Bold.ttf";

// make sure these files are in Resource/Fonts/
static const char* heavy_font = "contax.ttf";
static const char* body_font = "Montserrat-Regular.ttf";
static const char* open_font = "OpenSans-SemiBold.ttf";


static const float global_font_scale = 5.f / 6.f;
static const float global_icon_scale = 13.f / 15.f;

static const float open_text_size = 24.f;
static const float open_text_size30 = 30.f;
static const float open_text_turn_size = 36.f;

static const float header_text_size = 48.f;
static const float subheader_text_size = 36.f;
static const float button_text_size = 24.f;
static const float subbutton_text_size = 18.f;
static const float contaxPro_size144 = 144.f;


static const float body_text_size = 24.f;
static const float para_text_size = 18.f;

static const float font_text_size = 24.f;
static const float font_icon_size = 18.f;

static bool block_mouse_down = false;
static bool override_block_mouse_down = false;
static bool non_imgui_mouse_click = false;

inline auto font_para() { return ImGui::GetIO().Fonts->Fonts[0]; } // para
inline auto font_body() { return ImGui::GetIO().Fonts->Fonts[1]; } // body
inline auto font_header() { return ImGui::GetIO().Fonts->Fonts[2]; } // header
inline auto font_subheader() { return ImGui::GetIO().Fonts->Fonts[3]; } // subheader
inline auto font_button() { return ImGui::GetIO().Fonts->Fonts[4]; } // button
inline auto font_subbutton() { return ImGui::GetIO().Fonts->Fonts[5]; } // subbutton
inline auto font_open() { return ImGui::GetIO().Fonts->Fonts[6]; } // opensans
inline auto font_open_turn() { return ImGui::GetIO().Fonts->Fonts[7]; } // opensans for turn order
inline auto font_open_30() { return ImGui::GetIO().Fonts->Fonts[8]; } // opensans for size 30
inline auto font_contax_144() { return ImGui::GetIO().Fonts->Fonts[9]; } // ContaxPro for size 144



#define FONT_PARA		font_para()
#define FONT_BODY		font_body()
#define FONT_HEAD		font_header()
#define FONT_SHEAD		font_subheader()
#define FONT_BTN		font_button()
#define FONT_SBTN		font_subbutton()
#define FONT_OPEN		font_open()
#define FONT_TURN		font_open_turn()
#define FONT_OPEN_30	font_open_30()
#define FONT_CONTAX_144	font_contax_144()

#define FONT_DEFAULT	FONT_PARA
#define FONT_BOLD		FONT_BODY


namespace Tempest
{
	inline tmap<tpath, tsptr<Texture>, fs_hash> tex_map;

	inline fs::path get_user_path()
	{
		char* pValue;
		size_t len;
		[[maybe_unused]] errno_t err = _dupenv_s(&pValue, &len, "USERPROFILE");
		if (!pValue)
		{
			LOG_WARN("APPDATA environment variable could not be found!");
			return fs::path{};
		}

		tpath path{ pValue };
		free(pValue); // It's OK to call free with NULL
		path /= "Documents";
		path /= "CoReSys";
		return path;
	}

	bool check_if_mouse_within(const ImVec2 min, const ImVec2 max);

	void block_input_if_mouse_in_bounds(ImVec2 pos, ImVec2 size);

	void override_mouse_blocking(ImVec2 pos, ImVec2 size);

	void reset_blocking();

	bool check_mouse_blocking();

	bool NonImGuiMouseClick();
}