#pragma once
#include "Asset.h"
#include "Renderer/Texture.h"
#include "Core/Base.h"
#include <unordered_map>
#include "Audio/AudioSource.h"
#include "AssetPackReader.h"
#include "Font/Font.h"
#include "AssetManager/Metadata.h"
#include "Animation/Animation.h"

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

		template<typename T>
		T GetAssetMetaData(Asset asset);

		template<typename T>
		bool UpdateAssetMetaData(T metadata,Asset asset);

		std::string GetAssetPath(Asset asset);
		void PackAssetRegistry(const std::string& path);
	private:
		void SaveAssetRegistry();
		void LoadAssetRegistry();
		std::unordered_map<std::string, uint64_t> m_AssetRegistry;
		std::unordered_map<uint64_t, std::string> m_PathRegistry;
		std::unordered_map<uint64_t, Ref<Texture>> m_TextureRegistry;
		std::unordered_map<uint64_t, TextureMetaData> m_TextureMetaDataRegistry;
		std::unordered_map<uint64_t, Ref<AudioSource>> m_AudioRegistry;
		std::unordered_map<uint64_t, Ref<Font>> m_FontRegistry;
		std::unordered_map<uint64_t, Ref<Animation>> m_AnimationRegistry;
		std::string m_AssetRegistryRootPath = "Library/";

	};

	

}