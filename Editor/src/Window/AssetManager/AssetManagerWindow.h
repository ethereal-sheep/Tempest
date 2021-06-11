#pragma once
#include "../Window.h"

namespace UI
{
	class AssetManagerWindow final : public Window
	{
	public:
		AssetManagerWindow()
			: Window{ "AssetManager" }
		{
		}

		void Init() override;
		void Show() override;
		void Shutdown() override;
	};
}