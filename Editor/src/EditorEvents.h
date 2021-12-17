#pragma once
#include "Event/Event.h"
namespace Lithium
{

	class EditorEvent : public Event
	{

	};

	class AssetBrowserEvent : public EditorEvent
	{
	public:
		AssetBrowserEvent() = default;
		const char* GetName() const override {

			return "AssetBrowserEvent";
		}

		EVENT_TYPE(AssetBrowser);
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

	class MetaDataChangeEventEditorEvent : public EditorEvent
	{
		std::string _Path;

	public:
		MetaDataChangeEventEditorEvent() = default;
		MetaDataChangeEventEditorEvent(const std::string& path)
		{
			_Path = path;
		}
		const char* GetName() const override {

			return "MetaDataChangeEventEditorEvent";
		}
		std::string& GetPath()
		{
			return _Path;
		}
		EVENT_TYPE(OpenSpriteEditor);

	};
}