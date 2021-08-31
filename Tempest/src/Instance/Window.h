#pragma once

namespace Tempest
{
	class Instance;

	class IWindow
	{
	public:
		virtual ~IWindow() {};

		virtual const char* window_name() = 0;

		// called by window_manager
		virtual void init(Instance&) {};

		// called by window_manager
		virtual void exit(Instance&) {};

		// called by window_manager
		virtual void show(Instance&) = 0;

	protected:
		int window_flags = 0;

	public:
		bool visible = true;
	};

	using Window = IWindow;
}