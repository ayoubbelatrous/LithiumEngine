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
			auto& [tc, sc] = view.get<TransformComponent, SpriteRendererComponent>(entity);
			Renderer2D::DrawQuad(tc.GetMatrix(), sc.GetColor());
		}

	}

}
