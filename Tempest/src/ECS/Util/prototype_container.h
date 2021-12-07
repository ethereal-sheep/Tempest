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

#include "../../Memory.h"
#include "Util.h"
#include "prototype.h"

namespace Tempest
{
	class prototype_container_exception : public std::exception
	{
	public:
		prototype_container_exception(const string& err_msg = "prototype_container exception thrown!") : msg(err_msg) {}
		const char* what() const noexcept override { return msg.c_str(); }
	private:
		string msg;
	};

	// container for loading, editing, and saving
	// a set of prototypes
	class prototype_container final
	{
		tmap<string, prototype> prototypes;

	public:
		prototype_container() = default;

		prototype_container(const tpath& folder)
		{
			// if folder doesn't exist, create nothing
			if (!std::filesystem::exists(folder)) return;

			// iterate through folder
			// for each file in the folder, we try to deserialize
			for (auto file : std::filesystem::directory_iterator(folder))
			{
				// skip non-json files
				if (file.path().extension() != ".json") continue;

				try
				{
					prototypes.emplace(std::make_pair(file.path().stem().string(), prototype(file.path())));
				}
				catch (const std::exception& e)
				{
					LOG_WARN(e.what());
				}
			}
		}

		tpath save(const tpath& folder) const
		{
			for (auto& [s, proto] : prototypes)
			{
				proto.save(folder);
			}

			return folder;
		}

		prototype_container(const prototype_container&) = default;
		prototype_container(prototype_container&&) noexcept = default;
		prototype_container& operator=(const prototype_container&) = default;
		prototype_container& operator=(prototype_container&&) = default;

		bool has(const string& s) const
		{
			return prototypes.count(s);
		}

		prototype* emplace(prototype proto)
		{
			string name = proto.name;
			if (has(name)) {
				name = algo::get_next_name(name, prototypes);
				proto.name = name; // rename
			}

			prototypes.emplace(std::make_pair(name, std::move(proto)));
			return get_if(name);
		}

		prototype* emplace(const string& name)
		{
			prototypes.emplace(std::make_pair(name, prototype(name)));
			return get_if(name);
		}


		prototype* load_file(const tpath& file)
		{
			// if the stem exists, we do not add it in
			auto name = file.stem().string();
			if (has(name))
				return nullptr;

			prototypes.emplace(std::make_pair(name, prototype(file)));
			return get_if(name);
		}

		void erase(const string& name)
		{
			if (!has(name)) return;
			prototypes.erase(name);
		}
		template<typename Pred>
		void erase_if(Pred pred)
		{
			tvector<string> v;
			for (auto& [s, p] : prototypes)
			{
				if (pred(s))
					v.push_back(s);
			}

			for(auto& s : v)
				prototypes.erase(s);
		}

		prototype& get(const string& s)
		{
			// assume exist
			return prototypes.at(s);
		}

		prototype* get_if(const string& s)
		{
			if (!has(s))
				return nullptr;
			return &prototypes.at(s);
		}

		string rename(const string& old_n, const string& new_n)
		{
			if (!has(old_n)) return old_n; // fail

			auto next = algo::get_next_name(new_n, prototypes);

			auto node = prototypes.extract(old_n);
			node.key() = next;
			node.mapped().name = next;

			prototypes.insert(std::move(node));

			return next;
		}

		auto begin() const { return prototypes.begin(); }
		auto end() const { return prototypes.end(); }
		auto begin() { return prototypes.begin(); }
		auto end() { return prototypes.end(); }

	private:
	};
}