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
#include "Core.h"
#include "Util/range.h"
#include <numeric>

namespace Tempest
{
	class dice_distrubution
	{
		tvector<double> result;
		tvector<double> probability;
		tvector<double> win;
		tvector<double> lose;

		double total = 0.0;
		double mx = 0.0;

	public:

		dice_distrubution()
		{
			result = tvector<double>(1, 1);
		}

		dice_distrubution(size_t n, size_t d) : dice_distrubution()
		{
			add(n, d);
		}

		void add(size_t n, size_t d)
		{
			for (size_t i = 0; i < n; ++i)
			{
				tvector<double> curr(result.size() + d - 1, 0);
				for (size_t j = 0; j < result.size(); ++j)
					for (size_t k = 0; k < d; ++k)
						curr[j + k] += result[j];

				result = curr;
			}

			// calculate max and total
			double init = 0;
			total = std::accumulate(result.begin(), result.end(), init);

			// calculate max
			mx = *std::max_element(result.begin(), result.end());

			// calculate probability distribution
			probability = result;
			for (auto& p : probability)
				p /= total;

			win = probability;
			for (int i = (int)win.size() - 2; i >= 0; --i)
				win[i] += win[i + 1];

			lose = probability;
			for (int i = 1; i < lose.size(); ++i)
				lose[i] += lose[i - 1];
		}

		auto get_max() const
		{
			return mx;
		}

		auto get_max_probability() const
		{
			return mx / total;
		}

		auto get_total() const
		{
			return total;
		}

		const auto& get_probability_distibution() const
		{
			return probability;
		}
		const auto& get_probability_win() const
		{
			return win;
		}

		const auto& get_distribution() const
		{
			return result;
		}

		void clear()
		{
			result = tvector<double>(1, 1);
		}

	};
}