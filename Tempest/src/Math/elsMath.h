/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once

#include <cmath>

#include "elsHeader.h"


namespace els
{
	template<typename T = defaultType>
	constexpr T sqrt2 = static_cast<T>(1.4142135623730950488016887242);
	template<typename T = defaultType>
	constexpr T ln2 = static_cast<T>(0.6931471805599453094172321215);
	template<typename T = defaultType>
	constexpr T pi = static_cast<T>(3.1415926535897932384626433833);
	template<typename T = defaultType>
	constexpr T tau = 2 * pi<T>;
	template<typename T = defaultType>
	constexpr T e = static_cast<T>(2.7182818284590452353602874714);
	template<typename T = defaultType>
	constexpr T phi = static_cast<T>(1.6180339887498948482045868343);
	template<typename T = defaultType>
	constexpr T epsilon = static_cast<T>(0.00001);
	template<typename T = defaultType>
	constexpr T epsilon2 = epsilon<T> *epsilon<T>;

	template<size_t I, size_t E>
	constexpr size_t pow_i()
	{
		if constexpr (E == 0)
			return 1;
		else
			return pow_i<I, E - 1>() * I;
	}

	template<size_t I>
	constexpr size_t fib()
	{
		if constexpr (I == 0 || I == 1)
			return 1;
		else
			return fib<I - 1>() + I;
	}


	template <typename T, typename = std::enable_if_t<std::is_floating_point<T>::value>>
	inline constexpr T sin(T a) { return ::sin(a); }
	template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
	inline constexpr defaultType sin(T a) { return ::sin(static_cast<defaultType>(a)); }

	template <typename T, typename = std::enable_if_t<std::is_floating_point<T>::value>>
	inline constexpr T cos(T a) { return ::cos(a); }
	template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
	inline constexpr defaultType cos(T a) { return ::cos(static_cast<defaultType>(a)); }

	template <typename T, typename = std::enable_if_t<std::is_floating_point<T>::value>>
	inline constexpr T tan(T a) { return ::tan(a); }
	template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
	inline constexpr defaultType tan(T a) { return ::tan(static_cast<defaultType>(a)); }

	template <typename T, typename = std::enable_if_t<std::is_floating_point<T>::value>>
	inline T sinh(T a) { return ::sinh(a); }
	template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
	inline constexpr defaultType sinh(T a) { return ::sinh(static_cast<defaultType>(a)); }

	template <typename T, typename = std::enable_if_t<std::is_floating_point<T>::value>>
	inline constexpr T sinc(T a) { return ::sinc(a); }
	template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
	inline constexpr defaultType sinc(T a) { return ::sinc(static_cast<defaultType>(a)); }

	template <typename T, typename = std::enable_if_t<std::is_floating_point<T>::value>>
	inline constexpr T sincn(T a) { return sinc<T>(pi<T> *a); }
	template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
	inline constexpr defaultType sincn(T a) { return sinc<defaultType>(pi<defaultType> *a); }

	template <typename T, typename = std::enable_if_t<std::is_floating_point<T>::value>>
	inline constexpr T cosh(T a) { return ::cosh(a); }
	template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
	inline constexpr defaultType cosh(T a) { return ::cosh(static_cast<defaultType>(a)); }

	template <typename T, typename = std::enable_if_t<std::is_floating_point<T>::value>>
	inline constexpr T tanh(T a) { return ::tanh(a); }
	template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
	inline constexpr defaultType tanh(T a) { return ::tanh(static_cast<defaultType>(a)); }

	template <typename T, typename = std::enable_if_t<std::is_floating_point<T>::value>>
	inline constexpr T asin(T a) { return ::asin(a); }
	template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
	inline constexpr defaultType asin(T a) { return ::asin(static_cast<defaultType>(a)); }

	template <typename T, typename = std::enable_if_t<std::is_floating_point<T>::value>>
	inline constexpr T acos(T a) { return ::acos(a); }
	template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
	inline constexpr defaultType acos(T a) { return ::acos(static_cast<defaultType>(a)); }

