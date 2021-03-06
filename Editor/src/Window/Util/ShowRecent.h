/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include "Instance/Instance.h"
#include "Events/EventManager.h"
#include "Triggers/Triggers.h"

namespace Tempest
{
	class ShowRecent : public Window
	{
		const char* window_name() override
		{
			return "";
		}

		void save_current_to_recent()
		{
			// if current directory is nothing, we can just return
			if (current.empty() || current == ".json") return;

			// check APPDATA if there exist a file with project info
			char* pValue;
			size_t len;
			[[maybe_unused]] errno_t err = _dupenv_s(&pValue, &len, "USERPROFILE");
			if (!pValue)
			{
				LOG_WARN("APPDATA environment variable could not be found!");
				return;
			}
			tpath envpath{ pValue };
			free(pValue); // It's OK to call free with NULL
			envpath /= "Documents";
			envpath /= "CoReSys";

			if (!fs::exists(envpath))
				fs::create_directory(envpath);


			Writer writer;
			writer.StartObject();

			writer.StartArray("Recent");
			for (auto& path : recent)
			{
				if (path == current) continue;

				writer.StartObject();
				string p_str = path.string();
				writer.Member("rec", p_str);
				writer.EndObject();
			}

			writer.StartObject();
			string p_str = current.string();
			writer.Member("rec", p_str);
			writer.EndObject();

			writer.EndArray();

			writer.EndObject();


			Serializer::SaveJson(envpath / "_recent.json", writer.GetString());
		}

		void load_recent()
		{
			// check APPDATA if there exist a file with project info
			char* pValue;
			size_t len;
			[[maybe_unused]] errno_t err = _dupenv_s(&pValue, &len, "USERPROFILE");
			if (!pValue)
			{
				LOG_WARN("APPDATA environment variable could not be found!");
				return;
			}
			tpath path{ pValue };
			free(pValue); // It's OK to call free with NULL
			path /= "Documents";
			path /= "CoReSys";
			path /= "_recent.json";

			Serializer serializer;
			string json = serializer.GetJson(path);

			Reader reader(json.c_str());
			if (!reader.HasError())
			{
				reader.StartObject();

				size_t size;
				reader.StartArray("Recent", &size);
				for (size_t i = 0; i < size; ++i)
				{
					reader.StartObject();
					string p_str;
					reader.Member("rec", p_str);
					tpath a(p_str);
					if (fs::exists(a))
						recent.push_back(a);

					reader.EndObject();
				}
				reader.EndArray();

				reader.EndObject();
			}
			else
				LOG("No recent projects found.");
		}

		void init(Instance& instance) override
		{
			if (current.empty()) {
				current = instance.get_path() / (instance.get_name() + ".json");
			}

			Service<EventManager>::Get().register_listener<ShowRecentUtil>(&ShowRecent::show_recent, this);
			Service<EventManager>::Get().register_listener<GetRecentUtil>(&ShowRecent::get_recent, this);

			save_current_to_recent();
			load_recent();
		}


		void get_recent(const Event& e)
		{
			if (auto a = s_event_cast<GetRecentUtil>(e))
			{
				const_cast<GetRecentUtil*>(a)->paths = recent;
			}
		}

		void show_recent(const Event&)
		{
			if (recent.empty() || (recent.size() == 1 && recent[0] == current))
			{
				ImGui::MenuItem("(No recent projects)", nullptr, false, false);
			}
			else
			{
				for (auto it = recent.rbegin(); it != recent.rend(); ++it)
				{
					if (*it == current) continue;

					if (ImGui::MenuItem(it->stem().string().c_str()))
					{
						Service<EventManager>::Get().instant_dispatch<SaveCurrentBeforeOpenTrigger>(*it);
					}
				}
			}
		}


		void exit(Instance&) override
		{
			
		}


		void show(Instance& instance) override {
			if (current.empty()) {
				current = instance.get_path() / (instance.get_name() + ".json");
			}
		}

		tpath current;
		std::vector<tpath> recent;
	};
}