/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


# pragma once
#include "elsHeader.h"
#include "elsMath.h"
#include "elsRandom.h"


namespace els
{
	// noise using perlin noise
	// note*: change to simplex
	template <typename T>
	class PerlinNoise
	{
	public:
		using Scalar = T;

	private:

		// using original 1983 permutation as default
		inline static uint8_t p[] = { 151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,
										103,30,69,142,8,99,37,240,21,10,23,190,6,148,247,120,234,75,0,
										26,197,62,94,252,219,203,117,35,11,32,57,177,33,88,237,149,56,
										87,174,20,125,136,171,168,68,175,74,165,71,134,139,48,27,166,
										77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,
										46,245,40,244,102,143,54,65,25,63,161,1,216,80,73,209,76,132,
										187,208,89,18,169,200,196,135,130,116,188,159,86,164,100,109,
										198,173,186,3,64,52,217,226,250,124,123,5,202,38,147,118,126,
										255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,223,183,
										170,213,119,248,152,2,44,154,163,70,221,153,101,155,167,43,
										172,9,129,22,39,253,19,98,108,110,79,113,224,232,178,185,112,
										104,218,246,97,228,251,34,242,193,238,210,144,12,191,179,162,
										241,81,51,145,235,249,14,239,107,49,192,214,31,181,199,106,
										157,184,84,204,176,115,121,50,45,127,4,150,254,138,236,205,
										93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,
										151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,
										103,30,69,142,8,99,37,240,21,10,23,190,6,148,247,120,234,75,0,
										26,197,62,94,252,219,203,117,35,11,32,57,177,33,88,237,149,56,
										87,174,20,125,136,171,168,68,175,74,165,71,134,139,48,27,166,
										77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,
										46,245,40,244,102,143,54,65,25,63,161,1,216,80,73,209,76,132,
										187,208,89,18,169,200,196,135,130,116,188,159,86,164,100,109,
										198,173,186,3,64,52,217,226,250,124,123,5,202,38,147,118,126,
										255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,223,183,
										170,213,119,248,152,2,44,154,163,70,221,153,101,155,167,43,
										172,9,129,22,39,253,19,98,108,110,79,113,224,232,178,185,112,
										104,218,246,97,228,251,34,242,193,238,210,144,12,191,179,162,
										241, 81,51,145,235,249,14,239,107,49,192,214,31,181,199,106,
										157,184, 84,204,176,115,121,50,45,127,4,150,254,138,236,205,
										93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180 };

		static constexpr Scalar fade(Scalar t)
		{
			return t * t * t * (t * (t * 6 - 15) + 10);
		}
		static constexpr Scalar grad(uint8_t hash, Scalar x, Scalar y, Scalar z)
		{
			const uint8_t h = hash & 0b1111U;
			const Scalar u = h < 0b1000U ? x : y;
			const Scalar v = h < 0b0100U ? y : h == 0b1100U || h == 0b1110U ? x : z;
			return ((h & 0b0001U) == 0 ? u : -u) + ((h & 0b0010U) == 0 ? v : -v);
		}
		static constexpr Scalar weight(uint32_t octaves)
		{
			Scalar amp = 1;
			Scalar value = 0;

			for (size_t i = 0; i < octaves; ++i)
			{
				value += amp;
				amp /= 2;
			}

			return value;
		}

		static constexpr Scalar noise(Scalar x, Scalar y, Scalar z)
		{
			const int32_t X = static_cast<int32_t>(els::floor(x)) & 255;
			const int32_t Y = static_cast<int32_t>(els::floor(y)) & 255;
			const int32_t Z = static_cast<int32_t>(els::floor(z)) & 255;

			x -= els::floor(x);
			y -= els::floor(y);
			z -= els::floor(z);

			const Scalar u = fade(x);
			const Scalar v = fade(y);
			const Scalar w = fade(z);

			const std::int32_t A = p[X] + Y, AA = p[A] + Z, AB = p[A + 1] + Z;
			const std::int32_t B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z;

			return
				lerp(
					lerp(
						lerp(
							grad(p[AA], x, y, z),
							grad(p[BA], x - 1, y, z),
							u
						),
						lerp(
							grad(p[AB], x, y - 1, z),
							grad(p[BB], x - 1, y - 1, z),
							u
						),
						v
					),
					lerp(
						lerp(
							grad(p[AA + 1], x, y, z - 1),
							grad(p[BA + 1], x - 1, y, z - 1),
							u
						),
						lerp(
							grad(p[AB + 1], x, y - 1, z - 1),
							grad(p[BB + 1], x - 1, y - 1, z - 1),
							u
						),
						v
					),
					w
				);
		}
		static constexpr Scalar octave_noise(Scalar x, Scalar y, Scalar z, uint32_t octaves)
		{
			Scalar result = 0;
			Scalar amp = 1;

			for (uint32_t i = 0; i < octaves; ++i)
			{
				result += noise(x, y, z) * amp;
				x *= 2;
				amp /= 2;
			}

			return result; // unnormalized
		}
		static constexpr Scalar normalised_octave_noise(Scalar x, Scalar y, Scalar z, uint32_t octaves)
		{
			return octave_noise(x, y, z, octaves) / weight(octaves);
		}

public:
		static uint32_t reseed(uint32_t seed = random::device())
		{
			for (size_t i = 0; i < 256; ++i) p[i] = static_cast<std::uint8_t>(i);

			std::shuffle(std::begin(p), std::begin(p) + 256, random::defaultPRNG(seed));

			for (size_t i = 0; i < 256; ++i) p[256 + i] = p[i];
			
			return seed;
		}

