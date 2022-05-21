#version 460
/**********************************************************************************
* \author		Linus Ng Hao Xuan (haoxuanlinus.ng@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
// Input ----------------------------------------------------------------------
in vec4 fragmentColor;

// Output ---------------------------------------------------------------------
out vec4 color;

// Functions ------------------------------------------------------------------
void main()
{
	color = fragmentColor;
}
