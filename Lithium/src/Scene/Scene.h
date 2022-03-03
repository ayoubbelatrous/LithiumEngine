#pragma once
#include "entt.hpp"
#include <string>
#include "Core/Base.h"
#include "Event/Event.h"
#include "Event/AppEvents.h"
#include <functional>
#include "Core/UUID.h"

namespace Lithium
{
	class Entity;

	class Scene
	{
	public:
		using EventCallback = std::function<void(Event&)>;
		Scene();

		Entity CreateEntity(const std::string& name);
		Entity CreateEntityWithUUID(UUID uuid,const std::string& name);
		void DeleteEntity(Entity entity);
		
		entt::registry& GetRegistry() { return m_Registry; }

		void onEditorUpdate();
		void onUpdate();
		void SetEventCallback(const EventCallback& e) { m_Callback = e; }
		template<typename Comp>
	    void CopyComponent(Entity src, Entity dst)
	    {
		if (src.HasComponent<Comp>())
			dst.AddOrReplaceComponent<Comp>(src.GetComponent<Comp>());
	    }
		static Ref<Scene> Copy(const Ref<Scene>& src);
	Entity DuplicateEntity(Entity src);
	private:
		entt::registry m_Registry;
		EventCallback m_Callback;

	};

	


}