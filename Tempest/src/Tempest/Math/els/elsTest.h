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
#include <algorithm>

#include "elsHeader.h"
#include "elsValue.h"

#include "elsMatrix2.h"
#include "elsMatrix3.h"
#include "elsMatrix4.h"
#include "elsVector2.h"
#include "elsVector3.h"
#include "elsVector4.h"
#include "elsQuaternion.h"
#include "elsTransform2.h"
#include "elsTransform3.h"

#include "elsCompare.h"
#include "elsNoise.h"
#include "elsRandom.h"

namespace els
{
	template <typename T>
	struct strangeVec2
	{
		T x, y;
	};
	template <typename T>
	struct strangeVec3
	{
		T x, y, z;
	};
	template <typename T>
	struct strangeVec4
	{
		T x, y, z, w;
	};

	struct strangeVec2i
	{
		int x, y;
	};
	struct strangeVec3i
	{
		int x, y, z;
	};
	struct strangeVec4i
	{
		int x, y, z, w;
		constexpr strangeVec4i() : strangeVec4i{0}{}
		constexpr strangeVec4i(int i) : x{ i }, y{ i }, z{ i }, w{ i } { }


	};
	constexpr vec4 test_constexpr() { return vec4{ 1 }; }

	namespace test
	{
		static bool test_math()
		{
			els::lerp(1.f, 1, 2);

			constexpr float test = els::gcd(2,2);

			

			return true;
		}
		static bool test_random()
		{
			els::random::reseed();



			return true;
		}

		static bool test_mat2_functions()
		{
			vec2f tester = vec2f::i;
			vec2b testerB;
			vec2f testerF;
			vec2f testerF2 = vec2f::i;
			vec2f testerSum = vec2f::j;



			els::mat2 test = els::mat2::I;



			return true;
		}

		static bool test_vec2_functions()
		{
			vec2f tester = vec2f::i;
			vec2b testerB;
			vec2f testerF;
			vec2f testerF2 = vec2f::i;
			vec2f testerSum = vec2f::j;
			strangeVec2i strange_i;

			testerSum = els::lerp(testerF, testerF2, 0.5f);
			testerSum = els::normalized(testerSum);
			testerSum = els::projection(testerSum, testerF2);
			els::normalize(testerSum);

			els::dot(testerSum, testerF);
			els::cross(testerSum, testerF);
			els::distance(testerSum, testerF);
			els::distance2(testerSum, testerF);
			els::length(testerSum);
			els::length2(testerSum);
			els::angle(testerSum, testerF);


			testerB = to_vec2<bool>(strange_i);

			return true;
		}

		static bool test_vec3_functions()
		{
			vec3f tester = vec3f::i;
			vec3b testerB;
			vec3f testerF = vec3f::i;
			vec3f testerF2 = vec3f::j;
			vec3f testerSum = vec3f::k;
			strangeVec3i strange_i;

			testerSum = els::lerp(testerF, testerF2, 0.5f);
			testerSum = els::normalized(testerSum);
			testerSum = els::projection(testerSum, testerF2);
			els::normalize(testerSum);

			els::dot(testerSum, testerF);
			els::cross(testerSum, testerF);
			els::distance(testerSum, testerF);
			els::distance2(testerSum, testerF);
			els::length(testerSum);
			els::length2(testerSum);
			els::angle(testerSum, testerF);

			testerB = to_vec3<bool>(strange_i);
			return true;
		}

		static bool test_vec4_functions()
		{
			vec4f tester = vec4f::i;
			vec4b testerB;
			vec4f testerF;
			vec4f testerF2 = vec4f::j;
			vec4f testerSum = vec4f::k;

			strangeVec4i strange_i;

			testerSum = els::lerp(testerF, testerF2, 0.5f);
			testerSum = els::normalized(testerSum);
			testerSum = els::projection(testerSum, testerF2);
			els::normalize(testerSum);

			els::dot(testerSum, testerF);
			//els::cross(testerSum, testerF);
			els::distance(testerSum, testerF);
			els::distance2(testerSum, testerF);
			els::length(testerSum);
			els::length2(testerSum);
			els::angle(testerSum, testerF);

			testerB = to_vec4<bool>(strange_i);


			return true;
		}

		static bool test_mat3_functions()
		{
			els::mat3f tester = els::mat3f::I;

			els::vec2 testerB = els::make_vec2(els::value_ptr(tester));


			return true;
		}
	}

}