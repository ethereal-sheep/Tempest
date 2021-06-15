#pragma once
#include "Core.h"
#include "Util.h"
#include "Events/Events.h"
#include <any>

namespace Tempest
{
	class script_exception : public std::exception
	{
	public:
		script_exception(const string& err_msg = "sparse set exception thrown!") : msg(err_msg) {}
		const char* what() const noexcept override { return msg.c_str(); }
	private:
		string msg;
	};

	/**
	 * @brief Pure virtual interface class for scripts.
	 */
	class script
	{
	public:

		virtual ~script() = default;

		/**
		 * @brief Operator() overload. Calls the underlying function.
		 */
		virtual void operator()() = 0;

		/**
		 * @brief function to call to invoke underlying event script. If none is overriden, 
		 * throws script_exception
		 */
		virtual void event_call(const Event&) { throw script_exception("Bad Event call!"); }

		/**
		 * @brief Operator[] overload. Gets the output value at the function's
		 * index.
		 * @param index Index of the output pin
		 * @return Returns an any variable
		 */
		virtual std::any operator[](size_t index) = 0;

		/**
		 * @brief Sets the input script and the index to query the value
		 * @param index Index of the input pin
		 * @param input Input script
		 * @param node_index Index of the output pin at the input script
		 * @return Pointer to this script
		 */
		virtual script* set_input(size_t index, script* input, size_t node_index) = 0;
		
		/**
		 * @brief Sets the next script to call after this script
		 * @param input Next script to call
		 * @param index Index of the next script to call (usually 0; > 0 for 
		 * branching or sequence scripts)
		 * @return Pointer to this script
		 */
		virtual script* set_next(script* input, size_t index = 0) = 0;
		
		/**
		 * @brief Registers a callback to call when the script is called
		 * @param fn A callback function to call
		 */
		void register_callback(std::function<void()> fn)
		{
			callback = std::move(fn);
		}

	protected:
		std::function<void()> callback;
	};


	/**
	 * @brief Container type for input scripts. Each input is set by passing 
	 * the input script and index of the value. Values are queried
	 * by accessing the input scripts [] fn with the index provided. 
	 */
	template<typename Ret, typename... Args>
	class input_values;

	template<typename Ret, typename... Args>
	class input_values<Ret(Args...)>
	{
		std::array<std::pair<script*, size_t>, sizeof...(Args)> inputs;

	public:
		using ret_type = Ret;

		template<size_t, typename T>
		T&& get(T&& t)
		{
			return std::forward<T>(t);
		}

		template<size_t i, size_t N>
		auto get(std::_Ph<N>)
		{
			static_assert(i < sizeof...(Args));
			const auto& [input, index] = inputs[N - 1];
			if (!input)
				return std::tuple_element_t<i, std::tuple<Args...>>{};
			return std::any_cast<std::tuple_element_t<i, std::tuple<Args...>>>((*input)[index]);
		}

		void set_input(size_t index, script* node, size_t node_index)
		{
			inputs[index] = std::make_pair(node, node_index);
		}
	};

	/**
	 * @brief Binder class to store the placeholders until the script is
	 * ready to be called. When the Binder is called, the inputs and the fn is
	 * binded with reference to the binded placeholders.
	 */
	template<typename... Args>
	class input_binder
	{
		std::tuple<Args...> binded;

		template<typename Func, typename TScripts, size_t... Ns>
		auto call(Func f,
			TScripts inputs,
			std::index_sequence<Ns...>)
		{
			return f(inputs.get<Ns>(std::get<Ns>(binded))...);
		}

	public:
		input_binder(Args&&... args)
			: binded{ std::forward<Args>(args)... }
		{
		}

		template<typename Func, typename TScripts>
		auto operator()(Func f, TScripts inputs)
		{
			return call(f,
				inputs,
				std::make_index_sequence<sizeof...(Args)>{});
		}

		static constexpr size_t size()
		{
			return sizeof...(Args);
		}
	};

	/**
	 * @brief Runtime getter to get values of a tuple at runtime via a runtime
	 * index variable. Recusively calls the upper fn until N == index, then 
	 * the func is invoked (see input_binder).
	 */
	template<typename Func, typename Tuple, size_t N = 0>
	void runtime_get(Func func, Tuple& tuple, size_t index)
	{
		if (N == index)
			return std::invoke(func, std::get<N>(tuple));

		if constexpr (N + 1 < std::tuple_size_v<Tuple>)
			return runtime_get<Func, Tuple, N + 1>(func, tuple, index);
	}

	/**
	 * @brief Container to store the output values of a script when it has been
	 * called. Accessing the values before the function is called is UDB.
	 */
	template <typename T>
	class script_output;

	template <>
	class script_output<void>{};

	template <typename ... Args>
	class script_output<std::tuple<Args...>>
	{
		std::tuple<Args...> data;

