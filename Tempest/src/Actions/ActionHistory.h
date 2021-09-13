
#pragma once
#include "Core.h"
#include "IAction.h"

namespace Tempest
{
	class ActionHistory
	{
		// move this to some global settings
		// not implemented yet, currently still unlimited stack
		static constexpr size_t HISTORY_SIZE = 32;

		tvector<tuptr<IAction>> undo_stack;
		tvector<tuptr<IAction>> redo_stack;

	public:
		ActionHistory(m_resource* mr = std::pmr::get_default_resource()) :
			undo_stack(mr), redo_stack(mr) {}

		// modifiers
		template<typename TAction, typename... Args>
		void Commit(Args&&... args)
		{
			static_assert(std::is_base_of_v<IAction, TAction>, "Must inherit from Action");
			static_assert(!std::is_same_v<IAction, TAction>, "Event type cannot be IAction");

			auto action = make_uptr<TAction>(std::forward<Args>(args)...);

			// clear redo stack when committing new action
			redo_stack.clear();

			// push new action into undo stack
			undo_stack.push_back(std::move(action));

			// check if stack is of a certain length
			/*if (undo_stack.size() >= HISTORY_SIZE)
			{
				undo_stack.erase(undo_stack.begin());
			}*/
		}
		void Undo(Instance&);
		void Redo(Instance&);
		void Clear();

		// lookup
		bool HasUndo();
		bool HasRedo();
	};
}
