#pragma once
#include "AssetManager/Asset.h"

namespace Lithium
{
	class AssetPropertiesPanel
	{

	public:
		void OnCreate();
		void OnUpdate(bool* open);
		void SetAsset(Asset asset, Asset::AssetType assettype);
	private:
		Asset m_CurrentAsset;
		Asset::AssetType m_AssetType;
	};
}