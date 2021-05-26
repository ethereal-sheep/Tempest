#pragma once
#include "Core.h"
#include "Resource.h"

namespace Tempest
{
	enum class ResourceStrategy
	{
		DEFAULT, /* Using std::pmr::get_default_resource() as primary resource. */
		NEW_DELETE, /* Using New/Delete resource strategy to allocate/deallocate memory. */
		MALLOC, /* Using C-style malloc resource strategy to allocate/deallocate memory. */
		MONOTONIC, /* Using std::pmr::monotonic_buffer_resource() as primary resource. Specify block size within MemoryStrategy block_size member. Default size : 1000000 bytes*/
		UNSYNC_POOL, /* Using std::pmr::unsynchronize_pool_resource() as primary resource. */
		SYNC_POOL /* Using std::pmr::synchronize_pool_resource() as primary resource. Thread-safe but slower than UNSYNC_POOL and MONOTONIC strategies. */
	};

	enum class DebugFlag
	{
		NONE, /* No Debugger Resource. */
		DEBUG_NON_VERBOSE, /* Only warnings are logged. */
		DEBUG_STRICT, /* Statistics are logged upon memory destuction. Warnings are logged as well. */
		DEBUG_VERBOSE /* All messages are logged. ***WARNING: VERY SLOW!*** */
	};


	/**
	 * @brief Defines the memory strategy used by the MemoryObject
	 * or MemoryManager. 
	 * @param debug Specifies the type of debug settings. *See DebugFlag enum*
	 * @param resource_strategy Specifies resource strategy. *See ResourceStrategy
	 * enum*
	 * @param block_size Used in MONOTONIC strategy only. Defines block size.
	 */
	struct MemoryStrategy
	{
		DebugFlag debug = DebugFlag::NONE;
		ResourceStrategy resource_strategy;
		size_t block_size;

		MemoryStrategy(DebugFlag debug_f = DebugFlag::NONE, ResourceStrategy strategy = ResourceStrategy::DEFAULT, size_t block_s = (size_t)1e6)
			: debug(debug_f), resource_strategy(strategy), block_size(block_s) {}

	};

	/**
	 * @brief A MemoryObject that receives a strategy and allocates
	 * appropriate memory following the strategy.
	 */
	class MemoryObject
	{
	public:
		MemoryObject(const MemoryObject&) = delete;
		~MemoryObject();
		MemoryObject(MemoryStrategy strategy = {});

		/**
		 * @brief Returns the underlying memory resource constructed by the
		 * object.
		 */
		[[nodiscard]] m_resource* get();
		/**
		 * @brief Returns the underlying debug resource if it exists. Returns
		 * nullptr if it doesn't.
		 */
		[[nodiscard]] const debug_resource* get_debug() const;

		/**
		 * @brief Return true if underlying debug resource exists.
		 */
		[[nodiscard]] bool has_debug() const;

		/**
		 * @brief Gets underlying MemoryStrategy.
		 */
		[[nodiscard]] const MemoryStrategy& get_strategy() const;
	
	private:
		bool debug_flag = false;
		MemoryStrategy strat;
		tuptr<debug_resource> debug_memory_wrapper;
		tuptr<m_resource> memory;
	};
}