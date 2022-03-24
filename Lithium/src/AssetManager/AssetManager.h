#pragma once
#include "Asset.h"
#include "Renderer/Texture.h"
#include "Core/Base.h"
#include <unordered_map>
#include "Audio/AudioSource.h"
namespace Lithium
{
	class AssetManager
	{
	public:
		AssetManager()
		{
			LoadAssetRegistry();
		};
		
		template<typename T>
		Asset GetAssetFromPath(const std::string& path);
		
		template<typename T>
		T GetAsset(Asset asset);

	private:

		void SaveAssetRegistry();
		void LoadAssetRegistry();
		std::unordered_map<std::string, uint64_t> m_AssetRegistry;
		std::unordered_map<uint64_t, std::string> m_PathRegistry;
		std::unordered_map<uint64_t, Ref<Texture>> m_TextureRegistry;
		std::unordered_map<uint64_t, Ref<AudioSource>> m_AudioRegistry;
		std::string m_AssetRegistryRootPath = "Cache/";
	};

}