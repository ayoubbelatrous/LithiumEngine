#include "lipch.h"
#include "Components.h"
#include "Scene.h"
#include "Entity.h"
#include "Renderer/Renderer2D.h"
#include "Script/MonoServer.h"
#include "gtc/type_ptr.hpp"
#include "COre/Application.h"
#include "Core/Math.h"

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
		ent.AddComponent<RelationShipComponent>();
		CreateEntityEvent e = CreateEntityEvent();
		//callback(e);
		return ent;
	}

	Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name)
	{
		Entity ent(m_Registry.create(), this);
		ent.AddComponent<IDComponent>(uuid);
		ent.AddComponent<NameComponent>(name);
		ent.AddComponent<RelationShipComponent>();
		CreateEntityEvent e = CreateEntityEvent();
		//callback(e);
		return ent;
	}

	void Scene::DeleteEntity(Entity entity)
	{
		m_Registry.destroy(entity.GetHandle());
	}

	glm::mat4 getLocalModelMatrix(glm::vec3 position,glm::vec3 rotation,glm::vec3 scale)
	{
		const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f),
			glm::radians(rotation.x),
			glm::vec3(1.0f, 0.0f, 0.0f));
		const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f),
			glm::radians(rotation.y),
			glm::vec3(0.0f, 1.0f, 0.0f));
		const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f),
			glm::radians(rotation.z),
			glm::vec3(0.0f, 0.0f, 1.0f));

		// Y * X * Z
		const glm::mat4 roationMatrix = transformY * transformX * transformZ;

		// translation * rotation * scale (also know as TRS matrix)
		return glm::translate(glm::mat4(1.0f), position) *
			roationMatrix *
			glm::scale(glm::mat4(1.0f), scale);
	}
	void Scene::UpdateTransform(Entity entity)
	{
		RelationShipComponent& rc = entity.GetComponent<RelationShipComponent>();
		TransformComponent& tc = entity.GetComponent<TransformComponent>();

		for (auto& child : rc.Children)
		{
			Entity childEntity (GetUUIDMap()[child],this);
			TransformComponent& ctc = childEntity.GetComponent<TransformComponent>();

			glm::mat4 matrix = glm::mat4(0);
			matrix = tc.GetMatrix() * getLocalModelMatrix(ctc.Position,ctc.Rotation,ctc.Scale);

			glm::vec3 pos = glm::vec3(0);
			glm::vec3 rot;
			glm::vec3 scale;
			if (Math::DecomposeTransform(matrix, pos, rot, scale))
			{
			ctc.Position = pos;
			}

			UpdateTransform(childEntity);
		}
	}
	void Scene::onEditorUpdate()
	{
		{
			auto view = GetRegistry().view<RelationShipComponent>();
			
			for (auto e : view)
			{
				Entity entity(e, this);
				if (entity.GetComponent<RelationShipComponent>().Parent == 0)
				{
					//UpdateTransform(entity);
				}
			}
		}	
		
		{
			auto view = GetRegistry().view<TransformComponent, SpriteRendererComponent>();

			for (auto entity : view)
			{
				auto& [tc, sp] = view.get<TransformComponent, SpriteRendererComponent>(entity);
				BatchRenderer::DrawQuad(tc.GetMatrix(), sp.GetColor(), (uint32_t)entity);
			}
		}

		
		
		{
			auto view = GetRegistry().view<ScriptGroupeComponent>();
			for (auto entity : view)
			{
				Entity ent = { entity,this };
				ScriptGroupeComponent& ScriptGroupe = ent.GetComponent<ScriptGroupeComponent>();

				for (auto& script : ScriptGroupe.Scripts) {
					if (!script.Loaded)
					{

						script.Scriptobject = Application::Get().Monoserver->GetObject(script.Name);

						script.Loaded = true;

					}
				}
				

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
				BatchRenderer::DrawQuad(tc.GetMatrix() , sp.GetColor(), (uint32_t)entity);
			}
		}


		{
			auto view = GetRegistry().view<ScriptGroupeComponent>();

			for (auto entity : view)
			{
				Entity ent = { entity,this };

				for (auto& script:ent.GetComponent<ScriptGroupeComponent>().Scripts )
				{

					if (!script.Loaded)
					{

						script.Scriptobject = Application::Get().Monoserver->GetObject(script.Name);
						script.Loaded = true;


					}
					else
					{
						if (!script.Created)
						{
							IDComponent& idcomp = ent.GetComponent<IDComponent>();
							void* Args[1];
							Args[0] = &idcomp.ID;
							script.Scriptobject->SetProperty("ID", Args);
							script.Scriptobject->InvokeMethod("Start", nullptr);
							script.Created = true;
						}
					}


					script.Scriptobject->InvokeMethod("Update", nullptr);


				}
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
		CopyComponentAll<RelationShipComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponentAll<MaterialComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponentAll<SpriteRendererComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponentAll<RigidBody2DComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponentAll<BoxCollider2DComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponentAll<ScriptGroupeComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		return newscene;
	}

	Entity Scene::DuplicateEntity(Entity src)
	{
		Entity entity = CreateEntity("new Entity");
		if (src.HasComponent<TransformComponent>())
		{
			CopyComponent<TransformComponent>(src, entity);
		}

		if (src.HasComponent<RelationShipComponent>())
		{
			CopyComponent<RelationShipComponent>(src, entity);

		}

		if (src.HasComponent<SpriteRendererComponent>())
		{
			CopyComponent<SpriteRendererComponent>(src, entity);

		}
		if (src.HasComponent<NameComponent>())
		{
			CopyComponent<NameComponent>(src, entity);

		}

		if (src.HasComponent<ScriptComponent>())
		{
			CopyComponent<ScriptComponent>(src, entity);

		}

		

		return entity;
	}

}
