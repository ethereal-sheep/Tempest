#include "Instance.h"

namespace Tempest
{
	/**
	 * @brief Initializes any required components.
	 */
	void Instance::OnInit()
	{
		auto view = ecs.view<float, int>();
	}
	/**
	 * @brief Update loop. Update any required stuff here.
	 */
	void Instance::OnUpdate([[maybe_unused]] float dt)
	{
	}
	/**
	 * @brief Render loop. Render any required stuff here.
	 */
	void Instance::OnRender()
	{

	}
	/**
	 * @brief Deinitializes any 
	 */
	void Instance::OnExit()
	{
		//
	}
}