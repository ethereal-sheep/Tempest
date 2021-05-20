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

		template <typename ... Args>
		static void Register(Args&& ... args)
		{
			service = make_ptr<T>(std::forward<Args>(args)...);
		}

		static void Release()
		{
			service.reset(nullptr);
		}

	private:
		inline static tuptr<T> service;
	};
}