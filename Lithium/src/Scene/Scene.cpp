#include "lipch.h"

#include "Scene.h"
#include "Entity.h"

namespace Lithium
{

	Scene::Scene()
		:_Registry(entt::registry())
	{}

	
	
	Entity Scene::CreateEntity()
	{
		return {_Registry.create(),this };
	}

}
