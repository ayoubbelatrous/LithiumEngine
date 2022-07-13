#include "lipch.h"
#include "Scene/Entity.h"
#include "Scene/Components.h"
#include "Renderer/Renderer2D.h"
#include "Script/MonoServer.h"
#include "glm/gtc/type_ptr.hpp"
#include "Core/Application.h"
#include "Core/Math.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#include "box2d/box2d.h"
#include "AssetManager/AssetManager.h"
#include "Audio/Audio.h"
#include "Font/FontRenderer.h"
#include "Renderer/UIRenderer.h"
#include "Input/Input.h"


namespace Lithium
{

	class ContactListener : public b2ContactListener
	{
	public:
		void BeginContact(b2Contact* contact)
		{
			UUID* IDA = reinterpret_cast<UUID*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
			Entity entityA(Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[*IDA], Application::Get().sceneManager->GetActiveScene().get());
			UUID* IDB = reinterpret_cast<UUID*>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);
			Entity entityB(Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[*IDB], Application::Get().sceneManager->GetActiveScene().get());

			if (entityA.HasComponent<ScriptGroupeComponent>())
			{
				ScriptGroupeComponent& scriptgroupeA = entityA.GetComponent<ScriptGroupeComponent>();

				for (auto& script : scriptgroupeA.Scripts)
				{

					void* Args[1];
					Args[0] = Application::Get().Monoserver->CreateMonoCollsion2D(*IDB);
					script.Scriptobject->InvokeMethod("OnCollisionEnter", Args);
				}
			}


			if (entityB.HasComponent<ScriptGroupeComponent>())
			{
				ScriptGroupeComponent& scriptgroupeB = entityB.GetComponent<ScriptGroupeComponent>();

				for (auto& script : scriptgroupeB.Scripts)
				{

					void* Args[1];
					Args[0] = Application::Get().Monoserver->CreateMonoCollsion2D(*IDA);
					script.Scriptobject->InvokeMethod("OnCollisionEnter", Args);
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

			UUID* IDB = reinterpret_cast<UUID*>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);
			Entity entityB(Application::Get().sceneManager->GetActiveScene()->GetUUIDMap()[*IDB], Application::Get().sceneManager->GetActiveScene().get());

			if (entityB.HasComponent<ScriptGroupeComponent>())
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
		m_SceneID = UUID();
	}