		template <typename Vec2>
		static constexpr Scalar noise2D(const Vec2& v) { return noise(static_cast<Scalar>(v.x), static_cast<Scalar>(v.y), 0); }
		template <typename Vec3>
		static constexpr Scalar noise3D(const Vec3& v) { return noise(static_cast<Scalar>(v.x), static_cast<Scalar>(v.y), static_cast<Scalar>(v.z)); }
		template <typename S>
		static constexpr Scalar noise1D(S x) { return noise(static_cast<Scalar>(x), 0, 0); }
		template <typename S>
		static constexpr Scalar noise2D(S x, S y) { return noise(static_cast<Scalar>(x), static_cast<Scalar>(y), 0); }
		template <typename S>
		static constexpr Scalar noise3D(S x, S y, S z) { return noise(static_cast<Scalar>(x), static_cast<Scalar>(y), static_cast<Scalar>(z)); }

		template <typename Vec2, typename Ti, typename = std::enable_if_t<std::is_integral<Ti>::value>>
		static constexpr Scalar octave_noise2D(const Vec2& v, Ti octaves)
		{
			return octave_noise(
				static_cast<Scalar>(v.x), 
				static_cast<Scalar>(v.y), 
				0,
				static_cast<uint32_t>(octaves));
		}
		template <typename Vec3, typename Ti, typename = std::enable_if_t<std::is_integral<Ti>::value>>
		static constexpr Scalar octave_noise3D(const Vec3& v, Ti octaves)
		{
			return octave_noise(
				static_cast<Scalar>(v.x),
				static_cast<Scalar>(v.y),
				static_cast<Scalar>(v.z),
				static_cast<uint32_t>(octaves));
		}
		template <typename S, typename Ti, typename = std::enable_if_t<std::is_integral<Ti>::value>>
		static constexpr Scalar octave_noise1D(S x, Ti octaves)
		{
			return octave_noise(
				static_cast<Scalar>(x),
				0,
				0,
				static_cast<uint32_t>(octaves));
		}
		template <typename S, typename Ti, typename = std::enable_if_t<std::is_integral<Ti>::value>>
		static constexpr Scalar octave_noise2D(S x, S y, Ti octaves)
		{
			return octave_noise(
				static_cast<Scalar>(x),
				static_cast<Scalar>(y),
				0,
				static_cast<uint32_t>(octaves));
		}
		template <typename S, typename Ti, typename = std::enable_if_t<std::is_integral<Ti>::value>>
		static constexpr Scalar octave_noise3D(S x, S y, S z, Ti octaves)
		{
			return octave_noise(
				static_cast<Scalar>(x),
				static_cast<Scalar>(y),
				static_cast<Scalar>(z),
				static_cast<uint32_t>(octaves));
		}

		template <typename Vec2, typename Ti, typename = std::enable_if_t<std::is_integral<Ti>::value>>
		static constexpr Scalar normalised_octave_noise2D(const Vec2& v, Ti octaves)
		{
			return normalised_octave_noise(
				static_cast<Scalar>(v.x), 
				static_cast<Scalar>(v.y), 
				0, 
				static_cast<uint32_t>(octaves));
		}
		template <typename Vec3, typename Ti, typename = std::enable_if_t<std::is_integral<Ti>::value>>
		static constexpr Scalar normalised_octave_noise3D(const Vec3& v, Ti octaves)
		{
			return normalised_octave_noise(
				static_cast<Scalar>(v.x),
				static_cast<Scalar>(v.y),
				static_cast<Scalar>(v.z),
				static_cast<uint32_t>(octaves));
		}
		template <typename S, typename Ti, typename = std::enable_if_t<std::is_integral<Ti>::value>>
		static constexpr Scalar normalised_octave_noise1D(S x, Ti octaves)
		{
			return normalised_octave_noise(
				static_cast<Scalar>(x),
				0,
				0,
				static_cast<uint32_t>(octaves));
		}
		template <typename S, typename Ti, typename = std::enable_if_t<std::is_integral<Ti>::value>>
		static constexpr Scalar normalised_octave_noise2D(S x, S y, Ti octaves)
		{
			return normalised_octave_noise(
				static_cast<Scalar>(x),
				static_cast<Scalar>(y),
				0,
				static_cast<uint32_t>(octaves));
		}
		template <typename S, typename Ti, typename = std::enable_if_t<std::is_integral<Ti>::value>>
		static constexpr Scalar normalised_octave_noise3D(S x, S y, S z, Ti octaves)
		{
			return normalised_octave_noise(
				static_cast<Scalar>(x),
				static_cast<Scalar>(y),
				static_cast<Scalar>(z),
				static_cast<uint32_t>(octaves));
		}
	};

	using defaultNoise = PerlinNoise<defaultType>;
	using noise = defaultNoise;
}