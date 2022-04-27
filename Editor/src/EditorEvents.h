#pragma once
#include "Event/Event.h"
#include "AssetManager/Asset.h"

namespace Lithium
{

	class EditorEvent : public Event
	{

	};

	class AssetBrowserFileOpenEvent : public EditorEvent
	{
	public:
		AssetBrowserFileOpenEvent() = default;
		const char* GetName() const override {

			return "AssetBrowserFileOpenEvent";
		}
		AssetBrowserFileOpenEvent(Asset asset,Asset::AssetType type)
		{
			this->asset = asset;
			assettype = type;
		}
		Asset asset;
		Asset::AssetType assettype;
		EVENT_TYPE(AssetBrowserFileOpen);
	};
	class OpenSpriteEditorEvent : public EditorEvent
	{
		std::string _Path;

	public:
		OpenSpriteEditorEvent() = default;
		OpenSpriteEditorEvent(const std::string& path)
		{
			_Path = path;
		}
		const char* GetName() const override {

			return "OpenSpriteEditorEvent";
		}
		std::string& GetPath()
		{
			return _Path;
		}		
		EVENT_TYPE(OpenSpriteEditor);

	};
}