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

		tmap<int, tmap<int, tvector<id_t>>> map;

	public:
		Map() = default; // empty map

		void update()
		{
			// just to clean up
			// might be sufficient
			map.clear();
			for (auto& [id, pf] : prefabs)
			{
				if (pf.has<tc::Transform>() && pf.has<tc::Shape>())
				{
					auto shape = pf.get_if<tc::Shape>();
					auto transform = pf.get_if<tc::Transform>();

					const int& x = shape->x;
					const int& y = shape->y;

					int a_x = x, a_y = y, e_x = 0, e_y = 0;

					if (a_x % 2 != a_y % 2)
					{
						a_x = a_y = std::min(x, y);
						e_x = x - a_x;
						e_y = y - a_y;
					}

					vec3 min, max;

					min.x = -.5f - (a_x - 1) / 2.f;
					min.z = -.5f - (a_y - 1) / 2.f;
					min.y = 0;

					max.x = .5f + (a_x - 1) / 2.f + e_x;
					max.z = .5f + (a_y - 1) / 2.f + e_y;
					max.y = 0;

					auto rot = transform->rotation;
					min = rot * min;
					max = rot * max;

					if (max.x < min.x) std::swap(min.x, max.x);
					if (max.z < min.z) std::swap(min.z, max.z);

					min += transform->position;
					max += transform->position;

					for (int i = (int)std::floor(min.x); i < (int)std::floor(max.x); ++i)
						for (int j = (int)std::floor(min.z); j < (int)std::floor(max.z); ++j)
							map[i][j].push_back(id);
				}
			}
		}

		id_t find(int x, int y)
		{
			if (map.count(x) && map[x].count(y) && map[x][y].size())
				return map[x][y].front();
			return INVALID;
		}


		auto create(const prototype& proto)
		{
			return prefabs.insert_or_assign(idgen::generate(), proto.instance());
		}

		auto create(const prefab& pf)
		{
			return prefabs.insert_or_assign(idgen::generate(), pf);
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
		bool exist(id_t id) const { return prefabs.count(id); }
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

			if (!fs::exists(proto_folder))
				return;

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

		void override_prototypes(const tpath& proto_folder)
		{
			if (!fs::exists(proto_folder))
				return;

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

					// check if the prototype exist
					// if no then do nothing
					if (!categories[cat].has(name))
					{
						LOG_WARN("The prototype {0} has been deprecated from the system", name);
						continue;
					}

					categories[cat].get(name).load_override(entry.path());
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
					auto [it, b] = map_to_load.create(categories.at(cat).get(proto));
					it->second.deserialize(reader);
				}
				else
					LOG_WARN("prototype {0} of {1} not found!");

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

		/*
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
		
		*/

		void save(const tpath& root_folder)
		{
			if (!fs::exists(root_folder))
				fs::create_directories(root_folder);

			// save prototypes
			for (auto& [cat_name, proto_cat] : categories)
			{
				for (auto& [name, proto] : proto_cat)
				{
					proto.save_override(root_folder / "Prototypes" / cat_name);
				}
			}

			// save prefabs
			Writer writer;
			save_map(writer, map);
			Serializer::SaveJson(root_folder / "scene.json", writer.GetString());

		}

		void load(const tpath& root_folder)
		{
			// assume all the base prototypes are loaded already
			// so if we try to load any other prototypes
			// it is an override function
			// over all existing

			if (root_folder == "")
				return;

			auto file = root_folder;

			tpath root;
			tpath scene_json;
			if (!fs::exists(file))
				return;

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
				return;

			if (!fs::exists(scene_json))
				return;

			override_prototypes(root / "Prototypes");


			Serializer serializer;
			string json = serializer.GetJson(scene_json);

			Reader reader(json.c_str());
			if (reader.HasError())
				throw scene_exception(file.string() + " cannot be opened!");

			load_map(reader, map);
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