#pragma once

namespace Tempest
{
	class Instance;

	struct IAction
	{
		virtual const char* Name() = 0;
		virtual void Undo(Instance&) = 0;
		virtual void Redo(Instance&) = 0;
		virtual ~IAction() = default;
	};

	using Action = IAction;
}