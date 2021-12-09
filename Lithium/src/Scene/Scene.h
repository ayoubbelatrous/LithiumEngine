#pragma once
#include "entt.hpp"
#include <string>
#include "Core/Base.h"
#include "Event/Event.h"
#include "Event/AppEvents.h"
#include <functional>

namespace Lithium
{
	class Entity;

	class Scene
	{
	public:
		using EventCallback = std::function<void(Event&)>;
		Scene();

		Entity CreateEntity(const std::string& name);
		void DeleteEntity(Entity entity);
		
		entt::registry& GetRegistry() { return _Registry; }

		void onEditorUpdate();
		void SetEventCallback(const EventCallback& e) { callback = e; }
	private:
		entt::registry _Registry;
		EventCallback callback;

	};

}