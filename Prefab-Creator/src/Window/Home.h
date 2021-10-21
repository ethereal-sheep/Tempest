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
#include "Instance/Instance.h"
#include "Events/EventManager.h"
#include "Font.h"

namespace Tempest
{
    class Home : public Window
    {
        const char* window_name() override
        {
            return "Home";
        }
        void init(Instance& instance) override
        {
            // make sure 
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
			path /= "PrefabCreatorTempData";
			path /= "_recent.json";

			Serializer serializer;
			string json = serializer.GetJson(path);

			Reader reader(json.c_str());


			if (!reader.HasError())
			{
				reader.StartObject();
				string p_str;
				reader.Member("rec", p_str);
				reader.EndObject();

				if(!fs::exists(p_str))
					to_retarget = true;
				else
				{
					auto& creator = dynamic_cast<CreatorInstance&>(instance);
					creator.retarget(p_str);
				}
			}
			else
			{
				to_retarget = true;
			}

        }

		void exit(Instance& instance) override
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
			tpath envpath{ pValue };
			free(pValue); // It's OK to call free with NULL
			envpath /= "Documents";
			envpath /= "PrefabCreatorTempData";

			if (!fs::exists(envpath))
				fs::create_directory(envpath);


			string p_str = instance.get_full_path().string();
			LOG_ASSERT(fs::exists(p_str));

			Writer writer;
			writer.StartObject();
			writer.Member("rec", p_str);
			writer.EndObject();

			Serializer::SaveJson(envpath / "_recent.json", writer.GetString());

			// save any unsaved thing
		}

        void show(Instance& ) override
        {
			if (to_retarget)
			{
				Service<EventManager>::Get().instant_dispatch<RetargetingTrigger>();
				to_retarget = false;
			}


        }

		void test_compile()
		{

		}

		void test_load_res()
		{

		}

		bool to_retarget = false;
    };
}