/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once
#include "Core.h"
#include "Writer.h"
#include "Reader.h"

namespace Tempest
{
	class Serializer
	{
	public:
		template <typename TData>
		bool Save(tpath path, const char* identifier, TData& data)
		{
			std::ofstream save(path);

			if (save.good())
			{
				Writer writer;
				std::string type = identifier;
				writer.StartObject();
				writer.StartMeta();
				writer.Member("Type", type);
				writer.EndMeta();
				writer.Member(identifier, data);
				writer.EndObject();
				save << writer.GetString();
				return true;
			}

			return false;
		}

		template <typename TData>
		bool Load(tpath path, const char* identifier, TData& data)
		{
			bool success = false;

			if (!std::filesystem::exists(path))
				return success;

			Reader reader(GetJson(path).c_str());

			if (!reader.HasError())
			{
				std::string type;
				reader.StartObject();
				reader.StartMeta();
				reader.Member("Type", type);
				if (type != identifier)
					return false;
				reader.EndMeta();
				reader.Member(identifier, data);
				reader.EndObject();
				success = true;
			}

			return success;
		}

		template <typename TData, typename TFunc>
		bool Load(tpath path, const char* identifier, TData& data, TFunc func)
		{
			bool success = false;

			if (!std::filesystem::exists(path))
				return success;

			Reader reader(GetJson(path).c_str());

			if (!reader.HasError())
			{
				std::string type;
				reader.StartObject();
				reader.StartMeta();
				reader.Member("Type", type);
				if (type != identifier)
					return false;
				func(reader);
				reader.EndMeta();
				reader.Member(identifier, data);
				reader.EndObject();
				success = true;
			}

			return success;
		}

		static std::string GetJson(const tpath& path)
		{
			std::string json = "";

			if (!std::filesystem::exists(path))
			{
				return json;
			}

			// Load file
			std::ifstream load(path, std::ios::in);

			if (load.good())
			{
				json = std::move(std::string{
					std::istreambuf_iterator<char>(load),
					std::istreambuf_iterator<char>() }
				);
			}
			return std::move(json);
		}

		static bool SaveJson(const std::filesystem::path& path, const char* json)
		{
			std::ofstream save(path);

			if (save.good())
			{
				save << json;
				return true;
			}

			return false;
		}
	};


}
