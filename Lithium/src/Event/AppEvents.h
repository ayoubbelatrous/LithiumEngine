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

	class WindowResizeEvent : public Event
	{
	public:

		WindowResizeEvent() = default;
		WindowResizeEvent(int w,int h) : width(w),height(h)
		{}
		const char* GetName() const override {

			return "WindowResizeEvent";
		}
		int GetWidth() { return width; };
		int GetHeight() { return height; };
		int width = 0;
		int height = 0;
		EVENT_TYPE(WindowResize);
	};
}
