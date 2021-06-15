#pragma once
#include "Core.h"
#include "Util.h"

namespace Tempest
{
    // remove next time
    namespace PxErrorCode
    {
        enum class Enum {};
    }

    class px_err_callback //: public PxErrorCallback
    {
    public:
        void reportError(PxErrorCode::Enum code, const char* message, const char* file,
            int line) // override
        {
            LOG_ERROR("PhysX Err {0} Occured: {1}, {2}, {3}", code, message, file, line);
        }
    };
}