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

#include "../Core.h"

namespace Tempest
{
	template <typename T>
	class Service
	{
	public:
		[[nodiscard]] static T& Get()
		{
			return *service;
		}
		[[nodiscard]] static T* GetIf()
		{
			return service.get();
		}

		template <typename ... Args>
		static void Register(Args&& ... args)
		{
			service = make_uptr<T>(std::forward<Args>(args)...);
		}

		static void Release()
		{
			service.reset(nullptr);
		}

	private:
		inline static tuptr<T> service;
	};
}