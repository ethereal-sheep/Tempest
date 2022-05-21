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

	class IWindow
	{
	public:
		virtual ~IWindow() {};

		virtual const char* window_name() = 0;

		// called by window_manager
		virtual void init(Instance&) {};

		// called by window_manager
		virtual void exit(Instance&) {};

		// called by window_manager
		virtual void show(Instance&) = 0;

	protected:
		int window_flags = 0;

	public:
		bool visible = true;
	};

	using Window = IWindow;
}