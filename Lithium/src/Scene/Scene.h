#pragma once
#include "entt.hpp"
#include <string>
#include "Core/Base.h"
#include "Event/Event.h"
#include "Event/AppEvents.h"
#include <functional>
#include "Core/UUID.h"
#include "physics/physics.h"


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
		void UpdateTransform(Entity entity);
		void onEditorUpdate();
		void OnStart();
		void onUpdate();
		void OnStop();
		void SetEventCallback(const EventCallback& e) { m_Callback = e; }
		std::unordered_map<UUID, entt::entity> GetUUIDMap();
		
		template<typename Component>
	    static void CopyComponentAll(entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap);
	    
		template<typename T>
		void CopyComponent(Entity src, Entity dst);
		
		
		static Ref<Scene> Copy(const Ref<Scene>& src);
		
	    Entity DuplicateEntity(Entity src);
	
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
		Entity GetPrimaryCameraEntity();
		void SortScene();
		void OnViewportResize(uint32_t width, uint32_t height);
		void SetRenderEditorUi(bool render);
		bool GetRenderEditorUi();
		
	private:
		bool m_RenderEditorUI = false;
		uint32_t ViewportWidth = 1.0f;
		uint32_t ViewportHeight = 1.0f;
		Scope<PhysicsWorld> m_PhysicsWorld;
		entt::registry m_Registry;
		EventCallback m_Callback;
	};

	


}