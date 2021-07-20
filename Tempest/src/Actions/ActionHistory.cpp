
#include "ActionHistory.h"

namespace Tempest
{
	void ActionHistory::Undo(Instance& instance)
	{
		// check if undo stack has any entry
		if (!HasUndo())
			return;

		// move the top out of stack and pop it from stack
		tuptr<IAction> action = std::move(undo_stack.back());
		undo_stack.pop_back();

		// call undo
		action->Undo(instance);

		// push redo
		redo_stack.push_back(std::move(action));
	}

	void ActionHistory::Redo(Instance& instance)
	{
		// check if redo stack has any entry
		if (!HasRedo())
			return;

		// move the top out of stack and pop it from stack
		tuptr<IAction> action = std::move(redo_stack.back());
		redo_stack.pop_back();

		// call redo
		action->Redo(instance);

		// push undo
		undo_stack.push_back(std::move(action));
	}
	void ActionHistory::Clear()
	{
		redo_stack.clear();
		undo_stack.clear();
	}

	bool ActionHistory::HasUndo()
	{
		return !undo_stack.empty();
	}

	bool ActionHistory::HasRedo()
	{
		return !redo_stack.empty();
	}
}
