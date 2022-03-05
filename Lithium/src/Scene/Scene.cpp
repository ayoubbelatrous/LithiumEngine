#include "lipch.h"
#include "Components.h"
#include "Scene.h"
#include "Entity.h"
#include "Renderer/Renderer2D.h"
#include "Script/MonoServer.h"
#include "gtc/type_ptr.hpp"
#include "COre/Application.h"


namespace Lithium
{

	Scene::Scene()
		:m_Registry(entt::registry())
	{
	}
	

	template<typename Component>
	static void CopyComponentAll(entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		auto view = src.view<Component>();
		for (auto e : view)
		{
			UUID uuid = src.get<IDComponent>(e).ID;
			
			entt::entity dstEnttID = enttMap.at(uuid);

			auto component = src.get<Component>(e);
			dst.emplace_or_replace<Component>(dstEnttID, component);
		}
	}
	
	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity ent(m_Registry.create(), this);
		ent.AddComponent<IDComponent>(UUID());
		ent.AddComponent<NameComponent>(name);
		CreateEntityEvent e = CreateEntityEvent();
		//callback(e);
		return ent;
	}

	Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name)
	{
		Entity ent(m_Registry.create(), this);
		ent.AddComponent<IDComponent>(uuid);
		ent.AddComponent<NameComponent>(name);
		CreateEntityEvent e = CreateEntityEvent();
		//callback(e);
		return ent;
	}

	void Scene::DeleteEntity(Entity entity)
	{
		m_Registry.destroy(entity.GetHandle());
	}

	void Scene::onEditorUpdate()
	{
		{
			auto view = GetRegistry().view<TransformComponent, SpriteRendererComponent>();

			for (auto entity : view)
			{
				auto& [tc, sp] = view.get<TransformComponent, SpriteRendererComponent>(entity);
				BatchRenderer::DrawQuad(tc.GetMatrix(), sp.GetColor(), (uint32_t)entity);
			}
		}

	}

	void Scene::OnStart()
	{
		

		auto view = GetRegistry().view<RigidBody2DComponent>();


	}

	void Scene::onUpdate()
	{
		
		{
			auto view = GetRegistry().view<TransformComponent, SpriteRendererComponent>();

			for (auto entity : view)
			{

				auto& [tc, sp] = view.get<TransformComponent, SpriteRendererComponent>(entity);
				BatchRenderer::DrawQuad(tc.GetMatrix(), sp.GetColor(), (uint32_t)entity);
			}
		}
	}

	void Scene::OnStop()
	{

	}

	std::unordered_map<UUID, entt::entity> Scene::GetUUIDMap()
	{
		std::unordered_map<UUID, entt::entity> enttMap;

		auto entities = m_Registry.view<IDComponent>();

		for (auto entity : entities)
		{

			IDComponent idc = m_Registry.get<IDComponent>(entity);
			enttMap.emplace(idc.ID, entity);
		}
		return enttMap;
	}

	Ref<Scene> Scene::Copy(const Ref<Scene>& src)
	{
		std::unordered_map<UUID, entt::entity> enttMap;

		Ref<Scene> newscene = CreateRef<Scene>();
		auto view = src->GetRegistry().view<NameComponent>();
		entt::registry& srcSceneRegistry = src->GetRegistry();

		for (auto e : view)
		{
			UUID uuid = srcSceneRegistry.get<IDComponent>(e).ID;
			const auto& name = srcSceneRegistry.get<NameComponent>(e).Name;
			Entity newEntity = newscene->CreateEntityWithUUID(uuid, name);
			enttMap[uuid] = newEntity.GetHandle();
		}
		entt::registry& dstSceneRegistry = newscene->GetRegistry();
		CopyComponentAll<TransformComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponentAll<MaterialComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponentAll<SpriteRendererComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponentAll<CameraComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponentAll<RigidBody2DComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponentAll<BoxCollider2DComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		return newscene;
	}

	void Scene::Test()
	{
		
		
	}

	Entity Scene::DuplicateEntity(Entity src)
	{
		Entity entity = CreateEntity("new Entity");
		if (src.HasComponent<TransformComponent>())
		{
			CopyComponent<TransformComponent>(src, entity);
		}

		if (src.HasComponent<SpriteRendererComponent>())
		{
			CopyComponent<SpriteRendererComponent>(src, entity);

		}
		if (src.HasComponent<NameComponent>())
		{
			CopyComponent<NameComponent>(src, entity);

		}



		return entity;
	}

}
