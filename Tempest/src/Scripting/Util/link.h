#pragma once
#include "Core.h"

namespace Tempest
{
	using pin_id_t = id_t;
	using link_id_t = id_t;

	class link
	{
	public:
		link(link_id_t _id, pin_id_t _start, pin_id_t _end) : 
			id{ _id }, start{ _start }, end{_end} {}

		link(const link&) = delete;
		link& operator=(const link&) = delete;
		link(link&&) = delete;
		link& operator=(link&&) = delete;

		template <typename Archive>
		friend Archive& operator&(Archive& ar, link& link)
		{
			ar.StartObject();
			ar.Member("id", link.id);
			ar.Member("start", link.start);
			ar.Member("end", link.end);
			return ar.EndObject();
		}

		link_id_t get_id() const { return id; }
		pin_id_t get_start() const { return start; }
		pin_id_t get_end() const { return end; }

		operator link_id_t() const { return id; }

	private:
		link_id_t id;
		pin_id_t start;
		pin_id_t end;
	};
}