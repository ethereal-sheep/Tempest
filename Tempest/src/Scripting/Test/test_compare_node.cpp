/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "test_scripting.h"
#include "../Util/var_data.h"
#include "../Util/var_set.h"
#include "Util.h"
#include "Util/view.h"
#include "Scripting/SRM.h"
#include "Instance/NullTimeInstance.h"
#include "Events/Events/ScriptEvents.h"

namespace Tempest
{
	void CompareNode_ComparisonTesting()
	{
		NullTimeInstance instance;

		//int flow_out; //, lhs_out, rhs_out;

		auto tempFN = [&instance](int lhs, int rhs, int assert_value)
		{
			Entity ent = instance.ecs.create();
			auto compareNode = CompareNode::create_node("CompareFlow");

			compareNode->get_input_pin(1)->default_var.set(lhs);
			compareNode->get_input_pin(2)->default_var.set(rhs);



			auto compareScript = compareNode->create_script_pack(ent, instance);
			(*compareScript)();

			// Output values
			auto flow_out = std::any_cast<int>(compareScript->operator[](0));
			auto lhs_out = std::any_cast<int>(compareScript->operator[](1));
			auto rhs_out = std::any_cast<int>(compareScript->operator[](2));

			LOG_ASSERT(flow_out == assert_value);
			LOG_ASSERT(lhs_out == lhs);
			LOG_ASSERT(rhs_out == rhs);

			instance.ecs.destroy(ent);
			instance.srm.clear();
		};

		// There is only 3 comparison case, hence 0,1,2, the 3 test cases.
		tempFN(5, 2, 0);
		tempFN(5, 5, 1);
		tempFN(2, 5, 2);
	}

	void TestCompareNode()
	{
		CompareNode_ComparisonTesting();
	}
}