#include "AssetManagerWindow.h"


namespace UI
{
	void AssetManagerWindow::Init()
	{
	}

	void AssetManagerWindow::Show()
	{
		if (m_toggle)
		{
			if (ImGui::Begin("Asset Manager", nullptr, m_WindowFlags))
			{

			}
			ImGui::End();
		}
	}

	void AssetManagerWindow::Shutdown()
	{
	}

	
}