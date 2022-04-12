/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "test_entity.h"
#include "../Util/prototype.h"
#include "../Util/prototype_container.h"
#include "../Prototypes/Prototype_Category.h"
#include "ECS/ECS.h"

#include <rttr/registration>


namespace Tempest
{
	void testing_coptional1()
	{
		coptional<tc::Meta> a;
		a->name = "Test";

		// create child
		auto child_ptr = a.instance();
		auto& child = *dynamic_cast<coptional<tc::Meta>*>(child_ptr.get());
		LOG_ASSERT(child->name == "Test");
		LOG_ASSERT(child.is_child() == true);
		LOG_ASSERT(a.is_parent() == true);

		a->name = "LOL";
		LOG_ASSERT(child->name == "LOL");

		child.toggle();
		LOG_ASSERT(child->name == "LOL");

		a->name = "LMAO";
		LOG_ASSERT(child->name == "LOL");

		child.toggle();
		LOG_ASSERT(child->name == "LMAO");
	}


	void testing_prototype1()
	{
		prototype p;
		auto meta = p.emplace<tc::Meta>();

		meta->name = "LOL";
		LOG_ASSERT(p.has<tc::Meta>());

		auto& a = p.get<tc::Meta>();

		LOG_ASSERT(a.name == "LOL");
	}

	void testing_prototype2()
	{
		prototype p;
		auto meta = p.emplace<tc::Meta>();

		meta->name = "LOL";

		prototype p1 = p;
		LOG_ASSERT(p1.has<tc::Meta>());

		auto& a = p1.get<tc::Meta>();

		LOG_ASSERT(a.name == "LOL");
	}

	void testing_prototype_with_prefab()
	{
		prototype p;
		auto meta = p.emplace<tc::Meta>();
		meta->name = "LOL";

		auto prefab = p.instance();
		LOG_ASSERT(prefab.has<tc::Meta>());
		LOG_ASSERT(prefab.get<tc::Meta>().name == "LOL");
		meta->name = "LMAO";
		LOG_ASSERT(prefab.get<tc::Meta>().name == "LMAO");

		prefab.force<tc::Meta>().name = "Changed";
		LOG_ASSERT(prefab.get<tc::Meta>().name == "Changed");

		prefab.inherit<tc::Meta>(true);
		LOG_ASSERT(prefab.get<tc::Meta>().name == "LMAO");

		meta->name = "LOL";
		LOG_ASSERT(prefab.get<tc::Meta>().name == "LOL");

	}
	void testing_prototype_serialize()
	{
		const char* s = R"(S:\Development\Projects)";
		prototype p;
		auto meta = p.emplace<tc::Meta>();
		//auto transform = p.emplace<tc::Transform>();
		meta->name = "LOL";
		
		auto target = p.save(s);

		prototype d(target);
		LOG_ASSERT(d.has<tc::Meta>());
		LOG_ASSERT(d.has<tc::Transform>());
		LOG_ASSERT(d.get<tc::Meta>().name == "LOL");

		try
		{
			prototype fail{ tpath("Fail/Path")};
			LOG_CRITICAL("FAIL");
		}
		catch (const std::exception& a)
		{
			LOG(a.what());
		}
	}

	void testing_prototype_container()
	{
		const char* s = R"(S:\Development\Projects\Prototypes)";

		prototype p;
		auto meta = p.emplace<tc::Meta>();
		meta->name = "LOL";

		{
			prototype_container pc;
			pc.emplace("Empty");
			LOG_ASSERT(pc.emplace(p));

			meta->name = "Changed";

			LOG_ASSERT(pc.has("Empty"));
			LOG_ASSERT(pc.has("Prototype"));
			LOG_ASSERT(pc.get("Prototype").get<tc::Meta>().name == "LOL");

			pc.save(s);
		}

		{
			prototype_container pc(s);

			LOG_ASSERT(pc.has("Empty"));
			LOG_ASSERT(pc.has("Prototype"));
			LOG_ASSERT(pc.get("Prototype").get<tc::Meta>().name == "LOL");
		}

	}

	void testing_prototype_factory()
	{
		auto p = create_new_prototype("Wall");
		LOG_ASSERT(p.has<tc::Meta>());
		LOG_ASSERT(p.has<tc::Transform>());
	}

	void testing_prefab_copy()
	{
		prototype p;
		auto meta = p.emplace<tc::Meta>();
		meta->name = "LOL";

		auto prefab1 = p.instance(); // instanced
		auto prefab2 = prefab1; // cloned

		LOG_ASSERT(prefab1.get<tc::Meta>().name == "LOL");
		LOG_ASSERT(prefab2.get<tc::Meta>().name == "LOL");

		prefab1.force<tc::Meta>().name = "Changed";
		LOG_ASSERT(prefab1.get<tc::Meta>().name == "Changed");
		LOG_ASSERT(prefab2.get<tc::Meta>().name == "LOL");
	}

	void testing_prefab_serialize()
	{
		//prototype p;
		//const char* s = R"(S:\Development\Projects)";
		//auto meta = p.emplace<tc::Meta>();

		//// serialize
		//{
		//	auto prefab1 = p.instance(); // instanced
		//	auto prefab2 = prefab1; // cloned
		//}

		//// deserialize
		//{

		//}
	}

	void testing_prototype()
	{
		/*testing_coptional1();
		testing_prototype1();
		testing_prototype2();
		testing_prototype_serialize();
		testing_prototype_with_prefab();


		testing_prototype_container();


		testing_prefab_copy();
		testing_prefab_serialize();*/
	}
}