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

		ImGui::End();
	}

	void AssetPropertiesPanel::SetAsset(Asset asset, Asset::AssetType assettype)
	{
		m_CurrentAsset = asset;
		m_AssetType = assettype;
	}

}


