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
		OpenSpriteEditorEvent(Asset asset)
		{
			this->asset = asset;
		}
		const char* GetName() const override {

			return "OpenSpriteEditorEvent";
		}
		Asset asset;
		EVENT_TYPE(OpenSpriteEditor);

	};

	class EditAnimationEvent : public EditorEvent
	{
	public:
		EditAnimationEvent() = default;
		EditAnimationEvent(Asset asset)
		{
			this->asset = asset;
		}
		const char* GetName() const override {

			return "EditAnimationEvent";
		}
		Asset asset;
		EVENT_TYPE(EditAnimation);

	};
}