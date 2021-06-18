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
    static px_err_callback_t& px_err_callback()
    {
        static px_err_callback_t callback;
        return callback;
    }
}