	template<typename Component>
	void Scene::CopyComponentAll(entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
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

	Entity Scene::CreateUIEntity(const std::string& name)
	{
		Entity ent(m_Registry.create(), this);
		ent.AddComponent<IDComponent>(UUID());
		ent.AddComponent<NameComponent>(name);
		ent.AddComponent<RelationShipComponent>();
		ent.AddComponent<RectTrasnformComponent>();
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
			DeleteEntity(Entity(GetUUIDMap()[child], this));
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
			auto view = GetRegistry().view<RelationShipComponent, TransformComponent>();

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
			auto view = GetRegistry().view<AnimatorComponent>();

			for (auto e : view)
			{
				Entity entity(e, this);
				entity.GetComponent<AnimatorComponent>();
			}
		}
		{
			auto view = GetRegistry().view<SpriteRendererComponent, TransformComponent>();

			for (auto entity : view)
			{


				auto [sp, tc] = view.get<SpriteRendererComponent, TransformComponent>(entity);

				if (sp.TextureAsset.GetUUID() == 0)
				{
					BatchRenderer::DrawQuad(tc.ModelMatrix, sp.GetColor(), (uint32_t)entity);

				}
				else
				{
					Ref<Texture> textureAsset = Application::Get().assetManager->GetAsset<Ref<Texture>>(sp.TextureAsset);;

					TextureMetaData textureMetaData = Application::Get().assetManager->GetAssetMetaData<TextureMetaData>(sp.TextureAsset);

					if (textureMetaData.Mode == TextureMetaData::TextureMode::Multiple)
					{
						glm::vec2 cellsize = glm::vec2(textureMetaData.CellsizeX, textureMetaData.CellsizeY);
						int Width = textureAsset->GetWidth();
						int Height = textureAsset->GetWidth();
						int x = sp.TextureIndex / (Width / cellsize.x);
						int y = sp.TextureIndex % (int)(Width / cellsize.x);
						glm::vec2 index = glm::vec2(x, y);

						glm::vec2 textureCoords[] = {
							{ (index.x * cellsize.x) / Width, (index.y * cellsize.y) / Height},
							{ ((index.x + 1) * cellsize.x) / Width, (index.y * cellsize.y) / Height},
							{ ((index.x + 1) * cellsize.x) / Width, ((index.y + 1) * cellsize.y) / Height},
							{ (index.x * cellsize.x) / Width, ((index.y + 1) * cellsize.y) / Height},
						};
						BatchRenderer::DrawQuadSubTexture(tc.ModelMatrix, sp.GetColor(), textureCoords, textureAsset, (uint32_t)entity);
					}
					else if (textureMetaData.Mode == TextureMetaData::TextureMode::Single)
					{
						BatchRenderer::DrawQuad(tc.ModelMatrix, sp.GetColor(), textureAsset, (uint32_t)entity);
					}
				}
			}
		}
		if (m_RenderEditorUI)
		{

			{
				auto view = GetRegistry().view<TextRenderer, TransformComponent>();

				for (auto entity : view)
				{
					auto [txr, tc] = view.get<TextRenderer, TransformComponent>(entity);

					if (txr.FontAsset.GetUUID() != 0)
					{
						FontRenderer::DrawString((glm::vec2)tc.Position, tc.Scale.x, txr.Text, Application::Get().assetManager->GetAsset<Ref<Font>>(txr.FontAsset), txr.color, txr.Spacing, txr.LineSpacing, (int)entity);
					}

				}
			}

			{
				auto view = GetRegistry().view<ButtonComponent, TransformComponent>();

				for (auto entity : view)
				{
					auto [btn, tc] = view.get<ButtonComponent, TransformComponent>(entity);
					UIRenderer::DrawQuad(tc.ModelMatrix, btn.CurrentColor, (int)entity);
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
		m_PhysicsWorld = CreateScope<PhysicsWorld>(glm::vec2(0.0f, -9.8f));
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
			auto view = GetRegistry().view<AnimatorComponent>();

			for (auto e : view)
			{
				Entity entity(e, this);
				AnimatorComponent& animator = entity.GetComponent<AnimatorComponent>();
				if (animator.IsPlaying)
				{
					Ref<Animation> animation = Application::Get().assetManager->GetAsset<Ref<Animation>>(animator.AnimationAsset);
					animation->Update(UUID((uint64_t)e), animator.CurrentTime);
				}
			}
		}

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


				auto [sp, tc] = view.get<SpriteRendererComponent, TransformComponent>(entity);

				if (sp.TextureAsset.GetUUID() == 0)
				{
					BatchRenderer::DrawQuad(tc.ModelMatrix, sp.GetColor(), (uint32_t)entity);

				}
				else
				{
					Ref<Texture> textureAsset = Application::Get().assetManager->GetAsset<Ref<Texture>>(sp.TextureAsset);;

					TextureMetaData textureMetaData = Application::Get().assetManager->GetAssetMetaData<TextureMetaData>(sp.TextureAsset);

					if (textureMetaData.Mode == TextureMetaData::TextureMode::Multiple)
					{
						glm::vec2 cellsize = glm::vec2(textureMetaData.CellsizeX, textureMetaData.CellsizeY);
						int Width = textureAsset->GetWidth();
						int Height = textureAsset->GetWidth();
						int x = sp.TextureIndex / (Width / cellsize.x);
						int y = sp.TextureIndex % (int)(Width / cellsize.x);
						glm::vec2 index = glm::vec2(x, y);

						glm::vec2 textureCoords[] = {
							{ (index.x * cellsize.x) / Width, (index.y * cellsize.y) / Height},
							{ ((index.x + 1) * cellsize.x) / Width, (index.y * cellsize.y) / Height},
							{ ((index.x + 1) * cellsize.x) / Width, ((index.y + 1) * cellsize.y) / Height},
							{ (index.x * cellsize.x) / Width, ((index.y + 1) * cellsize.y) / Height},
						};
						BatchRenderer::DrawQuadSubTexture(tc.ModelMatrix, sp.GetColor(), textureCoords, textureAsset, (uint32_t)entity);
					}
					else if (textureMetaData.Mode == TextureMetaData::TextureMode::Single)
					{
						BatchRenderer::DrawQuad(tc.ModelMatrix, sp.GetColor(), textureAsset, (uint32_t)entity);
					}
				}
			}
			{
				auto view = GetRegistry().view<ParticleSystemRenderer, TransformComponent>();

				for (auto entity : view)
				{

					auto [psr, tc] = view.get<ParticleSystemRenderer, TransformComponent>(entity);
					psr.OnUpdate(tc.Position);
					psr.OnRender();
				}
			}


			BatchRenderer::End();


		}
		{
			FontRenderer::BeginScene(glm::ortho(0.0f, (float)m_ViewportWidth, 0.0f, (float)m_ViewportHeight));

			auto view = GetRegistry().view<TextRenderer, TransformComponent>();

			for (auto entity : view)
			{
				auto [txr, tc] = view.get<TextRenderer, TransformComponent>(entity);

				if (txr.FontAsset.GetUUID() != 0)
				{
					FontRenderer::DrawString((glm::vec2)tc.Position, tc.Scale.x, txr.Text, Application::Get().assetManager->GetAsset<Ref<Font>>(txr.FontAsset), txr.color);
				}

			}
			FontRenderer::EndScene();
			UIRenderer::BeginScene(glm::ortho(0.0f, (float)m_ViewportWidth, 0.0f, (float)m_ViewportHeight));
			{
				auto view = GetRegistry().view<ButtonComponent, TransformComponent>();

				for (auto entity : view)
				{
					auto [btn, tc] = view.get<ButtonComponent, TransformComponent>(entity);
					UIRenderer::DrawQuad(tc.ModelMatrix, btn.CurrentColor, (int)entity);
					glm::vec2 Max = { tc.Position.y + (tc.Scale.y / 2),tc.Position.x + (tc.Scale.x / 2) };
					glm::vec2 Min = { tc.Position.y - (tc.Scale.y / 2),tc.Position.x - (tc.Scale.x / 2) };
					float MouseX, MouseY;
					float mx, my;
					mx = ImGui::GetMousePos().x;
					my = ImGui::GetMousePos().y;
					mx -= m_Bounds[0].x;
					my -= m_Bounds[0].y;
					glm::vec2 vs = m_Bounds[1] - m_Bounds[0];
					my = vs.y - my;
					MouseX = (int)mx;
					MouseY = (int)my;

					if (btn.CheckIntersection({ MouseY,MouseX }, Min, Max))
					{
						if (Input::IsMouseKeyPressed(0))
						{
							btn.CurrentColor = btn.PressColor;
						}
						else
						{
							btn.CurrentColor = btn.HoveredColor;
						}
					}
					else
					{
						btn.CurrentColor = btn.Color;
					}

				}
			}
			UIRenderer::EndScene();
		}


		{
			const int32_t velocityIterations = 6;
			const int32_t positionIterations = 2;
			float TimeStep = 1.0f / 60.0f;
			m_PhysicsWorld->GetPtr()->Step(Application::Get().GetDeltaTime(), velocityIterations, positionIterations);


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

				if (entity.HasComponent<CircleCollider2DComponent>())
				{
					auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();
					if (cc2d.Created == false)
					{

						b2CircleShape circleShape;
						circleShape.m_p.Set(cc2d.Offset.x, cc2d.Offset.y);
						circleShape.m_radius = transform.Scale.x * cc2d.Radius;

						b2FixtureDef fixtureDef;
						fixtureDef.shape = &circleShape;
						fixtureDef.density = cc2d.Density;
						fixtureDef.friction = cc2d.Friction;
						fixtureDef.restitution = cc2d.Restitution;
						fixtureDef.restitutionThreshold = cc2d.RestitutionThreshold;
						cc2d.RuntimeFixture = body->CreateFixture(&fixtureDef);

						cc2d.Created = true;
					}


					b2Fixture* fx = (b2Fixture*)cc2d.RuntimeFixture;
					fx->SetDensity(cc2d.Density);
					fx->SetFriction(cc2d.Friction);
					fx->SetRestitution(cc2d.Restitution);
					fx->SetRestitutionThreshold(cc2d.RestitutionThreshold);

				}

			}
		}

