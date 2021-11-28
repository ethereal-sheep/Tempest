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
#include "type_traits.h"

namespace Tempest::algo
{
	/**
	 * @brief Gets the next name not found in iterator range.
	 * @tparam It Iterator type.
	 * @tparam Pred Predicate that accepts It::value_type as first arg, and string or const string&
	 * type as second arg.
	 * @param name Name value to be checked.
	 * @param begin Iterator to beginning of range.
	 * @param end Iterator to end of range.
	 * @param func Predicate function. Accepts It::value_type as first arg, and string or const string&
	 * type as second arg.
	 * @param counter unsigned value to append to end of range until a non-match is found. Default = 0.
	 * @return New name string not found in range.
	 */
	template <typename It, typename Pred>
	string get_next_name(const string& name, It begin, It end, Pred func, uint32_t counter = 0)
	{
		if (begin == end)
			return name;

		string newName = name;
		It find = begin;

		while (true)
		{
			find = end;
			for (It it = begin; it != end; ++it)
			{
				if (func(*it, newName))
				{
					find = it;
					break;
				}
			}

			if (find == end)
				break;

			newName = name + std::to_string(counter);
			counter++;
		}
		return newName;
	}

	/**
	 * @brief Gets the next name not found in an associative container. Uses count function to find
	 * matches.
	 * @tparam AssocCont Associative container type.
	 * @param name Name value to be checked.
	 * @param cont Reference to an associative container with string as key.
	 * @param counter unsigned value to append to end of range until a non-match is found. Default = 0.
	 * @return New name string not found in container.
	 */
	template <typename AssocCont>
	string get_next_name(const string& name, const AssocCont& cont, uint32_t counter = 0)
	{
		string newName = name;

		while (cont.count(newName))
		{
			newName = name + std::to_string(counter);
			counter++;
		}
		return newName;
	}
	/**
	 * @brief Splits a string or string_view into a vector of strings delimited by a delimiter
	 * @tparam TString a string type template
	 * @param s Copy of string to split
	 * @param delim Delimiter
	 * @return Vector of strings
	 */
	template< typename TString>
	tvector<string> delimited_split(TString s, const string& delim)
	{
		tvector<string> temp;
		size_t pos = 0;
		string token;
		while ((pos = s.find(delim)) != string::npos) {
			token = s.substr(0, pos);
			temp.emplace_back(std::move(token));
			s.erase(0, pos + delim.length());
		}
		temp.emplace_back(std::move(s));

		return std::move(temp);
	}
	/**
	 * @brief Splits a string or string_view into a vector of strings delimited by a delimiter
	 * @tparam TString a string type template
	 * @param s Copy of string to split
	 * @param delim Delimiter
	 * @return Vector of strings
	 */
	template< typename TString>
	tvector<string> delimited_split(TString s, char delim)
	{
		tvector<string> temp;
		size_t pos = 0;
		string token;
		while ((pos = s.find(delim)) != string::npos) {
			token = s.substr(0, pos);
			temp.emplace_back(std::move(token));
			s.erase(0, pos + delim.length());
		}
		temp.emplace_back(std::move(s));

		return std::move(temp);
	}

	/**
	 * @brief Erases elements from the vector using the erase-remove idiom.
	 * @tparam T value_type of vector
	 * @tparam Pred predicate type
	 * @param cont Reference to a vector to erase elements from
	 * @param pred Predicate function. Returns true if value should be erased.
	 */
	template <typename T, typename Pred>
	void erase_remove_if(tvector<T>& cont, Pred pred)
	{
		cont.erase(std::remove_if(cont.begin(), cont.end(), pred), cont.end());
	}

	/**
	 * @brief Erases elements from the vector using the erase-remove idiom, and moves it to
	 * another vector
	 * @tparam TMovable value_type of vector that is movable
	 * @tparam Pred predicate type
	 * @param cont Reference to a vector to erase elements from
	 * @param move_to Reference to a vector to move elements to
	 * @param pred Predicate function. Returns true if value should be erased.
	 */
	template <typename TMovable, typename Pred>
	void erase_remove_if(tvector<TMovable>& cont, tvector<TMovable>& move_to, Pred pred)
	{
		auto last_it = std::remove_if(cont.begin(), cont.end(), pred);
		move_to.insert(move_to.end(), std::make_move_iterator(last_it), std::make_move_iterator(cont.end()));
		cont.erase(last_it, cont.end());
	}

	/**
	 * @brief Discards key from the set using a predicate.
	 * @tparam T value_type of set
	 * @tparam Pred predicate type
	 * @param cont Reference to a unordered_set to erase elements from
	 * @param pred Predicate function. Returns true if value should be erased.
	 */
	template <typename T, typename Pred>
	void discard_if(tset<T>& cont, Pred pred)
	{
		for (auto it{ cont.begin() }, end{ cont.end() }; it != end; ) {
			if (pred(*it))
				it = cont.erase(it);
			else
				++it;
		}
	}
	/**
	 * @brief Discards key from the set using a predicate.
	 * @tparam T value_type of set
	 * @tparam Pred predicate type
	 * @param cont Reference to a unordered_set to erase elements from
	 * @param pred Predicate function. Returns true if value should be erased.
	 */
	template <typename TMovable, typename Pred>
	void discard_if(tset<TMovable>& cont, tset<TMovable>& move_to, Pred pred)
	{
		for (auto it{ cont.begin() }, end{ cont.end() }; it != end; ) {
			if (pred(*it))
				move_to.insert(cont.extract(*it));
			else
				++it;
		}
	}

}