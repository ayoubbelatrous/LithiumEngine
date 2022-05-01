#include "epch.h"
#include "Panels/AssetPropertiesPanel.h"
#include "Core/Application.h"
#include "imgui/imgui.h"
#include "EditorEvents.h"

namespace Lithium
{

	void AssetPropertiesPanel::OnCreate()
	{

	}

	void AssetPropertiesPanel::OnUpdate(bool* open)
	{
		ImGui::Begin("Asset Properties",open);
		if (m_CurrentAsset.GetUUID() != 0)
		{
			switch (m_AssetType)
			{	
			case Lithium::Asset::AssetType::None:
				break;
			case Lithium::Asset::AssetType::Texture:
				DrawTexture();
				break;
			case Lithium::Asset::AssetType::Font:
				break;
			case Lithium::Asset::AssetType::Animation:
				break;
			case Lithium::Asset::AssetType::AudioClip:
				break;
			default:
				break;
			}
		}
		ImGui::End();
	}

	void AssetPropertiesPanel::SetAsset(Asset asset, Asset::AssetType assettype)
	{
		m_CurrentAsset = asset;
		m_AssetType = assettype;
	}

	void AssetPropertiesPanel::DrawTexture()
	{
		ImGui::Text("AsseType: Texture");
		
		const char* texturemodeStrings[] = { "Single", "Multiple" };
		TextureMetaData textureMetaData = Application::Get().assetManager->GetAssetMetaData<TextureMetaData>(m_CurrentAsset);
		TextureMetaData newTextureMetaData = textureMetaData;
		const char* currenttexturemodeString = texturemodeStrings[(int)textureMetaData.Mode];
		if (ImGui::Button("Sprite Editor"))
		{
			OpenSpriteEditorEvent ev = OpenSpriteEditorEvent(m_CurrentAsset);
			callback(ev);
		}
		if (ImGui::BeginCombo("Texture Mode", currenttexturemodeString))
		{
			for (int i = 0; i < 2; i++)
			{
				bool isSelected = currenttexturemodeString == texturemodeStrings[i];
				if (ImGui::Selectable(texturemodeStrings[i], isSelected))
				{
					currenttexturemodeString = texturemodeStrings[i];
					newTextureMetaData.Mode = (TextureMetaData::TextureMode)i;
					Application::Get().assetManager->UpdateAssetMetaData<TextureMetaData>(newTextureMetaData, m_CurrentAsset);
				}

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}

		const char* filteringtypeStrings[] = { "Linear", "Nearest" };
		const char* currentfilteringtypeString = filteringtypeStrings[(int)textureMetaData.filteringMode];

		if (ImGui::BeginCombo("Filtering Mode", currentfilteringtypeString))
		{
			for (int i = 0; i < 2; i++)
			{
				bool isSelected = currentfilteringtypeString == filteringtypeStrings[i];
				if (ImGui::Selectable(filteringtypeStrings[i], isSelected))
				{
					currentfilteringtypeString = filteringtypeStrings[i];
					newTextureMetaData.filteringMode = (TextureMetaData::FilteringMode)i;
					Application::Get().assetManager->UpdateAssetMetaData<TextureMetaData>(newTextureMetaData, m_CurrentAsset);
				}

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}
	}

}


