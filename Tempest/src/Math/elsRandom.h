/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2020
* \note			Course: GAM200
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

#include "elsHeader.h"

namespace els
{
	namespace random
	{
		using defaultPRNG = std::mt19937;

		static std::random_device device{};
		static defaultPRNG prng{ device() };

		inline static uint32_t reseed(uint32_t seed = device())
		{
			prng.seed(seed);
			return seed;
		}

		template<typename T>
		inline static T uniform_rand(T min, T max)
		{
			if (min > max)
				return uniform_rand(max, min);
			if constexpr (std::is_floating_point<T>::value)
				return std::uniform_real_distribution<T>{ min, max }(prng);
			else
				return std::uniform_int_distribution<T>{ min, max }(prng);
		}

		template <typename T>
		inline static Vector2<T> uniform_rand(const Vector2<T>& min, const Vector2<T>& max)
		{
			return Vector2<T>{
				uniform_rand<T>(min.x, max.x),
					uniform_rand<T>(min.y, max.y)}; 
		}
		template <typename T>
		inline static Vector3<T> uniform_rand(const Vector3<T>& min, const Vector3<T>& max)
		{
			return Vector3<T>{
				uniform_rand<T>(min.x, max.x),
					uniform_rand<T>(min.y, max.y),
					uniform_rand<T>(min.z, max.z)};
		}
		template <typename T>
		inline static Vector4<T> uniform_rand(const Vector4<T>& min, const Vector4<T>& max)
		{
			return Vector4<T>{
				uniform_rand<T>(min.x, max.x),
					uniform_rand<T>(min.y, max.y),
					uniform_rand<T>(min.z, max.z),
					uniform_rand<T>(min.w, max.w)};
		}

		template<typename T, typename = std::enable_if_t<std::is_floating_point<T>::value>>
		inline static T normal_rand(T mean, T std_dev) { return std::normal_distribution<T>{ mean, std_dev }(prng); }
		template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
		inline static defaultType normal_rand(T mean, T std_dev) { return std::normal_distribution<defaultType>{ static_cast<defaultType>(mean), static_cast<defaultType>(std_dev) }(prng); }
		template<typename Ty, typename Tx>
		inline static std::common_type_t<Ty, Tx> normal_rand(Ty mean, Tx std_dev)
		{
			using promoted = std::common_type_t<Ty, Tx>;
			return std::normal_distribution<promoted>{
				static_cast<promoted>(mean),
					static_cast<promoted>(std_dev)}(prng);
		}

		template <typename T, typename = std::enable_if_t<std::is_floating_point<T>::value>>
		inline static Vector2<T> circle_rand(T radius)
		{
			if (radius <= static_cast<T>(0))
				return Vector2<T>::zero;

			T angle = uniform_rand(static_cast<T>(0), tau<T>);
			return Vector2<T>{cos(angle), sin(angle)} *radius;

		}
		template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
		inline static Vector2<defaultType> circle_rand(T radius)
		{
			return circular_rand(static_cast<defaultType>(radius));
		}

		template <typename T, typename = std::enable_if_t<std::is_floating_point<T>::value>>
		inline static Vector2<T> disk_rand(T radius)
		{
			using vec = Vector2<T>;

			if (radius <= static_cast<T>(0))
				return vec::zero;

			T radius2 = radius * radius;
			T len2 = 0;
			vec ret{ 0 };

			do
			{
				ret = uniform_rand(vec{ -radius }, vec{ radius });
				len2 = ret.length2();
			} while (len2 > radius2);

			return ret;
		}
		template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
		inline static Vector2<defaultType> disk_rand(T radius)
		{
			return disk_rand(static_cast<defaultType>(radius));
		}

		template <typename T, typename = std::enable_if_t<std::is_floating_point<T>::value>>
		inline static Vector3<T> sphere_rand(T radius)
		{
			if (radius <= static_cast<T>(0))
				return Vector3<T>::zero;

			T theta = uniform_rand(static_cast<T>(0), tau<T>);
			T phi = acos(uniform_rand(static_cast<T>(-1.0f), static_cast<T>(1.0f)));

			T x = sin(phi) * cos(theta);
			T y = sin(phi) * sin(theta);
			T z = cos(phi);

			return Vector3<T>{x, y, z} *radius;
		}
		template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
		inline static Vector3<defaultType> spherical_rand(T radius)
		{
			return sphere_rand(static_cast<defaultType>(radius));
		}

		template <typename T, typename = std::enable_if_t<std::is_floating_point<T>::value>>
		inline static Vector3<T> ball_rand(T radius)
		{
			using vec = Vector3<T>;

			if (radius <= static_cast<T>(0))
				return vec::zero;

			T radius2 = radius * radius;
			T len2 = 0;
			vec ret{ 0 };

			do
			{
				ret = uniform_rand(vec{ -radius }, vec{ radius });
				len2 = ret.length2();
			} while (len2 > radius2);

			return ret;

		}
		template <typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
		inline static Vector3<defaultType> ball_rand(T radius)
		{
			return ball_rand(static_cast<defaultType>(radius));
		}

	}
}