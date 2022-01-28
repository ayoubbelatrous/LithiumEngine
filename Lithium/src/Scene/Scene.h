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
		template<typename Comp>
	void CopyComponent(Entity src, Entity dst)
	{
		if (src.HasComponent<Comp>())
			dst.AddOrReplaceComponent<Comp>(src.GetComponent<Comp>());
	}

	Entity DuplicateEntity(Entity src);
	private:
		entt::registry _Registry;
		EventCallback callback;

	};

	


}