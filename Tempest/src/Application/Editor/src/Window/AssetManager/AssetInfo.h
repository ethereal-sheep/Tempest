#include <String>



namespace UI
{
	enum class AssetType
	{
		AT_UNASSIGNED,
		AT_MODEL,
		AT_SHADER,
		AT_MATERIAL,
		AT_AUDIOCLIP,
		AT_TEXTURE,
		AT_PREFAB,
		AT_ANIMATION,
		AT_PLAYBACK
	};

	struct AssetsInfo
	{
		std::string _path;
		std::string _folder;
		std::string _subFolder;
		std::string _file;
		AssetType _type;
		bool _hasSubFolder;

		AssetsInfo() : _type{ AssetType::AT_UNASSIGNED }, _hasSubFolder{ false } {}

		AssetsInfo(std::string path) :
			_path{ path }
		{
			_folder = SetFolder(_path);
			_subFolder = SetSubFolder(_path);
			_file = SetFileName(_path);
			_type = SetAssetType(_file);
		}
	private:
		std::string removeMainFolder(std::string path)
		{
			size_t pos = path.find_first_of('\\');
			std::string temp = path.substr(pos + 1);
			return temp;
		}

		std::string SetFolder(std::string path)
		{
			std::string temp = path;
			std::string temp2 = removeMainFolder(temp);

			size_t pos = temp2.find('\\');

			if (pos != std::string::npos)
				temp2.erase(temp2.find('\\'));

			return temp2;
		}

		std::string SetSubFolder(std::string path)
		{
			std::string temp = removeMainFolder(path);

			size_t pos = temp.find('\\');

			if (pos == std::string::npos)
				return std::string();

			std::string temp2 = removeMainFolder(temp);
			pos = temp2.find('.');
			size_t pos2 = temp2.find('\\');

			if (pos != std::string::npos && pos2 == std::string::npos)
				return std::string();

			if (pos2 != std::string::npos)
				temp2.erase(temp2.find('\\'));

			_hasSubFolder = true;
			return temp2;
		}

		std::string SetFileName(std::string path)
		{
			size_t pos = path.find('.');

			if (pos == std::string::npos)
				return std::string();

			pos = path.find_last_of('\\');
			std::string temp = path.substr(pos + 1);
			return temp;
		}

		AssetType SetAssetType(std::string path)
		{

			return AssetType::AT_UNASSIGNED;
		}
	};
}


