#pragma once
#include "Event.h"
#include "KeyCodes.h"

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
	class WindowFileDropEvent : public Event
	{
	public:

		WindowFileDropEvent() = default;
		WindowFileDropEvent(int count, const char** path) : Pathcount(count), paths(path)
		{}
		const char* GetName() const override {

			return "WindowFileDropEvent";
		}
		const char** paths;
		int Pathcount;
		const char** getPaths()
		{
			return paths;
		}
		EVENT_TYPE(WindowFileDrop);
	};


	class KeyEvent : public Event
	{
	public:

		KeyEvent() = default;
		KeyEvent(int keycode, int action, int mods,int scancode) : keycode(keycode), action(action),mods(mods),scancode(scancode)
		{}
		const char* GetName() const override {

			return "KeyEvent";
		}
		int GetKeyCode() { return keycode; };
		int GetKeyMods() { return mods; };
		int GetKeyScanCode() { return scancode; };
		int GetKeyAction() { return action; };
		int keycode;
		int action;
		int mods;
		int scancode;
		EVENT_TYPE(KeyPress);
	};


	class MouseWheelEvent : public Event
	{
	public:

		MouseWheelEvent() = default;
		MouseWheelEvent(float x,float y)
		:xOffset(x),yOffset(y){}
		const char* GetName() const override {

			return "MouseWheelEvent";
		}
		float GetOffsetX()
		{
			return xOffset;
		}
		float GetOffsetY()
		{
			return yOffset;
		}
	private:
		float xOffset;
		float yOffset;
		EVENT_TYPE(MouseWheel);

	};


	class CreateEntityEvent : public Event
	{
	public:

		CreateEntityEvent() = default;
		const char* GetName() const override {

			return "CreateEntityEvent";
		}

		EVENT_TYPE(CreateEntity);
		
	};


	

}
