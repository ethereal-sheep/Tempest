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
#include "Graph/graph.h"
#include "Events/Events.h"
#include "Util/sparse_scripts.h"

namespace Tempest
{
	/**
	 * @brief var_key for runtime variables.
	 */
	struct var_key
	{
		Entity entity;
		string var_name;

		bool operator==(const var_key& rhs) const
		{
			return rhs.entity == entity && rhs.var_name == var_name;
		}
	};

	/**
	 * @brief var_hash for runtime variables
	 */
	struct var_hash
	{
		size_t operator()(const var_key& key) const
		{
			return (std::hash<string>()(key.var_name) ^ key.entity);
		}
	};

	/**
	 * @brief Doesn't matter.
	 */
	struct var_serial
	{
		string operator()(const var_key& key) const
		{
			return key.var_name;
		}
		var_key operator[](const string& key) const
		{
			return { 0, key };
		}
	};

	/**
	 * @brief SRM_exception.
	 */
	class SRM_exception : public std::exception
	{
	public:
		SRM_exception(const string& err_msg = "SRM exception thrown!") : msg(err_msg) {}
		const char* what() const noexcept override { return msg.c_str(); }
	private:
		string msg;
	};

	/**
	 * @brief Script Runtime Manager. Manages all runtime scripts of
	 * a runtime instance, including the events that call them.
	 */
	class SRM
	{
		void try_invoke(script* callable, const Event& e)
		{
			try
			{
				callable->event_call(e);
			}
			catch (const std::exception& a)
			{
				LOG_ERROR(string("SRM: Exception thrown when invoking script! ") + a.what());
			}
		}

		template<typename Event>
		[[nodiscard]] bool listener_exist() const
		{
			return listeners.count(t_hash<Event>());
		}

	public:
		using Scripts = tvector<tuptr<script>>;
		using Listeners = tmap<size_t, sparse_scripts>;
		using Variables = unordered_var_set<var_key, var_hash, var_serial>;

		/**
		 * @brief Constructor
		 */
		SRM(m_resource* mem = std::pmr::get_default_resource()) : 
			memory_resource{ mem }, scripts{ mem }, listeners{ mem } {}

		/**
		 * @brief var_key for runtime variables.
		 */
		template <typename TEvent>
		void register_listener(id_t id, script* callable)
		{
			static_assert(std::is_base_of_v<Event, TEvent>, "Must inherit from Event");
			static_assert(!std::is_same_v<Event, TEvent>, "Event type cannot be IEvent");

			if (!callable) // bad script
				throw SRM_exception("SRM: Bad listener script!");

			if (!listener_exist<TEvent>())
				listeners.emplace(make_tpair(t_hash<TEvent>(), sparse_scripts(memory_resource)));

			listeners[t_hash<TEvent>()].emplace(id, callable);
		}

		/**
		 * @brief var_key for runtime variables.
		 */
		template <typename TEvent, typename... TArgs>
		void instant_dispatch_to_id(id_t id, TArgs&&... args)
		{
			TEvent new_event{ std::forward<TArgs>(args)... };
			
			if (!listener_exist<TEvent>()) return;

			if (!listeners[t_hash<TEvent>()].contains(id)) return;

			try_invoke(listeners[t_hash<TEvent>()].get(id), new_event);
		}

		/**
		 * @brief var_key for runtime variables.
		 */
		template <typename TEvent, typename... TArgs>
		void instant_dispatch(TArgs&&... args)
		{
			TEvent new_event{ std::forward<TArgs>(args)... };

			if (!listener_exist<TEvent>()) return;

			for (auto [id, script] : listeners[t_hash<TEvent>()])
				try_invoke(script, new_event);
		}

		/**
		 * @brief TODO
		 */
		template <typename TEvent, typename... TArgs>
		void queue_dispatch(TArgs&&... args)
		{

		}

		/**
		 * @brief TODO
		 */
		void dispatch_all_in_queue()
		{

		}

		/**
		 * @brief Adds a script to the SRM to be handled.
		 * @param callable unique pointer of a script
		 */
		script* add_script(script_ptr&& callable)
		{
			if (!callable) // bad script
				throw SRM_exception("SRM: Bad script!");

			auto& it = scripts.emplace_back(std::move(callable));
			return it.get();
		}

		/**
		 * @brief Adds a variable to the SRM to be handled.
		 * @param name String identifier to the variable
		 * @param var var_data variable
		 */
		var_data* add_variable(const string& name, const var_data& var)
		{
			var_key key{ INVALID, name };
			auto* data = variables.insert(key, var);
			if (!data)
				throw SRM_exception("SRM: Bad variable!");

			return data;
		}

		/**
		 * @brief Adds a variable to the SRM to be handled.
		 * @param id id identifier to the variable
		 * @param name String identifier to the variable
		 * @param var var_data variable
		 */
		var_data* add_variable_to_id(id_t id, const string& name, const var_data& var)
		{
			var_key key{ id, name };
			auto* data = variables.insert(key, var);
			if (!data)
				throw SRM_exception("SRM: Bad variable!");

			return data;
		}
		/**
		 * @brief Gets a variable stored in SRM.
		 * @param name String identifier to the variable
		 */
		var_data* get_variable(const string& name)
		{
			return variables.at_if({ INVALID, name });
		}

		/**
		 * @brief Gets a variable stored in SRM.
		 * @param id id identifier to the variable
		 * @param name String identifier to the variable
		 */
		var_data* get_variable_to_id(id_t id, const string& name)
		{
			return variables.at_if({ id, name });
		}

		/**
		 * @brief Clears the SRM.
		 */
		void clear()
		{
			listeners.clear();
			scripts.clear();
			variables.clear();
		}

	private:
		m_resource* memory_resource;
		
		Scripts scripts;
		Listeners listeners;
		Variables variables;
	};
}