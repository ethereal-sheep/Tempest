/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

namespace Tempest
{
	struct quitter
	{
		bool quit = false;
	};

	inline quitter& get_quitter()
	{
		static quitter q;
		return q;
	}
}