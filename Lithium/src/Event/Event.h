#pragma once


#include "Core/Base.h"
#include <functional>



namespace Lithium
{

	enum class EventType
	{
		WindowResize, WindowClose, WindowMove, WindowFocus,
		KeyPress,

	};

#define EVENT_TYPE(t) static EventType GetType() { return EventType::t; }\
								virtual EventType GetEventType() const override { return GetType(); }
	class Event
	{
		
	public:

		virtual ~Event() = default;

		virtual const char* GetName() const = 0;
		virtual EventType GetEventType() const = 0;
	};
}



