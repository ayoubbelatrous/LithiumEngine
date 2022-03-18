#include "lipch.h"
#include "AssetManager.h"
#include "Core/UUID.h"

namespace Lithium {

	Asset AssetManager::GetAssetFromPath(const std::string& path)
	{
		if (m_AssetRegistry.find(path) != m_AssetRegistry.end())
		{
			return Asset(UUID(m_AssetRegistry[path]));
		}
		else
		{
			UUID uuid = UUID();
			Ref<Texture> texture = CreateRef<Texture>(path);
			m_AssetRegistry.emplace(path, (uint64_t)uuid);
			m_TextureRegistry.emplace((uint64_t)uuid, texture);
			return Asset(m_AssetRegistry[path]);
		}
	}
	template<>
	Ref<Texture> AssetManager::GetAsset(Asset asset)
	{
		LT_PROFILE_FUNCTION("AssetManager::GetAsset()");
		uint64_t ID = (uint64_t)asset.GetUUID();

		if (m_TextureRegistry.find(ID) != m_TextureRegistry.end())
		{
			return m_TextureRegistry[ID];
		}
		else
		{
			return nullptr;
		}
	}


}