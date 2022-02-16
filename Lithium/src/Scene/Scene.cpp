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
		Entity ent(_Registry.create(), this);
		ent.AddComponent<NameComponent>(name);
		ent.AddComponent<ChildManagerComponent>();
		CreateEntityEvent e = CreateEntityEvent();
		//callback(e);
		return ent;
	}

	void Scene::DeleteEntity(Entity entity)
	{
		_Registry.destroy(entity.GetHandle());
	}

	void Scene::onEditorUpdate()
	{
		auto view = GetRegistry().view<TransformComponent, MeshRendererComponent,MeshComponent,MaterialComponent>();
		for (auto entity : view)
		{
			auto& [tc, msh] = view.get<TransformComponent, MeshComponent>(entity);
		
			Entity ent = { entity,this };
			MaterialComponent mtc = ent.GetComponent<MaterialComponent>();
			Ref<Material> material = mtc.material;

			Ref<Shader> shader = material->GetShader();
			
			shader->Bind();
			for (auto field : material->GetDataFields())
			{
				if (field->getType() == MaterialDataType::Vec4)
				{
					Vec4* vec4 = (Vec4*)field.get();
					shader->SetUniform4f(vec4->name,vec4->Value);
				}
			}
			shader->SetUniformMat4f("model", tc.GetMatrix());
			
			msh._Mesh->Render();
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
