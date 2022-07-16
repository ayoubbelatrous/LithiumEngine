#include "lipch.h"
#include "AssetManager.h"
#include "Audio/Audio.h"
#include "Core/UUID.h"
#include "Core/Log.h"
#include "AssetPacker.h"
#include "AssetPackReader.h"



namespace Lithium {
	template<>
	TextureMetaData AssetManager::GetAssetMetaData<TextureMetaData>(Asset asset)
	{
		std::filesystem::path Path = GetAssetPath(asset);
		Path += ".metadata";

		if (m_TextureMetaDataRegistry.find(asset.GetUUID()) != m_TextureMetaDataRegistry.end())
		{
			return m_TextureMetaDataRegistry[asset.GetUUID()];
		}

		if (std::filesystem::exists(Path))
		{

			YAML::Node data;
			try
			{
				data = YAML::LoadFile(Path.string());
			}
			catch (YAML::ParserException e)
			{
				LT_CORE_ERROR("failed to load MetaData");
				return TextureMetaData();
			}
			TextureMetaData::TextureMode mode;
			TextureMetaData::FilteringMode filteringmode;

			if (strcmp(data["TextureMode"].as<std::string>().c_str(), "Single") == 0)
			{
				mode = TextureMetaData::TextureMode::Single;
			}
			else
			{
				mode = TextureMetaData::TextureMode::Multiple;
			}
			int CellSizeX = data["CellSizeX"].as<int>();
			int CellSizeY = data["CellSizeY"].as<int>();
			auto Filteringmode = data["FilteringMode"].as<std::string>();
			if (strcmp(Filteringmode.c_str(), "Linear") == 0)
			{
				filteringmode = TextureMetaData::FilteringMode::Linear;

			}
			else if (strcmp(Filteringmode.c_str(), "Nearest") == 0)
			{
				filteringmode = TextureMetaData::FilteringMode::Nearest;
			}
			TextureMetaData textureMetaData(mode, TextureMetaData::TextureType::Sprite, filteringmode, CellSizeX, CellSizeY);
			m_TextureMetaDataRegistry[asset.GetUUID()] = textureMetaData;
			return textureMetaData;
		}
		else
		{
			YAML::Emitter emitter;
			emitter << YAML::BeginMap;
			emitter << YAML::Key << "TextureMode" << YAML::Value << std::string("Single");
			emitter << YAML::Key << "TextureType" << YAML::Value << std::string("Sprite");
			emitter << YAML::Key << "FilteringMode" << YAML::Value << std::string("Linear");
			emitter << YAML::Key << "CellSizeX" << YAML::Value << 1;
			emitter << YAML::Key << "CellSizeY" << YAML::Value << 1;
			emitter << YAML::EndMap;
			std::ofstream output(Path.string());
			output << emitter.c_str();
			TextureMetaData textureMetaData(TextureMetaData::TextureMode::Single, TextureMetaData::TextureType::Sprite, 1, 1);
			m_TextureMetaDataRegistry[asset.GetUUID()] = textureMetaData;
			return textureMetaData;
		}
	}
	template<typename T>
	bool AssetManager::UpdateAssetMetaData(T metadata, Asset asset)
	{
		ASSERT(false);
	}
	template<>
	bool AssetManager::UpdateAssetMetaData<TextureMetaData>(TextureMetaData metadata, Asset asset)
	{

		std::filesystem::path Path = GetAssetPath(asset);
		Path += ".metadata";

		if (m_TextureMetaDataRegistry.find(asset.GetUUID()) != m_TextureMetaDataRegistry.end())
		{
			m_TextureMetaDataRegistry.erase(asset.GetUUID());
			m_TextureRegistry.erase(asset.GetUUID());
		}

		std::string mode = "";
		std::string type = "Sprite";
		std::string filteringmode = "";

		if (metadata.Mode == TextureMetaData::TextureMode::Single)
		{
			mode = "Single";
		}
		else if ((metadata.Mode == TextureMetaData::TextureMode::Multiple))
		{
			mode = "Multiple";
		}

		if (metadata.filteringMode == TextureMetaData::FilteringMode::Linear)
		{
			filteringmode = "Linear";
		}
		else if (metadata.filteringMode == TextureMetaData::FilteringMode::Nearest)
		{
			filteringmode = "Nearest";
		}
		YAML::Emitter emitter;
		emitter << YAML::BeginMap;
		emitter << YAML::Key << "TextureMode" << YAML::Value << mode;
		emitter << YAML::Key << "TextureType" << YAML::Value << type;
		emitter << YAML::Key << "FilteringMode" << YAML::Value << filteringmode;
		emitter << YAML::Key << "CellSizeX" << YAML::Value << metadata.CellsizeX;
		emitter << YAML::Key << "CellSizeY" << YAML::Value << metadata.CellsizeY;
		emitter << YAML::EndMap;
		std::ofstream output(Path.string());
		output << emitter.c_str();
		return true;
	}
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
	Asset AssetManager::GetAssetFromPath<Ref<Animation>>(const std::string& path)
	{
		if (m_AssetRegistry.find(path) != m_AssetRegistry.end())
		{
			return Asset(UUID(m_AssetRegistry[path]));
		}
		else
		{
			UUID uuid = UUID();
			Ref<Animation> animation = CreateRef<Animation>();
			m_AssetRegistry.emplace(path, (uint64_t)uuid);
			m_PathRegistry.emplace((uint64_t)uuid, path);
			m_AnimationRegistry.emplace((uint64_t)uuid, animation);
			SaveAssetRegistry();
			return Asset(m_AssetRegistry[path]);
		}
	}