	template <typename T, typename = std::enable_if_t<std::is_floating_point<T>::value>>
	inline constexpr T atan(T a) { return ::atan(a); }
	template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
	inline constexpr defaultType atan(T a) { return ::atan(static_cast<defaultType>(a)); }

	template <typename Ty, typename Tx,
		typename = std::enable_if_t<
		std::is_floating_point<Ty>::value ||
		std::is_floating_point<Tx>::value>>
		inline constexpr auto atan2(Ty y, Tx x) -> decltype(::atan2(y, x)) { return ::atan2(y, x); }
	template <typename Ty, typename Tx,
		typename = std::enable_if_t<
		std::is_integral<Ty>::value&&
		std::is_integral<Tx>::value>>
		inline constexpr defaultType atan2(Ty y, Tx x) { return static_cast<defaultType>(::atan2(y, x)); }


	template <typename T, typename = std::enable_if_t<std::is_floating_point<T>::value>>
	inline constexpr T sqrt(T x) { return ::sqrt(x); }
	template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
	inline constexpr defaultType sqrt(T x) { return ::sqrt(static_cast<defaultType>(x)); }

	template <typename Ty, typename Tx>
	inline constexpr auto pow(Ty base, Tx exp) -> decltype(::pow(base, exp)) { return ::pow(base, exp); }


	template <typename T, typename = std::enable_if_t<std::is_floating_point<T>::value>>
	inline constexpr T log(T a) { return ::log(a); }
	template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
	inline constexpr defaultType log(T a) { return ::log(static_cast<defaultType>(a)); }

	template <typename T, typename = std::enable_if_t<std::is_floating_point<T>::value>>
	inline constexpr T exp(T a) { return ::exp(a); }
	template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
	inline constexpr defaultType exp(T a) { return ::exp(static_cast<defaultType>(a)); }



	template <typename T, typename = std::enable_if_t<std::is_floating_point<T>::value>>
	inline constexpr T floor(T x) { return ::floor(x); }
	template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
	inline constexpr defaultType floor(T x) { return ::floor(static_cast<defaultType>(x)); }

	template <typename T, typename = std::enable_if_t<std::is_floating_point<T>::value>>
	inline constexpr T frac(T x) { return  x - ::floor(x); }


	template <typename T, typename = std::enable_if_t<std::is_floating_point<T>::value>>
	inline constexpr T ceil(T x) { return ::ceil(x); }
	template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
	inline constexpr defaultType ceil(T x) { return ::ceil(static_cast<defaultType>(x)); }

	template <typename T>
	inline constexpr T abs(T x) { return ::abs(x); }

	template <typename Ty, typename Tx,
		typename = std::enable_if_t<
		std::is_floating_point<Ty>::value ||
		std::is_floating_point<Tx>::value>>
		inline constexpr auto mod(Ty y, Tx x) -> decltype(::fmod(y, x)) { return ::fmod(y, x); }
	template <typename Ty, typename Tx,
		typename = std::enable_if_t<
		std::is_integral<Ty>::value&&
		std::is_integral<Tx>::value>>
		inline constexpr std::common_type_t<Ty, Tx> mod(Ty y, Tx x) { return y % x; }

	template <typename T>
	inline constexpr T round(T a) { return static_cast<T>(::round(a)); }

	template <typename TVal, typename T,
		typename = std::enable_if_t<
		std::is_floating_point<TVal>::value ||
		std::is_floating_point<T>::value>>
		inline constexpr auto map(TVal val, T start_from, T stop_from, T start_to, T stop_to) -> decltype(val - start_from)
	{
		return (val - start_from) * (stop_to - start_to) / (stop_from - start_from) + val;
	}

	template <typename TVal, typename T,
		typename = std::enable_if_t<
		std::is_integral<TVal>::value&&
		std::is_integral<T>::value>>
		inline constexpr defaultType map(TVal val, T start_from, T stop_from, T start_to, T stop_to)
	{
		return (val - start_from) * static_cast<defaultType>(stop_to - start_to) / static_cast<defaultType>(stop_from - start_from) + val;
	}

