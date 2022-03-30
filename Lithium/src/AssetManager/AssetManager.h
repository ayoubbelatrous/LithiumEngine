#pragma once
#include "Asset.h"
#include "Renderer/Texture.h"
#include "Core/Base.h"
#include <unordered_map>
#include "Audio/AudioSource.h"
#include "AssetPackReader.h"
namespace Lithium
{
	class AssetManager
	{
	public:
		AssetManager()
		{
			assetReader = CreateRef<AssetPackReader>("assets/build/shared.assets");
			LoadAssetRegistry();
		};
		
		template<typename T>
		Asset GetAssetFromPath(const std::string& path);
		
		template<typename T>
		T GetAsset(Asset asset);

		std::string GetAssetPath(Asset asset);
		void PackAssetRegistry(const std::string& path);
	private:

		void SaveAssetRegistry();
		void LoadAssetRegistry();
		std::unordered_map<std::string, uint64_t> m_AssetRegistry;
		std::unordered_map<uint64_t, std::string> m_PathRegistry;
		std::unordered_map<uint64_t, Ref<Texture>> m_TextureRegistry;
		std::unordered_map<uint64_t, Ref<AudioSource>> m_AudioRegistry;
		std::string m_AssetRegistryRootPath = "Cache/";
		Ref<AssetPackReader> assetReader;

	};

}