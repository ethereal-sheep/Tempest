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
#include "px_include.h"

namespace
{
	class px_bp_callback : public physx::PxBroadPhaseCallback
	{
		/**
		\brief Out-of-bounds notification. TO DO IF NEEDED.

		This function is called when an object leaves the broad-phase.

		\param[in] shape	Shape that left the broad-phase bounds
		\param[in] actor	Owner actor
		*/
		void	onObjectOutOfBounds(physx::PxShape& shape, physx::PxActor& actor) override 
		{
		}

		/**
		\brief Out-of-bounds notification. TO DO IF NEEDED.

		This function is called when an aggregate leaves the broad-phase.

		\param[in] aggregate	Aggregate that left the broad-phase bounds
		*/
		void	onObjectOutOfBounds(physx::PxAggregate& aggregate) override 
		{
		}
	};
}