#ifndef PROFILE_COLOR_H
#define PROFILE_COLOR_H

#define WHITE 0xffffffff
#define TEAL 0x6df7c1ff
#define BLUE 0x11adc1ff
#define GREY 0x606c81ff
#define DARK_PURPLE 0x393457ff
#define DARK_GREEN 0x1e8875ff
#define GREEN 0x5bb361ff
#define LIGHT_GREEN 0xa1e55aff
#define YELLOW 0xf7e476ff
#define ORANGE 0xf99252ff
#define LIGHT_RED 0xcb4d68ff
#define PURPLE 0x6a3771ff
#define RED 0xc92464ff
#define PINK 0xf48cb6ff
#define BEIGE 0xf7b69eff
#define FADED_GREEN 0x9b9c82ff

#define COLOR_ARRAY_SIZE 16

namespace Tempest::Profile
{
	static uint32_t color_array[COLOR_ARRAY_SIZE] =
	{
		WHITE,
		RED,
		GREEN,
		BLUE,
		YELLOW,
		ORANGE,
		PURPLE,
		LIGHT_GREEN,
		LIGHT_RED,
		TEAL,
		DARK_PURPLE,
		PINK,
		DARK_GREEN,
		BEIGE,
		FADED_GREEN,
		GREY,
	};

}


#endif