#pragma once
#include "Event/Event.h"
#include "Event/AppEvents.h"

namespace Lithium
{
	class Layer
	{
	public:
		Layer(const std::string& name);

		virtual void OnCreate() {}
		virtual void OnUpdate() {}
		virtual void OnDestroy() {}
		virtual void onEvent(Event& event) { };

		std::string& GetName() { return _name; }
		std::string _name = "TEST";
	};
}