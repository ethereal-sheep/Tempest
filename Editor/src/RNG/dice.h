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
#include "Core.h"
#include "TMath.h"

namespace Tempest
{
	inline size_t dice(size_t dice_count = 1, size_t d = 6)
	{
		size_t sum = 0;
		for (size_t i = 0; i < dice_count; ++i)
			sum += els::random::uniform_rand<size_t>(1, d);
		return sum;
	}

	inline size_t d4(size_t dice_count = 1)
	{
		return dice(dice_count, 4);
	}
	inline size_t d6(size_t dice_count = 1)
	{
		return dice(dice_count, 6);
	}
	inline size_t d8(size_t dice_count = 1)
	{
		return dice(dice_count, 8);
	}
	inline size_t d10(size_t dice_count = 1)
	{
		return dice(dice_count, 10);
	}
	inline size_t d12(size_t dice_count = 1)
	{
		return dice(dice_count, 12);
	}
	inline size_t d20(size_t dice_count = 1)
	{
		return dice(dice_count, 20);
	}
	inline size_t d100(size_t dice_count = 1)
	{
		return dice(dice_count, 100);
	}
}