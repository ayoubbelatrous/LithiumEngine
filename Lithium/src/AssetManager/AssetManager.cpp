#include "lipch.h"

#include "AssetManager.h"


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
			CORE_LOG("Asset Already loaded"<< path);
			uint32_t id =  _Cache[path];
			//*_id = id;
			Ref<Texture> asset = _TextureCache[id];
			return asset;
		}
		else
		{
			CORE_LOG("loaded asset" << path);
			Ref<Texture>texture =  CreateRef<Texture>(path);
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

}