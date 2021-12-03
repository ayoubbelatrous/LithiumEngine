#include "lipch.h"
#include "Components.h"
#include "Scene.h"
#include "Entity.h"

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

}
