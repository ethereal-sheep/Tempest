#pragma once
#include "../Resource.h"

/**
 * @brief string that causes mem leaks
 */
class lousy_string_2
{
	using allocator_type = std::pmr::polymorphic_allocator<std::byte>;

	allocator_type m_allocator;
	size_t m_length;
	char* m_buffer;

public:
	lousy_string_2(const char* c_str, allocator_type allocator)
		: m_allocator{ allocator },
		m_length{ std::strlen(c_str) },
		m_buffer{ (char*)m_allocator.resource()->allocate(m_length + 1, 1) }
	{
		std::strcpy(m_buffer, c_str);
	}
	//~lousyString() // testing without destructor
	//{
	//	m_allocator.resource()->deallocate(m_buffer, m_length, 1);
	//}
};