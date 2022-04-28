#pragma once
#include "AssetManager/Asset.h"
#include "../EditorEvents.h"
namespace Lithium
{
	using EventCallback = std::function<void(Event&)>;
	class AssetPropertiesPanel
	{
	public:
		void OnCreate();
		void OnUpdate(bool* open);
		void SetAsset(Asset asset, Asset::AssetType assettype);
		void SetEventCallback(const EventCallback& callbackfn)
		{
			callback = callbackfn;
		}
	private:
		void DrawTexture();
		Asset m_CurrentAsset;
		EventCallback callback;

		Asset::AssetType m_AssetType;
	};
}