#include "lipch.h"
#include "Components.h"
#include "Scene.h"
#include "Entity.h"
#include "Renderer/Renderer2D.h"
#include "Script/MonoServer.h"
#include "gtc/type_ptr.hpp"
#include "Core/Application.h"
#include "Core/Math.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "gtx/string_cast.hpp"
#include "box2d/box2d.h"
#include "AssetManager/AssetManager.h"
#include "Audio/Audio.h"
#include "Font/FontRenderer.h"

namespace Lithium
{

	class ContactListener : public b2ContactListener
	{
	public:
		void BeginContact(b2Contact* contact)
		{
			UUID* IDA = reinterpret_cast<UUID*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
			Entity entityA(Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[*IDA], Application::Get().sceneManager->GetActiveScene().get());
			if (entityA.HasComponent<ScriptGroupeComponent>())
			{
				ScriptGroupeComponent& scriptgroupeA = entityA.GetComponent<ScriptGroupeComponent>();

				for (auto& script : scriptgroupeA.Scripts)
				{
					script.Scriptobject->InvokeMethod("OnCollisionEnter", nullptr);
				}
			}
			
			UUID* IDB = reinterpret_cast<UUID*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
			Entity entityB (Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[*IDB], Application::Get().sceneManager->GetActiveScene().get());

			if (entityA.HasComponent<ScriptGroupeComponent>())
			{
				ScriptGroupeComponent& scriptgroupeB = entityB.GetComponent<ScriptGroupeComponent>();

				for (auto& script : scriptgroupeB.Scripts)
				{
					script.Scriptobject->InvokeMethod("OnCollisionEnter", nullptr);
				}
			}
		}
		void EndContact(b2Contact* contact)
		{

			UUID* IDA = reinterpret_cast<UUID*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
			Entity entityA(Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[*IDA], Application::Get().sceneManager->GetActiveScene().get());
			if (entityA.HasComponent<ScriptGroupeComponent>())
			{
				ScriptGroupeComponent& scriptgroupeA = entityA.GetComponent<ScriptGroupeComponent>();

				for (auto& script : scriptgroupeA.Scripts)
				{
					script.Scriptobject->InvokeMethod("OnCollisionExit", nullptr);
				}
			}

			UUID* IDB = reinterpret_cast<UUID*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
			Entity entityB(Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[*IDB], Application::Get().sceneManager->GetActiveScene().get());

			if (entityA.HasComponent<ScriptGroupeComponent>())
			{
				ScriptGroupeComponent& scriptgroupeB = entityB.GetComponent<ScriptGroupeComponent>();

				for (auto& script : scriptgroupeB.Scripts)
				{
					script.Scriptobject->InvokeMethod("OnCollisionExit", nullptr);
				}
			}
		}

	};

	static ContactListener* CListener;