		{
			auto view = GetRegistry().view<ScriptGroupeComponent>();

			for (auto entity : view)
			{
				Entity ent = { entity,this };

				for (auto& script : ent.GetComponent<ScriptGroupeComponent>().Scripts)
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

	template<typename T>
	void Scene::CopyComponent(Entity src, Entity dst)
	{
		if (src.HasComponent<T>())
			dst.AddOrReplaceComponent<T>(src.GetComponent<T>());
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
		CopyComponentAll<AnimatorComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponentAll<ParticleSystemRenderer>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponentAll<CircleCollider2DComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponentAll<ButtonComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);

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

	void Scene::OnViewportResize(uint32_t width, uint32_t height, glm::vec2 Bounds[2])
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;
		m_Bounds[0] = Bounds[0];
		m_Bounds[1] = Bounds[1];
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.SetViewportSize(width, height);
		}
	}

	void Scene::SetRenderEditorUi(bool render)
	{
		m_RenderEditorUI = render;
	}

	bool Scene::GetRenderEditorUi()
	{
		return m_RenderEditorUI;
	}

	uint64_t Scene::GetSceneID()
	{
		return m_SceneID;
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		ASSERT(true != false);
	}


	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto e : view)
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
	template<>
	void Scene::OnComponentAdded<AnimatorComponent>(Entity entity, AnimatorComponent& component)
	{
	}
	template<>
	void Scene::OnComponentAdded<ParticleSystemRenderer>(Entity entity, ParticleSystemRenderer& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CircleCollider2DComponent>(Entity entity, CircleCollider2DComponent& component)
	{
	}
	template<>
	void Scene::OnComponentAdded<RectTrasnformComponent>(Entity entity, RectTrasnformComponent& component)
	{
	}
	template<>
	void Scene::OnComponentAdded<ButtonComponent>(Entity entity, ButtonComponent& component)
	{
	}

}

