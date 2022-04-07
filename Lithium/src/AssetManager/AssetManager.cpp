#include "lipch.h"
#include "AssetManager.h"
#include "Audio/Audio.h"
#include "Core/UUID.h"
#include "AssetPacker.h"
#include "AssetPackReader.h"


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
	Asset AssetManager::GetAssetFromPath<Ref<AudioSource>>(const std::string& path)
	{
		if (m_AssetRegistry.find(path) != m_AssetRegistry.end())
		{
			return Asset(UUID(m_AssetRegistry[path]));
		}
		else
		{
			UUID uuid = UUID();
			Ref<AudioSource> audioSource = Audio::LoadFromFile(path);
			m_AssetRegistry.emplace(path, (uint64_t)uuid);
			m_PathRegistry.emplace((uint64_t)uuid, path);
			m_AudioRegistry.emplace((uint64_t)uuid, audioSource);
			SaveAssetRegistry();
			return Asset(m_AssetRegistry[path]);
		}
	}
	template<>
	Asset AssetManager::GetAssetFromPath<Ref<Font>>(const std::string& path)
	{
		if (m_AssetRegistry.find(path) != m_AssetRegistry.end())
		{
			return Asset(UUID(m_AssetRegistry[path]));
		}
		else
		{
			UUID uuid = UUID();
			Ref<Font> font = CreateRef<Font>(path);
			m_AssetRegistry.emplace(path, (uint64_t)uuid);
			m_PathRegistry.emplace((uint64_t)uuid, path);
			m_FontRegistry.emplace((uint64_t)uuid, font);
			SaveAssetRegistry();
			return Asset(m_AssetRegistry[path]);
		}
	}
	template<>
	Ref<Texture> AssetManager::GetAsset(Asset asset)
	{
		LT_PROFILE_FUNCTION("Bla Bla")
		uint64_t ID = (uint64_t)asset.GetUUID();

		if (m_TextureRegistry.find(ID) != m_TextureRegistry.end())
		{
			return m_TextureRegistry[ID];
		}
		else
		{
			
			std::string path = m_PathRegistry[asset.GetUUID()];
			
			Ref<Texture> texture = CreateRef<Texture>(path);
			m_TextureRegistry.emplace(asset.GetUUID(), texture);
			SaveAssetRegistry();

			return texture;
		}
	}
	template<>
	Ref<AudioSource> AssetManager::GetAsset(Asset asset)
	{

		uint64_t ID = (uint64_t)asset.GetUUID();

		if (m_AudioRegistry.find(ID) != m_AudioRegistry.end())
		{
			return m_AudioRegistry[ID];
		}
		else
		{
			std::string path = m_PathRegistry[asset.GetUUID()];
			Ref<AudioSource> audioSource = Audio::LoadFromFile(path);
			m_AudioRegistry.emplace(asset.GetUUID(), audioSource);
			SaveAssetRegistry();

			return audioSource;
		}
	}

	template<>
	Ref<Font> AssetManager::GetAsset(Asset asset)
	{

		uint64_t ID = (uint64_t)asset.GetUUID();

		if (m_FontRegistry.find(ID) != m_FontRegistry.end())
		{
			return m_FontRegistry[ID];
		}
		else
		{
			std::string path = m_PathRegistry[asset.GetUUID()];
			Ref<Font> font = CreateRef<Font>(path);
			m_FontRegistry.emplace(asset.GetUUID(), font);
			SaveAssetRegistry();

			return font;
		}
	}


	std::string AssetManager::GetAssetPath(Asset asset)
	{
		if (m_PathRegistry.find(asset.GetUUID()) != m_PathRegistry.end())
		{
			return m_PathRegistry[asset.GetUUID()];
		}
		else
		{
			return std::string();
		}
	}

	void AssetManager::PackAssetRegistry(const std::string& path)
	{
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