	public:
		script_output& operator=(std::tuple<Args...> t)
		{
			data = std::move(t);
			return *this;
		}

		auto operator[](size_t index)
		{
			std::any ret;
			auto getter = [&ret](auto x) { ret = x; };
			runtime_get(getter, data, index);

			return ret;
		}
	};

	/**
	 * @brief Container to store the output values of a script when it has been
	 * called. Values are forwarded and not copied.
	 */
	template <typename T>
	class script_forward;

	template <>
	class script_forward<void>
	{
	};

	template <typename ... Args>
	class script_forward<std::tuple<Args...>>
	{
		std::tuple<Args...> data;

	public:
		script_forward(std::tuple<Args...> t) : data{ std::move(t) } {}

		auto operator[](size_t index)
		{
			std::any ret;
			auto getter = [&ret](auto x) { ret = x; };
			runtime_get(getter, data, index);

			return ret;
		}
	};


	/**
	 * @brief Runtime Script. When script is called, input values are queried
	 * and underlying fn is called. Output values are stored and can be queried
	 * by the next script (accessing values before script is called is UDB).
	 * 
	 * Once done, the next script is called.
	 */
	template<typename Func, typename Ret, typename TInputBinder, typename TInputScripts>
	class runtime_script : public script
	{
		Func func;
		script_output<Ret> outputs;
		TInputBinder binder;
		TInputScripts inputs;

		script* next = nullptr;

	public:
		runtime_script(Func f, TInputBinder _binder)
			: func{ f }, binder{ std::move(_binder) }{}

		void operator()() override
		{
			if (callback)
				callback();

			if constexpr (std::is_same_v<Ret, void>)
				binder(func, inputs);
			else
				outputs = binder(func, inputs);

			if (next)
				(*next)();
		}

		std::any operator[](size_t index) override
		{
			if constexpr (std::is_same_v<Ret, void>)
				return {};
			else
				return outputs[index];
		}

		script* set_input(size_t index, script* input, size_t node_index) override
		{
			inputs.set_input(index, input, node_index);
			return this;
		}

		script* set_next(script* input, size_t) override
		{
			next = input;
			return this;
		}

	};

	/**
	 * @brief Constexpr Script. Script is never called. Values are only 
	 * queried. Unlike runtime_script, constexpr script's values can be 
	 * accessed at anytime and behaviour is well defined. No next scripts are 
	 * called by a constexpr script.
	 */
	template<typename Func, typename Ret, typename TInputBinder, typename TInputScripts>
	class constexpr_script : public script
	{
		Func func;
		TInputBinder binder;
		TInputScripts inputs;

	public:
		constexpr_script(Func f, TInputBinder _binder)
			: func{ f }, binder{ std::move(_binder) }{}

		void operator()() override	{
			// do nothing
		}

		std::any operator[](size_t index) override
		{
			if constexpr (std::is_same_v<Ret, void>)
				return {};
			else
				return script_forward<Ret>(binder(func, inputs))[index];
		}

		script* set_input(size_t index, script* input, size_t node_index) override
		{
			inputs.set_input(index, input, node_index);
			return this;
		}

		script* set_next(script*, size_t) override
		{
			return this;
		}
	};

	/**
	 * @brief Data Script. Script is never called. Script stores a single 
	 * value type (can be ref) and returns it when queried.
	 */
	template<typename T>
	class data_script : public script
	{
		T value;

	public:
		data_script(T value) : value{ std::forward<T>(value) } { }

		void operator()() override
		{
			// do nothing
		}

		std::any operator[](size_t index [[maybe_unused]] ) override
		{
			return value;
		}

		script* set_input(size_t, script*, size_t) override
		{
			// no input to set
			return this;
		}

		script* set_next(script*, size_t) override
		{
			// no output to set
			return this;
		}
	};

	/**
	 * @brief Branch Script. Branches flows based on input type. Underlying
	 * script returns a integer value that specifies the next flow to call.
	 */
	template<typename Func, size_t NOutputs, typename TInputBinder, typename TInputScripts>
	class branch_script : public script
	{
		Func func;
		int choice;
		TInputBinder binder;
		TInputScripts inputs;
		std::array<script*, NOutputs> arr = { 0 };

	public:
		branch_script(Func f, TInputBinder _binder)
			: func{ f }, binder{ std::move(_binder) }{}

		void operator()() override
		{
			if (callback)
				callback();

			choice = std::get<0>(binder(func, inputs));

			// if out of range, return last
			if (choice < 0 || choice >= NOutputs)
				choice = NOutputs - 1;

			// check if choice is not nullptr
			if (arr[choice])
				(*arr[choice])();

		}

		std::any operator[](size_t index [[maybe_unused]] ) override
		{
			return choice;
		}

