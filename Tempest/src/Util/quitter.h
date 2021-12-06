#pragma once

namespace Tempest
{
	struct quitter
	{
		bool quit = false;
	};

	inline quitter& get_quitter()
	{
		static quitter q;
		return q;
	}
}