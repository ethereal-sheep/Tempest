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

#include "elsHeader.h"

namespace els
{
	template <typename TVec>
	inline constexpr auto dot(const TVec& a, const TVec& b)
	{
		return a.dot(b);
	}
	template <typename TVec>
	inline constexpr auto cross(const TVec& a, const TVec& b)
	{
		return a.cross(b);
	}
	template <typename TVec>
	inline constexpr auto distance(const TVec& a, const TVec& b)
	{
		return a.distance(b);
	}
	template <typename TVec>
	inline constexpr auto distance2(const TVec& a, const TVec& b)
	{
		return a.distance2(b);
	}
	template <typename TVec>
	inline constexpr auto length(const TVec& a)
	{
		return a.length();
	}
	template <typename TVec>
	inline constexpr auto length2(const TVec& a)
	{
		return a.length2();
	}
	template <typename TVec>
	inline constexpr auto angle(const TVec& a, const TVec& b)
	{
		return a.angle(b);
	}
	template<typename TVec>
	inline constexpr TVec projection(const TVec& a, const TVec& b)
	{
		return std::move(a.projection(b));
	}

	// use default in math header
	/*template<typename TVec, typename S, typename = std::enable_if_t<std::is_arithmetic<S>::value>>
	inline constexpr TVec lerp(const TVec& a, const TVec& b, S t)
	{
		return std::move(a.lerp(b, t));
	}*/

	template<typename TVec>
	inline constexpr TVec normalized(const TVec& a)
	{
		return std::move(a.normalized());
	}
	template<typename TVec>
	inline constexpr void normalize(TVec& rhs)
	{
		rhs.normalize();
	}
}