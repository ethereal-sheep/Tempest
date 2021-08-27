#pragma once
#include "ECS/Components/Components.h"

namespace Tempest
{
	class Statline
	{
		template <typename Archiver>
		friend Archiver& operator&(Archiver& ar, Statline& component)
		{
			ar.StartObject();
			ar.Vector("Statline_Name", component.stats);
			return ar.EndObject();
		}

		Statline()
		{
			stats.push_back("HP");
			stats.push_back("ATK");
			stats.push_back("DEF");
		}

		bool remove_stat(size_t index)
		{
			if (index >= stats.size()) return false;

			stats.erase(stats.begin() + index);
			return true;
		}

		bool remove_stat(const string& name)
		{
			auto f = std::find(stats.begin(), stats.end(), name);
			if (f == stats.end()) return false;
			stats.erase(f);
			return true;
		}


		bool add_stat(const string& name)
		{
			if (stats.size() >= tc::STAT_TOTAL)
				return false;

			auto f = std::find(stats.begin(), stats.end(), name);
			if (f != stats.end()) return false;

			stats.push_back(name);
			return true;
		}

		bool rename_stat(size_t index, const string& newn)
		{
			if (index >= stats.size() || count(newn)) return false;

			stats[index] = newn;
		}
		bool rename_stat(const string& oldn, const string& newn)
		{
			if (!exist(oldn) || exist(newn)) return false;

			auto f = index_of_stat(oldn);
			stats[f] = newn;
			return true;
		}


		[[nodiscard]] size_t index_of_stat(const string& name) const
		{
			auto f = std::find(stats.begin(), stats.end(), name);
			return f - stats.begin();
		}

		[[nodiscard]] const string& operator[](size_t index) const
		{
			return stats[index];
		}


		[[nodiscard]] size_t count(const string& name) const
		{
			auto f = std::find(stats.begin(), stats.end(), name);
			if (f == stats.end()) return 0;
			return 1;
		}

		[[nodiscard]] bool exist(const string& name) const
		{
			auto f = std::find(stats.begin(), stats.end(), name);
			if (f == stats.end()) return false;
			return true;
		}

		[[nodiscard]] size_t size() const
		{
			return stats.size();
		}

		[[nodiscard]] const tvector<string>& get_stats() const
		{
			return stats;
		}

	private:

		tvector<string> stats;
	};
}