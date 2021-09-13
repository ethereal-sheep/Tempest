#pragma once
#include "Core.h"
#include "TMath.h"
#include "dice.h"
#include "dice_distribution.h"

namespace Tempest
{

	constexpr uint32_t MAX_DICE_TYPE = 100;

	class dice_bag
	{
		uint32_t min_val = 0;
		uint32_t max_val = 0;
		// d to n
		tmap<uint32_t, uint32_t> bag;

	public:

		size_t min() const
		{
			return min_val;
		}

		size_t max() const
		{
			return max_val;
		}

		size_t size() const
		{
			return min_val;
		}

		size_t count(uint32_t d)
		{
			if (d > MAX_DICE_TYPE) 
				return 0;
			return bag[d];
		}

		float expected() const
		{
			return (max_val + min_val) / 2.f;
		}

		size_t spread() const
		{
			auto a = max_val + 1u;
			return a - min_val;
		}

		size_t roll() const
		{
			size_t sum = 0;
			for (auto [d, c] : bag)
				sum += dice(c, d);
			return sum;
		}
		size_t operator()() const
		{
			return roll();
		}

		void clear()
		{
			bag.clear();
		}

		bool add(uint32_t count, uint32_t d)
		{
			if (d > MAX_DICE_TYPE) return false;

			bag[d] += count;
			min_val += count;
			max_val += d * count;

			return true;
		}
		bool remove(uint32_t count, uint32_t d)
		{
			if (d > MAX_DICE_TYPE) return false;

			if (bag[d] < count) count = bag[d];

			bag[d] -= count;
			min_val -= count;
			max_val -= d * count;

			return true;
		}

		dice_distrubution get_distribution() const
		{
			dice_distrubution dd;
			for (auto [d, n] : bag)
			{
				dd.add(n, d);
			}
			return dd;
		}

		size_t count_d2() { return bag[2]; }
		size_t count_d4() { return bag[4]; }
		size_t count_d6() { return bag[6]; }
		size_t count_d8() { return bag[8]; }
		size_t count_d10() { return bag[10]; }
		size_t count_d12() { return bag[12]; }
		size_t count_d20() { return bag[20]; }
		size_t count_d100() { return bag[100]; }

		void add_d2(int32_t count) { add(count, 2); }
		void add_d4(int32_t count) { add(count, 4); }
		void add_d6(int32_t count) { add(count, 6); }
		void add_d8(int32_t count) { add(count, 8); }
		void add_d10(int32_t count) { add(count, 10); }
		void add_d12(int32_t count) { add(count, 12); }
		void add_d20(int32_t count) { add(count, 20); }
		void add_d100(int32_t count) { add(count, 100); }

		void remove_d2(int32_t count) { remove(count, 2); }
		void remove_d4(int32_t count) { remove(count, 4); }
		void remove_d6(int32_t count) { remove(count, 6); }
		void remove_d8(int32_t count) { remove(count, 8); }
		void remove_d10(int32_t count) { remove(count, 10); }
		void remove_d12(int32_t count) { remove(count, 12); }
		void remove_d20(int32_t count) { remove(count, 20); }
		void remove_d100(int32_t count) { remove(count, 100); }

	};
}