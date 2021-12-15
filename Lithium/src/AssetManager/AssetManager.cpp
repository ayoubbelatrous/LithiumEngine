#include "lipch.h"

#include "AssetManager.h"
#include "yaml-cpp/yaml.h"
#include <filesystem>
namespace Lithium
{

	template<typename T>
	T AssetManager(const std::string& path)
	{
		static_assert(false);
	}

	template<>
	Ref<Texture> AssetMananger::LoadAsset<Ref<Texture>>(const std::string& path)
	{
		
		if (_Cache.find(path) != _Cache.end())
		{
			CORE_LOG("loaded Texture Asset " << path);
			uint32_t id =  _Cache[path];
			//*_id = id;
			Ref<Texture> asset = _TextureCache[id];
			
			return asset;
		}
		else
		{
			std::filesystem::path _path = path;
			Ref<Texture>texture =  CreateRef<Texture>(path);
			GenerateTextureMetadata(CreateRef<TextureData>(), _path.replace_extension(".metadata"));
			_TextureCache.emplace(Ptr,texture);
			_Cache.emplace(path, Ptr);
			//*_id = Ptr;
			Ptr++;
			return texture;
		}
		
	}
	template<typename T>
	T AssetMananger::GetByHandle(uint32_t id)
	{
		static_assert(false);

	}
	template<>
	Ref<Texture> AssetMananger::GetByHandle<Ref<Texture>>(uint32_t id)
	{
		
		if (_TextureCache.find(id) != _TextureCache.end())
		{
			return _TextureCache[id];
		}
		else
		{
			return CreateRef<Texture>();
		}
	}

	bool AssetMananger::IsLoaded(uint32_t id)
	{
		return true;
	}

	void AssetMananger::GenerateTextureMetadata(const Ref<TextureData>& texturedata,const std::filesystem::path& path)
	{
		YAML::Emitter emitter;
		emitter << YAML::BeginMap;
		emitter << YAML::Key << "Mode" << YAML::Value << (int)texturedata->GetMode();
		emitter << YAML::Key << "CellSizeX" << YAML::Value << texturedata->GetCellSize().x;
		emitter << YAML::Key << "CellSizeY" << YAML::Value << texturedata->GetCellSize().y;
		emitter << YAML::Key << "Width" << YAML::Value << 0;
		emitter << YAML::Key << "Height" << YAML::Value << 0;

		emitter << YAML::EndMap;
		std::ofstream output(path.c_str());
		output << emitter.c_str();
	}

	

	Ref<TextureData> AssetMananger::LoadTextureMetadata(const std::string& path)
	{
		
			YAML::Node data;
			try
			{
				data = YAML::LoadFile(path);
			}
			catch (YAML::ParserException e)
			{
				CORE_LOG("failed to load metadata");
				return nullptr;
			}
			TextureMode mode = (TextureMode)data["Mode"].as<int>();
			int cellsizeX = data["CellSizeX"].as<int>();
			int cellsizeY = data["CellSizeY"].as<int>();
			int width = data["Width"].as<int>();
			int height = data["Height"].as<int>();
			Ref<TextureData> texturedata = CreateRef<TextureData>(mode, cellsizeX, cellsizeY, width, height);
			return texturedata;
		
	}

}