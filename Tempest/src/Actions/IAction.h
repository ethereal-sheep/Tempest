/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

namespace Tempest
{
	class Instance;

	struct IAction
	{
		virtual const char* Name() = 0;
		virtual void Undo(Instance&) = 0;
		virtual void Redo(Instance&) = 0;
		virtual ~IAction() = default;
	};

	using Action = IAction;
}