		script* set_input(size_t index, script* input, size_t node_index) override
		{
			inputs.set_input(index, input, node_index);
			return this;
		}

		script* set_next(script* input, size_t index) override
		{
			if (index >= NOutputs)
				return this;

			arr[index] = input;
			return this;
		}
	};

	/**
	 * @brief Sequence Script. Calls a set of output scripts in order.
	 */
	template<size_t NOutputs>
	class sequence_script : public script
	{
		std::array<script*, NOutputs> arr = { 0 };

	public:
		sequence_script() {}

		void operator()() override
		{
			if (callback)
				callback();

			for (auto s : arr)
				if (s) (*s)();
		}

		std::any operator[](size_t index [[maybe_unused]] ) override
		{
			return false;
		}

		script* set_input(size_t, script*, size_t) override
		{
			// do ntohing
			return this;
		}

		script* set_next(script* input, size_t index) override
		{
			if (index >= NOutputs)
				return this;

			arr[index] = input;
			return this;
		}
	};
	
	/**
	 * @brief Event Script. Calls a set of output scripts in order.
	 */
	template<typename Func, typename Ret>
	class event_script : public script
	{
		Func func;
		script_output<Ret> outputs;

		script* next = nullptr;

	public:
		event_script(Func f) : func(f) {}

		void operator()() override
		{
			// does nothing
		}

		std::any operator[](size_t index [[maybe_unused]] ) override
		{
			return false;
		}

		script* set_input(size_t, script*, size_t) override
		{
			// do ntohing
			return this;
		}

		script* set_next(script* input, size_t ) override
		{
			next = input;
			return this;
		}

		// specially for events only
		void event_call(const Event& e) override
		{
			if (callback)
				callback();

			if constexpr (!std::is_same_v<Ret, void>)
				outputs = func(e);
			else
				func(e);

			if (next)
				(*next)();
		}
	};


	/**
	 * @brief Creates a 
	 * 
	 * Runtime Script. When script is called, input values are queried
	 * and underlying fn is called. Output values are stored and can be queried
	 * by the next script (accessing values before script is called is UDB).
	 *
	 * Once done, the next script is called.
	 */
	template<typename FuncType, typename Func, typename... Args>
	[[nodiscard]] auto CreateRuntimeScript(Func f, Args&&... args)
	{
		using ret_type = typename input_values<FuncType>::ret_type;
		return make_uptr<runtime_script<
			Func, ret_type, input_binder<Args...>, input_values<FuncType>>>
			(f, input_binder<Args...>{ std::forward<Args>(args)... });
	}

	/**
	 * @brief Creates a
	 * 
	 * Constexpr Script. Script is never called. Values are only
	 * queried. Unlike runtime_script, constexpr script's values can be 
	 * accessed at anytime and behaviour is well defined. No next scripts are 
	 * called by a constexpr script.
	 */
	template<typename FuncType, typename Func, typename... Args>
	[[nodiscard]] auto CreateConstexprScript(Func f, Args&&... args)
	{
		using ret_type = typename input_values<FuncType>::ret_type;
		return make_uptr<constexpr_script<
			Func, ret_type, input_binder<Args...>, input_values<FuncType>>>
			(f, input_binder<Args...>{ std::forward<Args>(args)... });
	}

	/**
	 * @brief Creates a
	 * 
	 * Data Script. Script is never called. Script stores a single
	 * value type (can be ref) and returns it when queried.
	 */
	template<typename T>
	[[nodiscard]] auto CreateDataScript(T value)
	{
		return make_uptr<data_script<T>>(std::forward<T>(value));
	}

	/**
	 * @brief Creates a
	 * 
	 * Branch Script. Branches flows based on input type. Underlying
	 * script returns a integer value that specifies the next flow to call.
	 */
	template<typename FuncType, size_t NOutputs, typename Func, typename... Args>
	[[nodiscard]] auto CreateBranchScript(Func f, Args&&... args)
	{
		return make_uptr<branch_script<
			Func, NOutputs, input_binder<Args...>, input_values<FuncType>>>
			(f, input_binder<Args...>{ std::forward<Args>(args)...}
		);
	}
	/**
	 * @brief Creates a
	 *
	 * Sequence Script. Calls a set of output scripts in order.
	 */
	template<size_t NOutputs>
	[[nodiscard]] auto CreateSequenceScript()
	{
		return make_uptr<sequence_script<NOutputs>>();
	}
	
	/**
	 * @brief Creates a
	 *
	 * Event Script. Entry point to a bunch of scripts.
	 */
	template<typename Func>
	[[nodiscard]] auto CreateEventScript(Func f)
	{
		return make_uptr<event_script<Func, std::invoke_result_t<Func, Event>>>(f);
	}

	using script_ptr = tuptr<script>;
}