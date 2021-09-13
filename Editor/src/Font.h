#pragma once
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



//static const char* default_font = "FiraMono-Regular.ttf";
//static const char* bold_font = "FiraMono-Bold.ttf";

// make sure these files are in Resource/Fonts/
static const char* heavy_font = "contax.ttf";
static const char* body_font = "Montserrat-Regular.ttf";

static const float global_font_scale = 5.f / 6.f;
static const float global_icon_scale = 13.f / 15.f;

static const float header_text_size = 48.f;
static const float subheader_text_size = 36.f;
static const float button_text_size = 24.f;
static const float subbutton_text_size = 18.f;

static const float body_text_size = 24.f;
static const float para_text_size = 18.f;

static const float font_text_size = 24.f;
static const float font_icon_size = 18.f;


inline auto font_para() { return ImGui::GetIO().Fonts->Fonts[0]; } // para
inline auto font_body() { return ImGui::GetIO().Fonts->Fonts[1]; } // body
inline auto font_header() { return ImGui::GetIO().Fonts->Fonts[2]; } // header
inline auto font_subheader() { return ImGui::GetIO().Fonts->Fonts[3]; } // subheader
inline auto font_button() { return ImGui::GetIO().Fonts->Fonts[4]; } // button
inline auto font_subbutton() { return ImGui::GetIO().Fonts->Fonts[5]; } // subbutton



#define FONT_PARA		font_para()
#define FONT_BODY		font_body()
#define FONT_HEAD		font_header()
#define FONT_SHEAD		font_subheader()
#define FONT_BTN		font_button()
#define FONT_SBTN		font_subbutton()

#define FONT_DEFAULT	FONT_PARA
#define FONT_BOLD		FONT_BODY

namespace Tempest
{
	
}