	static b2BodyType Rigidbody2DTypeToBox2DBody(PhysicsBodyType bodyType)
	{
		switch (bodyType)
		{
		case PhysicsBodyType::Static:    return b2_staticBody;
		case PhysicsBodyType::Dynamic:   return b2_dynamicBody;
		case PhysicsBodyType::Kinematic: return b2_kinematicBody;
		}
		return b2_staticBody;
	}

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
		ent.AddComponent<TransformComponent>();
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
		RelationShipComponent& rc = entity.GetComponent<RelationShipComponent>();
		for (auto& child : rc.Children)
		{
			DeleteEntity(Entity(GetUUIDMap()[child],this));
		}
		m_Registry.destroy(entity.GetHandle());
	}

	
	void Scene::UpdateTransform(Entity entity)
	{
		RelationShipComponent& rc = entity.GetComponent<RelationShipComponent>();
		TransformComponent& ParentTransform = entity.GetComponent<TransformComponent>();

		for (auto& child : rc.Children)
		{
			Entity childEntity(GetUUIDMap()[child], this);
			TransformComponent& ChildTransform = childEntity.GetComponent<TransformComponent>();
// 			glm::mat4 mat = ParentTransform.ModelMatrix * ChildTransform.GetMatrix();
// 			glm::vec3 pos;
// 			glm::vec3 rot;
// 			glm::vec3 sca;
// 			Math::DecomposeTransform(mat, pos, rot, sca);

			ChildTransform.Position = ParentTransform.Position + ChildTransform.LocalPosition;
			ChildTransform.ModelMatrix = ChildTransform.GetMatrix();
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
					TransformComponent& tc = entity.GetComponent<TransformComponent>();
					tc.ModelMatrix = tc.GetMatrix();
					
					UpdateTransform(entity);
				}
			}
		}

		

		{
			auto view = GetRegistry().view<SpriteRendererComponent, TransformComponent>();

			for (auto entity : view)
			{


				auto& [sp, tc] = view.get<SpriteRendererComponent, TransformComponent>(entity);

				if (sp.TextureAsset.GetUUID() == 0)
				{
					BatchRenderer::DrawQuad(tc.ModelMatrix, sp.GetColor(), (uint32_t)entity);
				}
				else
				{
					BatchRenderer::DrawQuad(tc.ModelMatrix, sp.GetColor(), Application::Get().assetManager->GetAsset<Ref<Texture>>(sp.TextureAsset), (uint32_t)entity);
				}
			}
		}
		{
		
			auto view = GetRegistry().view<TextRenderer, TransformComponent>();

			for (auto entity : view)
			{
				auto& [txr, tc] = view.get<TextRenderer, TransformComponent>(entity);

				if (txr.FontAsset.GetUUID() != 0)
				{
					FontRenderer::DrawString((glm::vec2)tc.Position, tc.Scale.x, txr.Text, Application::Get().assetManager->GetAsset<Ref<Font>>(txr.FontAsset),txr.color,txr.Spacing,txr.LineSpacing,(int)entity);
				}

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
						if (Application::Get().Monoserver->CheckIfClassExists(script.Name))
						{
							script.Scriptobject = Application::Get().Monoserver->GetObject(script.Name);

							script.Loaded = true;

						}
						else
						{
							script.Loaded = false;
						}
						
					}
				}
				

			}
		}

	}

	void Scene::OnStart()
	{
		m_PhysicsWorld = CreateScope<PhysicsWorld>(glm::vec2(0.0f,-9.8f));
		CListener = new ContactListener();
		m_PhysicsWorld->GetPtr()->SetContactListener(CListener);
		SortScene();

		{
			auto view = m_Registry.view<AudioSourceComponent>();
			for (auto e : view)
			{
				Entity entity(e, this);
				AudioSourceComponent& source = entity.GetComponent<AudioSourceComponent>();
				if (source.PlayOnAwake)
				{

					if (source.AudioAsset.GetUUID() != 0)
					{
						Ref<AudioSource> audioSource = Application::Get().assetManager->GetAsset<Ref<AudioSource>>(source.AudioAsset);
						Audio::Play(audioSource);
					}

				}
			}
		}
	}

	void Scene::onUpdate()
	{

		{
			auto view = GetRegistry().view<RelationShipComponent>();

			for (auto e : view)
			{
				Entity entity(e, this);
				if (entity.GetComponent<RelationShipComponent>().Parent == 0)
				{
					TransformComponent& tc = entity.GetComponent<TransformComponent>();
					tc.ModelMatrix = tc.GetMatrix();

					UpdateTransform(entity);
				}
			}
		}
		{
			auto view = GetRegistry().view<AudioSourceComponent>();

			for (auto e : view)
			{
				Entity entity(e, this);
				AudioSourceComponent& audioSource = entity.GetComponent<AudioSourceComponent>();
				TransformComponent& transform = entity.GetComponent<TransformComponent>();
				if (audioSource.AudioAsset.GetUUID() == 0)
				{
					continue;
				}
				Ref<AudioSource> sourceAsset = Application::Get().assetManager->GetAsset<Ref<AudioSource>>(audioSource.AudioAsset);
				sourceAsset->SetSpatial(audioSource.Spatial);
				sourceAsset->SetLoop(audioSource.Loop);
				sourceAsset->SetGain(audioSource.Gain);
				sourceAsset->SetPitch(audioSource.Pitch);
				if (audioSource.Spatial)
				{
					sourceAsset->SetPosition(glm::value_ptr(transform.Position));
				}

			}
		}
		
		{
			RendererCommand::ClearColor(GetPrimaryCameraEntity().GetComponent<CameraComponent>().ClearColor);
			RendererCommand::Clear();
			BatchRenderer::Begin(GetPrimaryCameraEntity().GetComponent<TransformComponent>().ModelMatrix, GetPrimaryCameraEntity().GetComponent<CameraComponent>().Camera.GetProjection());
			auto view = GetRegistry().view<SpriteRendererComponent, TransformComponent>();

			for (auto entity : view)
			{
				

				auto& [sp, tc] = view.get<SpriteRendererComponent, TransformComponent>(entity);

				if (sp.TextureAsset.GetUUID() == 0)
				{
					BatchRenderer::DrawQuad(tc.ModelMatrix, sp.GetColor(), (uint32_t)entity);
				}
				else
				{
					BatchRenderer::DrawQuad(tc.ModelMatrix, sp.GetColor(), Application::Get().assetManager->GetAsset<Ref<Texture>>(sp.TextureAsset), (uint32_t)entity);
				}
			}
			BatchRenderer::End();


		}
		{
			FontRenderer::BeginScene(glm::ortho(0.0f, (float)ViewportWidth, 0.0f, (float)ViewportHeight));

			auto view = GetRegistry().view<TextRenderer, TransformComponent>();

			for (auto entity : view)
			{
				auto& [txr, tc] = view.get<TextRenderer, TransformComponent>(entity);

				if (txr.FontAsset.GetUUID() != 0)
				{
					FontRenderer::DrawString((glm::vec2)tc.Position, tc.Scale.x, txr.Text, Application::Get().assetManager->GetAsset<Ref<Font>>(txr.FontAsset), txr.color);
				}

			}
			FontRenderer::EndScene();
		}


		{
			const int32_t velocityIterations = 6;
			const int32_t positionIterations = 2;
			float TimeStep = 1.0f / 60.0f;
			m_PhysicsWorld->GetPtr()->Step(TimeStep, velocityIterations, positionIterations);
			
			
			auto view = GetRegistry().view<Rigidbody2DComponent>();

			for (auto e : view)
			{
				Entity entity(e, this);

				Rigidbody2DComponent& rb2d = entity.GetComponent<Rigidbody2DComponent>();
				auto& transform = entity.GetComponent<TransformComponent>();
				if (rb2d.Created == false)
				{
					b2BodyDef bodyDef;
					bodyDef.type = Rigidbody2DTypeToBox2DBody(rb2d.Type);
					bodyDef.position.Set(transform.Position.x, transform.Position.y);
					bodyDef.angle = glm::radians(transform.Rotation.z);
					bodyDef.userData.pointer = reinterpret_cast<intptr_t>(&entity.GetComponent<IDComponent>().ID);

					b2Body* body = m_PhysicsWorld->GetPtr()->CreateBody(&bodyDef);
					body->SetFixedRotation(rb2d.FixedRotation);
					rb2d.RuntimeBody = body;
					rb2d.Created = true;
				}
				b2Body* body = (b2Body*)rb2d.RuntimeBody;
				
				//body->SetTransform({ transform.Position.x ,transform.Position.y }, glm::radians(transform.Rotation.z));
				const auto& position = body->GetPosition();
				transform.Position.x = position.x;
				transform.Position.y = position.y;
				transform.Rotation.z = glm::degrees(body->GetAngle());

				if (entity.HasComponent<BoxCollider2DComponent>())
				{
					auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
					if (bc2d.Created == false)
					{
						b2PolygonShape boxShape;
						boxShape.SetAsBox(bc2d.Size.x * transform.Scale.x, bc2d.Size.y * transform.Scale.y);

						b2FixtureDef fixtureDef;
						fixtureDef.isSensor = bc2d.IsSensor;
						fixtureDef.shape = &boxShape;
						fixtureDef.density = bc2d.Density;
						fixtureDef.friction = bc2d.Friction;
						fixtureDef.restitution = bc2d.Restitution;
						fixtureDef.restitutionThreshold = bc2d.RestitutionThreshold;
						bc2d.RuntimeFixture = body->CreateFixture(&fixtureDef);
						

						bc2d.Created = true;
					}

					
					b2Fixture* fx = (b2Fixture*)bc2d.RuntimeFixture;
					fx->SetDensity(bc2d.Density);
					fx->SetFriction(bc2d.Friction);
					fx->SetRestitution(bc2d.Restitution);
					fx->SetRestitutionThreshold(bc2d.RestitutionThreshold);
					
				}

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
						if (Application::Get().Monoserver->CheckIfClassExists(script.Name))
						{
							script.Scriptobject = Application::Get().Monoserver->GetObject(script.Name);

							script.Loaded = true;

						}
						else
						{
							script.Loaded = false;
						}


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
						script.Scriptobject->InvokeMethod("Update", nullptr);

					}




				}
			}
		}
	}

	void Scene::OnStop()
	{
		delete CListener;
		{
			auto view = m_Registry.view<AudioSourceComponent>();
			for (auto e : view)
			{
				Entity entity(e, this);
				AudioSourceComponent& source = entity.GetComponent<AudioSourceComponent>();
			
				if (source.AudioAsset.GetUUID() != 0)
				{
					Ref<AudioSource> audioSource = Application::Get().assetManager->GetAsset<Ref<AudioSource>>(source.AudioAsset);
					Audio::Stop(audioSource);
				}

			}
		}
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
		CopyComponentAll<SpriteRendererComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponentAll<Rigidbody2DComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponentAll<BoxCollider2DComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponentAll<ScriptGroupeComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponentAll<CameraComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponentAll<AudioSourceComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponentAll<TextRenderer>(dstSceneRegistry, srcSceneRegistry, enttMap);
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

		if (src.HasComponent<Rigidbody2DComponent>())
		{
			CopyComponent<Rigidbody2DComponent>(src, entity);

		}

		if (src.HasComponent<BoxCollider2DComponent>())
		{
			CopyComponent<BoxCollider2DComponent>(src, entity);

		}
		if (src.HasComponent<CameraComponent>())
		{
			CopyComponent<CameraComponent>(src, entity);

		}

		if (src.HasComponent<AudioSourceComponent>())
		{
			CopyComponent<AudioSourceComponent>(src, entity);

		}

		

		return entity;
	}
	void Scene::SortScene()
	{
		m_Registry.sort<SpriteRendererComponent>([](const SpriteRendererComponent& lhs, const SpriteRendererComponent& rhs) {
			return lhs.DrawOrder < rhs.DrawOrder;
			});
	}


	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		ViewportWidth = width;
		ViewportHeight = height;
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.SetViewportSize(width, height);
		}
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		ASSERT(true != false);
	}


	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto e: view)
		{
			Entity entity(e, this);
			if (entity.GetComponent<CameraComponent>().Primary == true)
			{
				return entity;
			}
			else
			{
				return Entity();
			}
		}
	}

	template<>
	void Scene::OnComponentAdded<IDComponent>(Entity entity, IDComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<NameComponent>(Entity entity, NameComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{
		SortScene();
	}

	template<>
	void Scene::OnComponentAdded<Rigidbody2DComponent>(Entity entity, Rigidbody2DComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<ScriptGroupeComponent>(Entity entity, ScriptGroupeComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<ScriptComponent>(Entity entity, ScriptComponent& component)
	{
	}
	template<>
	void Scene::OnComponentAdded<AudioSourceComponent>(Entity entity, AudioSourceComponent& component)
	{
	}
	template<>
	void Scene::OnComponentAdded<TextRenderer>(Entity entity, TextRenderer& component)
	{
	}
}
