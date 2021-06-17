#pragma once
#include "../Util/px_include.h"

// we can just inject into physx
namespace physx
{
    // no idea if we need to use this or not
    PxFilterFlags SampleFilterShader(
        PxFilterObjectAttributes attributes0, PxFilterData filterData0,
        PxFilterObjectAttributes attributes1, PxFilterData filterData1,
        PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
    {
        // let triggers through
        if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
        {
            pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
            return PxFilterFlag::eDEFAULT;
        }
        // generate contacts for all that were not filtered above
        pairFlags = PxPairFlag::eCONTACT_DEFAULT;

        // trigger the contact callback for pairs (A,B) where
        // the filtermask of A contains the ID of B and vice versa.
        if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
            pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;

        return PxFilterFlag::eDEFAULT;
    }

    
}
