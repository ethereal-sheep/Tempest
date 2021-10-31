/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#include "../../Memory.h"
#include "../Entity.h"
#include "ECS/Components/Components.h"
#include "Util.h"
#include "prefab.h"
#include "coptional.h"

namespace Tempest
{
	class prototype_exception : public std::exception
	{
	public:
		prototype_exception(const string& err_msg = "prototype exception thrown!") : msg(err_msg) {}
		const char* what() const noexcept override { return msg.c_str(); }
	private:
		string msg;
	};
	// tech defines prototype category
	// designer creates prototype from prototype category
	class prototype final
	{
	public:
		string cat = "";
		string name = "Prototype";
		static const bool is_entity_keyed = false;

		prototype(const string& c = "Wall", const string& s = "Prototype") : cat{ c }, name { s }
		{
			// empty prototype
		}

		prototype(const tpath& file) : cat{ file.parent_path().stem().string() }, name { file.stem().string() }
		{
			// throw if fail
			Serializer serializer;
			string json = serializer.GetJson(file);

			Reader reader(json.c_str());
			if (reader.HasError())
				throw prototype_exception(file.string() + " cannot be opened!");

			reader.StartObject();

			// Meta Info
			string proto;
			reader.StartMeta();
			reader.Member("Type", proto);
			reader.Member("Category", cat);
			reader.EndMeta();

			if (proto != "Prototype")
				throw prototype_exception(file.string() + " is corrupted!");


			size_t size;
			reader.StartArray("Components", &size);
			for (size_t i = 0; i < size; ++i)
			{
				reader.StartObject();

				string typeinfo;
				reader.StartMeta();
				reader.Member("Type", typeinfo);
				reader.EndMeta();

				auto type = magic_enum::enum_cast<ComponentType>(typeinfo);
				if (!type.has_value())
					throw prototype_exception(string() + "Component of type " + typeinfo + " cannot be found!");

				deserialize_helper(reader, *this, *type);

				reader.EndObject();
			}

			reader.EndArray();

			reader.EndObject();
		}

		const string& get_name() const {
			return name;
		}

		tpath save(const tpath& folder) const
		{
			// if directory doesn't exist, create new_directory
			if (!std::filesystem::exists(folder))
				std::filesystem::create_directories(folder);

			// delete existing

			Writer writer;

			writer.StartObject();
			writer.StartMeta();
			writer.Member("Type", "Prototype");
			writer.Member("Category", cat);
			writer.EndMeta();


			writer.StartArray("Components");
			for (auto& [hash, ptr] : components)
			{
				writer.StartObject();
				writer.StartMeta();
				writer.Member("Type", ptr->type_info);
				writer.EndMeta();

				ptr->serialize(writer);

				writer.EndObject();
			}

			writer.EndArray();

			writer.EndObject();



			tpath target = folder;
			if (fs::is_directory(target))
				target /= (name + ".json");

			Serializer::SaveJson(target, writer.GetString());
			return target;
		}

		prototype(const prototype& rhs)
		{
			*this = rhs;
		}

		prototype& operator=(const prototype& rhs)
		{
			name = rhs.name;
			cat = rhs.cat;
			components.clear();
			for (auto& [hash, ptr] : rhs.components)
			{
				components[hash] = ptr->clone();
			}
			return *this;
		}

		prototype(prototype&&) noexcept = default;
		prototype& operator=(prototype&&) = default;

		template<typename Component>
		bool has() const
		{
			return components.count(t_hash<Component>());
		}

		size_t size() const
		{
			return components.size();
		}

		template<typename Component>
		Component& get()
		{
			return static_cast<coptional<Component>*>(
				components.at(t_hash<Component>()).get())->get();
		}

		template<typename Component>
		Component* get_if()
		{
			if (!has<Component>())
				return nullptr;

			return &get<Component>();

		}

		template <typename Component>
		Component* emplace()
		{
			if(has<Component>())
				return nullptr;

			components.emplace(
				make_tpair(
					t_hash<Component>(), 
					make_uptr<coptional<Component>>()));

			return get_if<Component>();
		}

		template<typename Component>
		void erase()
		{
			if (!has<Component>()) return;
			components.erase(t_hash<Component>());
		}

		// create an instance of this 
		prefab instance() const
		{
			// create a prefab with an instance of this
			prefab p;
			p.proto = name;
			p.cat = cat;
			for (auto& [hash, ptr] : components)
				p.components[hash] = ptr->instance();
			return p;
		}

		

		template <typename Archiver>
		friend Archiver& operator&(Archiver& ar, prototype& p)
		{
			ar.StartObject();
			/*ar.Member("Weapon_Name", component.name);
			ar.Member("Main_Stat", component.main_stat);
			ar.Vector("Weapon_Stats", component.stats);*/
			return ar.EndObject();
		}


	private:

		tmap<size_t, tuptr<c_base>> components;
	};
}