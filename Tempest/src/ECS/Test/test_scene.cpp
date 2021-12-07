/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "test_entity.h"
#include "../Scene.h"
#include "ECS/ECS.h"

namespace Tempest
{
	void testing_scene_proto()
	{
		const char* root = R"(S:\Development\Projects\testing_scene1)";
		size_t cat_size = 1231516445;
		tvector<id_t> v;
		{
			Scene s;
			cat_size = s.get_prototype_categories().size();
			
			for (auto [cat_name, proto_cat] : s.get_prototype_categories())
			{
				for (auto [proto_name, proto] : proto_cat)
				{
					auto p = s.get_map().create(proto);
					p.first->second.force<tc::Meta>().name = "Override";
				}
			}
			s.save(root);
		}
		{
			tpath t = root;
			Scene s;
			s.load(t);
			LOG_ASSERT(s.get_prototype_categories().size() == cat_size);
		}
	}

	void testing_scene()
	{
		//testing_scene_proto();
	}
}