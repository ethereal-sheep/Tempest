
#include "debug_resource.h"

namespace Tempest
{
	using size_type = size_t;

	// Keep track of number of bytes that would be leaked by
	// allocator destructor
	std::atomic_size_t debug_resource::s_leaked_bytes(0);

	// Keep track of number of blocks that would be leaked by
	// allocator destructor 
	std::atomic_size_t debug_resource::s_leaked_blocks(0);

	// magic number identifying memory allocated by this resource
	static constexpr size_t allocatedMemoryPattern = 0xCAFEF00D;

	// 2nd magic number written over other magic number upon deallocation
	static constexpr size_t deallocatedMemoryPattern = 0xDEADC0DE;

	static const std::byte scribbledMemoryByte{ 0xA1 }; // byte used to scribble
	// deallocated memory

	static const std::byte paddedMemoryByte{ 0xC6 }; // byte used to write over
	// newly-allocated memory and
	// padding

	static constexpr size_t paddingSize = alignof(max_align_t);

	struct alignas(std::max_align_t) Padding
	{
		// This struct just make 'Header' readable.
		std::byte m_padding[paddingSize];
	};


	struct Header
	{
		size_t m_magic_number;
		Padding m_padding;
	};

	union AlignedHeader
	{
		// Maximally-aligned raw buffer big enough for a Header.

		Header m_object;
		max_align_t m_alignment;
	};

	constexpr size_t blockSize(size_t bytes) { return sizeof(Header) + bytes + paddingSize; }


	debug_resource::debug_resource(
		std::string name,
		m_resource* upstream)
		: m_name{ name },
		m_blocks(upstream),
		m_upstream(upstream)
	{}

	debug_resource::~debug_resource()
	{
		// If any blocks have not been released, report them as leaked
		s_leaked_blocks += blocks_outstanding();

		// Reclaim blocks that would have been leaked
		for (auto& record : m_blocks)
		{
			// logging here
			AlignedHeader* head = static_cast<AlignedHeader*>(record.m_ptr) - 1;

			std::cout << "*** Memory Leak at 0x" << record.m_ptr << ": " << record.m_bytes << " bytes leaked ***" <<
				std::endl;
			s_leaked_bytes += record.m_bytes;
			m_upstream->deallocate(head, sizeof(AlignedHeader) + record.m_bytes + paddingSize,
				record.m_alignment);
		}
	}

	void* debug_resource::do_allocate(size_t bytes, size_t alignment)
	{
		if (alignment > alignof(max_align_t))
		{
			// Over-aligned allocations not supported.
			throw std::bad_alloc();
		}
		// allocates extra bytes for header and padding
		AlignedHeader* head = static_cast<AlignedHeader*>(m_upstream->allocate(
			sizeof(AlignedHeader) + bytes + paddingSize, alignment));

		void* user = head + 1;

		std::memset((char*)(head + 1) - paddingSize,
			std::to_integer<unsigned char>(paddedMemoryByte), paddingSize);
		std::memset((char*)(head + 1) + bytes,
			std::to_integer<unsigned char>(paddedMemoryByte), paddingSize);

		head->m_object.m_magic_number = allocatedMemoryPattern;


		// record the allocation
		m_blocks.push_back(allocation_rec{ user, m_blocks_allocated++, bytes, alignment });

		// increments
		m_bytes_allocated += bytes;
		m_bytes_outstanding += bytes;
		if (m_bytes_outstanding > m_bytes_highwater)
			m_bytes_highwater.store(m_bytes_outstanding);

		// set history
		m_last_allocated_num_bytes = bytes;
		m_last_allocated_alignment = alignment;
		m_last_allocated_address = user;

		if (m_verbose_flag)
		{
			//logging ()
			std::cout << m_name << "[" << (m_blocks_allocated - 1) << "]"
				<< ": Allocating " << bytes << " bytes at 0x" << user << std::endl;
		}

		return user;
	}

	void debug_resource::do_deallocate(void* ptr, size_t bytes, size_t alignment)
	{
		//deallocating nullptr
		if (ptr == nullptr)
		{
			// display error?
			++m_bad_deallocations;
			return;
		}
		AlignedHeader* head = static_cast<AlignedHeader*>(ptr) - 1;

		// find allocation record of ptr
		auto i = std::find_if(m_blocks.begin(), m_blocks.end(),
			[ptr](const allocation_rec& r)
			{
				return r.m_ptr == ptr;
			});

		if (i == m_blocks.end())
			throw std::invalid_argument("Deallocating invalid pointer"); // check if block exist
		if (i->m_bytes != bytes)
			throw std::invalid_argument("Deallocating pointer size mismatch");
		if (i->m_alignment != alignment)
			throw std::invalid_argument("Deallocating pointer alignment mismatch");

		bool miscError = false;

		if (allocatedMemoryPattern != head->m_object.m_magic_number)
		{
			miscError = true;
		}


		int overrunBy = 0;
		int underrunBy = 0;

		if (!miscError)
		{
			std::byte* pcBegin;
			std::byte* pcEnd;

			// Check the padding before the segment.  Go backwards so we will
			// report the trashed byte nearest the segment.
			pcBegin = static_cast<std::byte*>(ptr) - 1;
			pcEnd = (std::byte*)&head->m_object.m_padding;

			for (std::byte* pc = pcBegin; pcEnd <= pc; --pc)
			{
				if (paddedMemoryByte != *pc)
				{
					underrunBy = static_cast<int>(pcBegin + 1 - pc);
					break;
				}
			}

			if (!underrunBy)
			{
				// Check the padding after the segment.
				std::byte* tail = static_cast<std::byte*>(ptr) + i->m_bytes;
				pcBegin = tail;
				pcEnd = tail + paddingSize;
				for (std::byte* pc = pcBegin; pc < pcEnd; ++pc)
				{
					if (paddedMemoryByte != *pc)
					{
						overrunBy = static_cast<int>(pc + 1 - pcBegin);
						break;
					}
				}
			}
		}

		if (miscError)
		{
			++m_misc_errors;
			//logging
			// doesn't currently work as a double delete is at basic checking caught earlier 
			if (deallocatedMemoryPattern == head->m_object.m_magic_number)
			{
				std::cout << "*** Deallocating already deallocated memory at " << ptr << " ***" << std::endl;

				++m_bad_deallocations;
				return;
			}
			std::cout << "*** Corrupted Memory at 0x" << ptr << ": Invalid Magic Number "
					<< std::hex << head->m_object.m_magic_number << " ***" << std::endl;
		}
		if (overrunBy || underrunBy)
		{
			++m_misc_errors;
			//logging
			std::cout << "*** Corrupted Memory at 0x" << ptr << ": Buffer " <<
				((underrunBy) ? "underrun" : "overrun") << " by "
				<< overrunBy + underrunBy << " ***" << std::endl;
		}

		if (m_verbose_flag)
		{
			//logging
			std::cout << m_name << "[" << (i->m_index) << "]"
				<< ": Deallocating " << bytes << " bytes at 0x" << ptr << std::endl;
		}

		// set as dead
		head->m_object.m_magic_number = deallocatedMemoryPattern;

		// set history
		m_last_deallocated_num_bytes = bytes;
		m_last_deallocated_alignment = alignment;
		m_last_deallocated_address = ptr;

		m_upstream->deallocate(head, sizeof(AlignedHeader) + i->m_bytes + paddingSize, i->m_alignment);
		m_blocks.erase(i);
		m_bytes_outstanding -= bytes;
	}

	bool debug_resource::do_is_equal(const memory_resource& other) const noexcept
	{
		return this == &other;
	}
}


