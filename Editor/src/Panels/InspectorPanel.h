#pragma once
#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include "Core/Base.h"
namespace Lithium
{
	class InspectorPanel
	{
	public:
		void OnCreate();
		void OnUpdate(Entity& _Selection);
		void SetScene(Ref<Scene>& scene)
		{
			_Scene = scene;
		}
	private:
		Ref<Scene> _Scene;
	};
}
