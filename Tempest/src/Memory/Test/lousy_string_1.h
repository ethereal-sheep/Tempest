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
#include "../Resource.h"

/**
 * @brief string that causes overflow
 */
class lousy_string_1
{
    using allocator_type = std::pmr::polymorphic_allocator<std::byte>;

    allocator_type m_allocator;
    size_t m_length;
    char* m_buffer;

public:
    lousy_string_1(const char* c_str, allocator_type allocator)
        : m_allocator{ allocator },
        m_length{ std::strlen(c_str) },
        m_buffer{ (char*)m_allocator.resource()->allocate(m_length, 1) }
        // intentionally allocating one less byte for null terminator
    {
        std::strcpy(m_buffer, c_str);
    }

    ~lousy_string_1()
    {
        m_allocator.resource()->deallocate(m_buffer, m_length, 1);
    }
};