	template<>
	Ref<Texture> AssetManager::GetAsset(Asset asset)
	{

		uint64_t ID = (uint64_t)asset.GetUUID();
		if (ID == 0)
		{
			LT_CORE_INFO("[AssetManager].Texture Doesn't Exist");
			return nullptr;
		}
		if (m_TextureRegistry.find(ID) != m_TextureRegistry.end())
		{
			return m_TextureRegistry[ID];
		}
		else
		{

			std::filesystem::path path = m_PathRegistry[asset.GetUUID()];
			TextureMetaData metadata = GetAssetMetaData<TextureMetaData>(asset);
			Ref<Texture> texture;
			if (metadata.filteringMode == TextureMetaData::FilteringMode::Linear)
			{
				texture = CreateRef<Texture>(path.string(), 0);
			}
			else if (metadata.filteringMode == TextureMetaData::FilteringMode::Nearest)
			{
				texture = CreateRef<Texture>(path.string(), 1);
			}
			m_TextureRegistry.emplace(asset.GetUUID(), texture);
			SaveAssetRegistry();

			return texture;
		}
	}
	template<>
	Ref<AudioSource> AssetManager::GetAsset(Asset asset)
	{

		uint64_t ID = (uint64_t)asset.GetUUID();
		if (ID == 0)
		{
			LT_CORE_INFO("[AssetManager].AudioSource Doesn't Exist");
			return nullptr;
		}
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
		if (ID == 0)
		{
			LT_CORE_INFO("[AssetManager].Font Doesn't Exist");
			return nullptr;
		}
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

	template<>
	Ref<Animation> AssetManager::GetAsset(Asset asset)
	{

		uint64_t ID = (uint64_t)asset.GetUUID();
		if (ID == 0)
		{
			LT_CORE_INFO("[AssetManager] : Animation Doesn't Exist");
			return nullptr;
		}
		if (m_AnimationRegistry.find(ID) != m_AnimationRegistry.end())
		{
			return m_AnimationRegistry[ID];
		}
		else
		{
			std::string path = m_PathRegistry[asset.GetUUID()];
			Ref<Animation> animation = Animation::DeserializeAnimation(path);
			m_AnimationRegistry.emplace(asset.GetUUID(), animation);
			SaveAssetRegistry();
			return animation;
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

	uint32_t AssetManager::CreateUserTexture(int width, int height, int format)
	{
		Ref<Texture> texture;
		if (format == 0)
		{
			texture = CreateRef<Texture>(width, height, Texture::TextureType::RGB);

		}
		if (format == 1)
		{
			texture = CreateRef<Texture>(width, height, Texture::TextureType::RGBA);
		}
		uint32_t textureID = texture->GetID();
		m_UserTextures.emplace(textureID, texture);
		return textureID;
	}

	void AssetManager::DeleteUserTexture(uint32_t id)
	{
		m_UserTextures.erase(id);
	}

	void AssetManager::SetUserTextureData(uint32_t id, void* data)
	{
		m_UserTextures[id]->SetData(data);
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
			if (std::filesystem::exists(path))
			{
				emitter << YAML::BeginMap;
				emitter << YAML::Key << "UUID" << YAML::Value << uuid;
				emitter << YAML::Key << "Path" << YAML::Value << path;
				emitter << YAML::EndMap;
			}
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