#pragma once
#include "Core.h"
#include "Util.h"
#include "px_include.h"

namespace Tempest
{
    class px_err_callback_t : public physx::PxErrorCallback
    {
    public:
        void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file,
            int line) // override
        {
            LOG_ERROR("PhysX Err {0} Occured: {1}, {2}, {3}", code, message, file, line);
        }
    };

    static px_err_callback_t& px_err_callback()
    {
        static px_err_callback_t callback;
        return callback;
    }
}