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
#include "Core.h"
#include "Util.h"
#include "px_include.h"

namespace Tempest
{
    /**
     * @brief Used for PhysX. Error callback function. Logs whatever error
     * PhysX returns to us as an error message.
     */
    class px_err_callback_t : public physx::PxErrorCallback
    {
    public:
        // not my fault, and I don't want to suppress
        void reportError(
            physx::PxErrorCode::Enum code, 
            const char* message, 
            const char* file,
            int line) override
        {
            LOG_ERROR("PhysX Err {0} Occured: {1}, {2}, {3}", code, message, file, line);
        }
    };

    /**
     * @brief Used for PhysX. Error callback function. Logs whatever error
     * PhysX returns to us as an error message.
     */
    inline px_err_callback_t& px_err_callback()
    {
        static px_err_callback_t callback;
        return callback;
    }
}