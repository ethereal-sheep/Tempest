#pragma once
#include "internal.h"

namespace Tempest
{
	/**
	 * @brief Link object to connect two pins between two nodes
	 */




	//class link
	//{
	//public:
	//	link(pin_id_t _start, pin_id_t _end) : 
	//		id{ idgen::generate() }, start{ _start }, end{_end} {}

	//	link(const link&) = delete;
	//	link& operator=(const link&) = delete;
	//	link(link&&) = default;
	//	link& operator=(link&&) = default;

	//	/**
	//	 * @brief Serializing function. Writes to the writer object.
	//	 */
	//	Writer& serialize(Writer& writer) const
	//	{
	//		writer.StartObject();
	//		writer.Member("Start", start);
	//		writer.Member("End", end);
	//		return writer.EndObject();
	//	}

	//	/**
	//	 * @brief Gets link id.
	//	 */
	//	link_id_t get_id() const { return id; }
	//	/**
	//	 * @brief Gets id of start pin.
	//	 */
	//	pin_id_t get_start() const { return start; }
	//	/**
	//	 * @brief Gets id of end pin.
	//	 */
	//	pin_id_t get_end() const { return end; }

	//	operator link_id_t() const { return id; }


	//private:
	//	link_id_t id;
	//	pin_id_t start;
	//	pin_id_t end;
	//};
}