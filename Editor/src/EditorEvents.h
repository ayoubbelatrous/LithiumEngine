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
	
}