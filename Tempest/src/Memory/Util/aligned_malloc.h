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

namespace Tempest
{
    void* aligned_malloc(size_t required_bytes, size_t alignment)
    {
        void* p1; // original block
        void** p2; // aligned block
        size_t offset = alignment - 1 + sizeof(void*);
        if ((p1 = (void*)malloc(required_bytes + offset)) == NULL)
        {
            return NULL;
        }
        p2 = (void**)(((size_t)(p1)+offset) & ~(alignment - 1));
        p2[-1] = p1;
        return p2;
    }

    void aligned_free(void* p)
    {
        free(((void**)p)[-1]);
    }
}