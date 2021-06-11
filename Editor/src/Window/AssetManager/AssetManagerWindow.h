#pragma once
#include "../Window.h"
#include <unordered_map>
#include "AssetInfo.h"


namespace UI
{
	class AssetManagerWindow final : public Window
	{
	public:
		AssetManagerWindow()
			: Window{ "Asset Manager" }
		{
		}

		void Init() override;
		void Show() override;
		void Shutdown() override;
		void Unload();
	private:
		std::string _WindowName;
		ImVec2 m_AssetsWinSize;
		bool _subActive;
		std::string _AssetIndex;
		std::string _FileIndex;
		std::vector<std::string> _fullPathName;
		std::vector<AssetsInfo> _assetsInfo;
		std::vector<std::string> _FolderNames;

		std::unordered_map<std::string, std::string> _TreeContents;
		std::unordered_map<std::string, std::vector<AssetsInfo>> _FolderListContents;
		std::unordered_map<std::string, std::vector<AssetsInfo>> _SubFolderListContents;
	};
}