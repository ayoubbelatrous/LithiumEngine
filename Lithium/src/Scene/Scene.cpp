#include "lipch.h"
#include "Components.h"
#include "Scene.h"
#include "Entity.h"
#include "Renderer/Renderer2D.h"

namespace Lithium
{

	Scene::Scene()
		:_Registry(entt::registry())
	{}


	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity ent(_Registry.create(),this);
		ent.AddComponent<NameComponent>(name);
		ent.AddComponent<ChildManagerComponent>();
		CreateEntityEvent e = CreateEntityEvent();
		callback(e);
		return ent;
	}

	void Scene::DeleteEntity(Entity entity)
	{
		_Registry.destroy(entity.GetHandle());
	}

	void Scene::onEditorUpdate()
	{
		auto view = GetRegistry().view<TransformComponent, SpriteRendererComponent>();
		for (auto entity : view)
		{
			auto [tc, sc] = view.get<TransformComponent, SpriteRendererComponent>(entity);
			Entity ent = {entity,this};
			//NameComponent& name = ent.GetComponent<NameComponent>();
		
			if (sc.tex->loaded)
			{
				BatchRenderer::DrawQuad(tc.GetMatrix(), sc.Color, sc.tex,(int)entity);

			}
			else
			{
				BatchRenderer::DrawQuad(tc.GetMatrix(), sc.Color,(int)entity);

			}
			
		}
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
