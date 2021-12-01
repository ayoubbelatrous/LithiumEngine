#pragma once
#include "Event.h"

namespace Lithium
{

	class WindowCloseEvent : public Event
	{
	public:
		
		WindowCloseEvent() = default;

		const char* GetName() const override {

			return "WindowCloseEvent";
		}

		EVENT_TYPE(WindowClose);
		
	};
}
