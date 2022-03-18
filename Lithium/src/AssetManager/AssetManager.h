#pragma once
#include "Asset.h"
#include "Renderer/Texture.h"
#include "Core/Base.h"
#include <unordered_map>

namespace Lithium
{
	class AssetManager
	{
	public:
		AssetManager() = default;

		
		Asset GetAssetFromPath(const std::string& path);
		
		template<typename T>
		T GetAsset(Asset asset);
	private:
		std::unordered_map<std::string, uint64_t> m_AssetRegistry;
		std::unordered_map<uint64_t, Ref<Texture>> m_TextureRegistry;
	};

}