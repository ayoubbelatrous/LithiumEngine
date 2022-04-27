#include "epch.h"
#include "Panels/AssetPropertiesPanel.h"
#include "imgui/imgui.h"

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
		
	}

}


