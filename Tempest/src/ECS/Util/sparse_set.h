#pragma once

#include "../../Core.h"
#include <vector>

namespace Tempest
{
	template <typename T>
	class sparse_set
	{
		std::vector<T> sparse;
		std::vector<T> dense;

	protected:

		class sparse_set_iterator final {
			friend class sparse_set<T>;

			using packed_type = std::vector<T>;
			using index_type = id_t;

			sparse_set_iterator(const packed_type& ref, const index_type idx) ENTT_NOEXCEPT
				: packed{ &ref }, index{ idx }
			{}


	public:

		size_t size() const { return dense.size(); }
		bool empty() const { return dense.empty(); }
		void sort() {/*TODO*/ }
		void sort() {/*TODO*/ }
		void sort() {/*TODO*/ }
		void sort() {/*TODO*/ }
		void sort() {/*TODO*/ }
		void sort() {/*TODO*/ }


	};

	template<typename SysCall>
	void for_entities(SysCall process)
	{
		for (auto id : dense)
			process(id);
	}
}