	template <typename T>
	inline constexpr const T& clamp(const T& val, const T& min, const T& max)
	{
		if (min > max)
			return clamp(val, max, min);
		return (val < min) ? min : (max < val) ? max : val;
	}

	template <typename T, typename = std::enable_if_t<std::is_floating_point<T>::value>>
	inline constexpr T deg_to_rad(T x) { return x * pi<T> / 180; }
	template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
	inline constexpr defaultType deg_to_rad(T x) { return x * pi<defaultType> / 180; }

	template <typename T, typename = std::enable_if_t<std::is_floating_point<T>::value>>
	inline constexpr T rad_to_deg(T x) { return x * 180 / pi<T>; }
	template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
	inline constexpr defaultType rad_to_deg(T x) { return x * 180 / pi<defaultType>; }

	template <typename Tfrom, typename Tto, typename Tweight>
	inline constexpr auto lerp(Tfrom from, Tto to, Tweight t) -> decltype(from* (1 - t) + to * t)
	{
		return from * (1 - t) + to * t;
	}

	template <typename Tfrom, typename Tto, typename Tweight,
		typename = std::enable_if_t<
		std::is_floating_point<Tfrom>::value ||
		std::is_floating_point<Tto>::value ||
		std::is_floating_point<Tweight>::value>>
		inline constexpr auto lerp_angle(Tfrom from, Tto to, Tweight t) -> decltype(from + to + t)
	{
		using promoted = decltype(from + to + t);
		auto difference = fmod(to - from, tau<promoted>);
		auto distance = fmod(2 * difference, tau<promoted>) - difference;
		return from + distance * t;
	}

	template <typename Tfrom, typename Tto, typename Tweight,
		typename = std::enable_if_t<
		std::is_integral<Tfrom>::value&&
		std::is_integral<Tto>::value&&
		std::is_integral<Tweight>::value>>
		inline constexpr defaultType lerp_angle(Tfrom from, Tto to, Tweight t)
	{
		defaultType difference = fmod(to - from, tau<defaultType>);
		defaultType distance = fmod(2 * difference, tau<defaultType>) - difference;
		return from + distance * t;
	}

	template <typename Tfrom, typename Tto, typename Tweight>
	inline constexpr std::common_type_t<Tfrom, Tto, Tweight> inverse_lerp(Tfrom from, Tto to, Tweight t)
	{
		using promoted = std::common_type_t<Tfrom, Tto, Tweight>;
		if (from - to <= epsilon2<promoted>)
			return static_cast<promoted>(from);
		return  (t - from) / (to - from);
	}

	template <typename Ty, typename Tx>
	inline constexpr Tx step(Ty edge, Tx x) { return (x > edge) ? static_cast<Tx>(1) : static_cast<Tx>(0); }

	template <typename Ty, typename Tx, typename = std::enable_if_t<std::is_floating_point<Tx>::value>>
	inline constexpr Tx smoothstep(Ty edge0, Ty edge1, Tx x)
	{
		const Tx tmp = clamp((x - edge0) / (edge1 - edge0), static_cast<Tx>(0), static_cast<Tx>(1));
		return tmp * tmp * (static_cast<Tx>(3) - static_cast<Tx>(2) * tmp);
	}

	template <typename Ty, typename Tx,
		typename = std::enable_if_t<
		std::is_integral<Ty>::value&&
		std::is_integral<Tx>::value>>
		inline constexpr std::common_type_t<Ty, Tx> gcd(Ty y, Tx x)
	{
		if (x == 0)
			return y;
		return gcd(y, x % y);
	}

	template <typename Ty, typename Tx,
		typename = std::enable_if_t<
		std::is_integral<Ty>::value&&
		std::is_integral<Tx>::value>>
		inline constexpr std::common_type_t<Ty, Tx> lcm(Ty y, Tx x)
	{
		return (y * x) / gcd(y, x);
	}


} // namespace els
