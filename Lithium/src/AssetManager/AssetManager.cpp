#include "lipch.h"
#include "AssetManager.h"
#include "Core/Base.h"
#include "Core/Math.h"

namespace Lithium {


	template<>
	Asset<Ref<Mesh>> AssetManager::LoadAsset(const std::string& path)
	{
		Ref<Mesh> mesh = CreateRef<Mesh>();
		Asset<Ref<Mesh>> m(path);
		m.SetAsset(mesh);

		return m;
	}

	template<>
	Asset<Ref<Material>> AssetManager::LoadAsset(const std::string& path)
	{
		Ref<Material> material = Material::MaterialFromFile(path);
		Asset<Ref<Material>> m(path);
		m.SetAsset(material);

		return m;
	}

	template<>
	Asset<Ref<Texture>> AssetManager::LoadAsset(const std::string& path)
	{
		Asset<Ref<Texture>> asset(path);

		if (_PathTable.find(path) != _PathTable.end())
		{
			uint32_t id = _PathTable[path];
			Ref<Texture> texture = _TextureCache[id];
			asset.SetAsset(texture);
			uint32_t* refrence = &_TextureRefrenceCount[id];;
			asset.ref = refrence;
			*refrence += 1;
			asset.SetDeleteCallBack([this]() {
				this->CheckForRefrences();
				});
			return asset;
		}
		else
		{
			
			float id = Math::Random::Float();
			id *= 10000000;


			_PathTable.emplace(path, id);
			_TextureRefrenceCount.emplace(id, 0);


			Ref<Texture> texture = CreateRef<Texture>(path);


			_TextureCache.emplace(id, texture);


			asset.SetAsset(_TextureCache[id]);
			
		
			uint32_t* refrence = &_TextureRefrenceCount[id];;
			asset.ref = refrence;
			*refrence+=1;
		

			asset.SetDeleteCallBack([this]() {
				this->CheckForRefrences();
			});
			return asset;
		}
	}

	void AssetManager::CheckForRefrences()
	{
		//TODO: delete cache when refrence count hits 0
	}

}