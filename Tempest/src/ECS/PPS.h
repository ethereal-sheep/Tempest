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

#include "Util/type_traits.h"
#include "Util/runtime_view.h"

#include "Util.h"
#include "ECS/Util/prototype_container.h"
#include "ECS/Prototypes/Prototype_Category.h"

namespace Tempest
{
	static const char* default_folder = "Components";


	class PPS_exception : public std::exception
	{
	public:
		PPS_exception(const string& err_msg = "PPS exception thrown!") : msg(err_msg) {}
		const char* what() const noexcept override { return msg.c_str(); }
	private:
		string msg;
	};

	/**
	 * @brief Prototype Prefab System to manage all prototypes and prefabs
	 * in a certain instance.
	 * Handles registering of components, creation/destruction of entities,
	 * assigning components to entities etc.
	 */
	class PPS final
	{
		tmap<string, prototype_container> prototype_containers;
	public:
		/**
		 * @brief Constructs an PPS object
		 * @param mem Pointer to a polymorphic memory resource; default gets
		 * the default memory resource
		 * @param folder string folder of tempest resources folder
		 */
		PPS(m_resource* mem = std::pmr::get_default_resource(),
			string folder = "")
		{
			// create sets for each proto_category
			for (auto& [s, category] : get_prototype_categories())
			{
				prototype_containers[s];
			}


			// some prototypes folder

			// if folder doesn't exist, create nothing
			if (!std::filesystem::exists(folder)) return;


			// iterate through each folder
			// for each file in the folder, we try to deserialize
			for (auto file : std::filesystem::directory_iterator(folder))
			{
				// skip non-dir files
				if (!std::filesystem::is_directory(file.path())) continue;

				// skip folders that do not correspond to a category
				auto cat = file.path().stem().string();
				if (!prototype_containers.count(cat)) continue;


				// verify if the prototypes are okay
				tvector<string> to_erase;
				for (auto& [name, p] : prototype_containers[cat])
				{
					if (!verify(cat, p))
					{
						// set it up for removal
						to_erase.push_back(name);
						LOG_WARN("{0} does not fit the {1) category requirements! Removed from PPS.", name, cat);
					}
				}

				for (auto& s : to_erase)
					prototype_containers[cat].erase(s);

			}
		}

		PPS(const PPS& rhs) = default;
		PPS& operator=(const PPS& rhs) = default;
		PPS(PPS&&) noexcept = default;
		PPS& operator=(PPS&&) = default;


	private:

	};


}