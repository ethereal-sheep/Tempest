/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#include "Events/Events/IEvent.h"

namespace Tempest
{
	template<size_t CharCode>
	struct OnKeyDownEvent : public Event {};

	template<size_t CharCode>
	struct OnKeyReleaseEvent : public Event {};

	template<size_t CharCode>
	struct OnKeyHoldEvent : public Event {};

	struct OnAnyKeyDownEvent : public Event {};
	struct OnAnyKeyReleaseEvent : public Event {};
	struct OnAnyKeyHoldEvent : public Event {};
}