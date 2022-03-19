#include "lipch.h"
#include "AssetManager.h"
#include "Core/UUID.h"

namespace Lithium {

	template<>
	Asset AssetManager::GetAssetFromPath<Ref<Texture>>(const std::string& path)
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
			m_PathRegistry.emplace((uint64_t)uuid, path);
			m_TextureRegistry.emplace((uint64_t)uuid, texture);
			SaveAssetRegistry();
			return Asset(m_AssetRegistry[path]);
		}
	}
	template<>
	Ref<Texture> AssetManager::GetAsset(Asset asset)
	{
		
		uint64_t ID = (uint64_t)asset.GetUUID();

		if (m_TextureRegistry.find(ID) != m_TextureRegistry.end())
		{
			SaveAssetRegistry();
			return m_TextureRegistry[ID];
		}
		else
		{
			
			std::string path = m_PathRegistry[asset.GetUUID()];
			CORE_LOG("texture path");
			Ref<Texture> texture = CreateRef<Texture>(path);
			m_TextureRegistry.emplace(asset.GetUUID(), texture);
			SaveAssetRegistry();
			return texture;
		}
	}


	void AssetManager::SaveAssetRegistry()
	{
		YAML::Emitter emitter;
			
		emitter << YAML::BeginMap;
		emitter << YAML::Key << "PathRegistry" << YAML::Value << YAML::BeginSeq;
		for (auto& asset : m_PathRegistry)
		{
			uint64_t uuid = asset.first;
			std::string path = asset.second;
			emitter << YAML::BeginMap;
			emitter << YAML::Key << "UUID" << YAML::Value << uuid;
			emitter << YAML::Key << "Path" << YAML::Value << path;
			emitter << YAML::EndMap;
		}
		emitter << YAML::EndSeq;
		emitter << YAML::EndMap;
		std::ofstream output(m_AssetRegistryRootPath + "Registry.asset");
		output << emitter.c_str();
	}

	void AssetManager::LoadAssetRegistry()
	{
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(m_AssetRegistryRootPath + "Registry.asset");
		}
		catch (YAML::ParserException e)
		{
			CORE_LOG("failed to load AssetRegistry");
			return;
		}


		auto Registry = data["PathRegistry"];
		for (auto asset : Registry)
		{
			uint64_t uuid = asset["UUID"].as<uint64_t>();
			std::string path = asset["Path"].as<std::string>();
			m_AssetRegistry.emplace(path, uuid);
			m_PathRegistry.emplace(uuid, path);
		}
	}

}