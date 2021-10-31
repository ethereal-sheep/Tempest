#pragma once

#include "Core.h"

#include "../Memory.h"
#include "Util/prefab.h"
#include "Util/coptional.h"
#include "Util/prototype_container.h"
#include "Util/registry.h"

namespace Tempest
{
	// scene is an object that stores map data
	// serializing/deserializing
	// runtime load into ecs
	// iterate through what we need so we can render

	class scene_exception : public std::exception
	{
	public:
		scene_exception(const string& err_msg = "scene exception thrown!") : msg(err_msg) {}
		const char* what() const noexcept override { return msg.c_str(); }
	private:
		string msg;
	};

	class Map
	{
		tmap<id_t, prefab> prefabs;

	public:
		Map() = default; // empty map

		auto create(const prototype& proto)
		{
			return prefabs.insert_or_assign(idgen::generate(), proto.instance());
		}

		auto force_create(id_t id, const prototype& proto)
		{
			// if id exist, overwrite
			return prefabs.insert_or_assign(id, proto.instance());
		}

		prefab& get(id_t id)
		{
			return prefabs.at(id);
		}

		prefab* get_if(id_t id)
		{
			if(!prefabs.count(id))
				return nullptr;
			return &prefabs.at(id);
		}

		auto extract(id_t t) { return prefabs.extract(t); }
		auto insert(tmap<id_t, prefab>::node_type&& n) { return prefabs.insert(std::move(n)); }
		bool exist(id_t id) const { return prefabs.contains(id); }
		size_t count(id_t id) const { return prefabs.count(id); }
		size_t size() const { return prefabs.size(); }
		auto begin() { return prefabs.begin(); }
		auto begin() const { return prefabs.begin(); }
		auto end() { return prefabs.end(); }
		auto end() const { return prefabs.end(); }

	};

	class Scene
	{
		tmap<string, prototype_container> categories;
		Map map;

		void load_prototypes(const tpath& proto_folder)
		{
			categories.clear();
			for (auto folder : fs::directory_iterator(proto_folder))
			{
				if (!fs::is_directory(folder))
					continue;
				// each of this are folders
				auto cat = folder.path().filename().string();

				for (auto entry : fs::directory_iterator(folder))
				{
					if (entry.path().extension() != ".json")
						continue;
					// each of this json files
					auto name = entry.path().stem().string();

					// try to load
					try
					{
						categories[cat].load_file(entry.path());
					}
					catch (const std::exception&)
					{
						// do nothing if we can't
						// we can clean if we want as well
						LOG_WARN("Could not load prototype file: {}", entry.path().string());
						continue;
					}
				}
			}
		}

		void load_map(Reader& reader, Map& map_to_load)
		{
			reader.StartObject();
			size_t size;
			reader.StartArray("Prefabs", &size);
			for (size_t i = 0; i < size; ++i)
			{
				string cat;
				string proto;
				reader.StartObject();
				reader.StartMeta();
				reader.Member("Cat", cat);
				reader.Member("Proto", proto);
				reader.EndMeta();
				
				// check if proto exist
				if (categories.count(cat) && categories.at(cat).has(proto))
				{
					map_to_load.create(categories.at(cat).get(proto));
				}
				else
					LOG_WARN("prototype {0} of {1}");

				reader.EndObject();
			}
			reader.EndArray();



			reader.EndObject();
		}

		void save_map(Writer& writer, Map& map_to_save)
		{
			writer.StartObject();

			writer.StartMeta();
			writer.Member("Type", "Map");
			writer.EndMeta();

			writer.StartArray("Prefabs");
			for (auto& [x, p] : map_to_save)
			{
				writer.StartObject();
				writer.StartMeta();
				writer.Member("Cat", p.cat);
				writer.Member("Proto", p.proto);
				writer.EndMeta();

				p.serialize(writer);

				writer.EndObject();
			}
			writer.EndArray();

			writer.EndObject();
		}

	public:
		// empty scene
		Scene()
		{
			// empty scene
			load_prototypes("Prototypes");
		}

		Scene(const tpath& file)
		{
			tpath root;
			tpath scene_json;
			if(!fs::exists(file))
				throw scene_exception(file.string() + " does not exist!");
			if (fs::is_directory(file))
			{
				root = file;
				scene_json = file / "scene.json";
			}
			else if (file.filename() == "scene.json")
			{
				root = file.parent_path();
				scene_json = file;
			}
			else
				throw scene_exception(file.string() + " is an unknown path!");

			load_prototypes(root / "Prototypes");


			Serializer serializer;
			string json = serializer.GetJson(scene_json);

			Reader reader(json.c_str());
			if (reader.HasError())
				throw scene_exception(file.string() + " cannot be opened!");

			load_map(reader, map);
		}


		void save(const tpath& root_folder)
		{
			if (!fs::exists(root_folder))
				fs::create_directories(root_folder);

			// save prototypes
			for (auto& [cat_name, proto_cat] : categories)
			{
				for (auto& [name, proto] : proto_cat)
				{
					proto.save(root_folder / "Prototypes" / cat_name);
				}
			}

			// save prefabs
			Writer writer;
			save_map(writer, map);
			Serializer::SaveJson(root_folder / "scene.json", writer.GetString());

		}

		auto create(const prototype& p)
		{
			return map.create(p);
		}

		auto get_prototype_categories()
		{
			return make_range(categories);
		}

		auto& get_map()
		{
			return map;
		}

	};

}