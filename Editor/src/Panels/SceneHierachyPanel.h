#pragma once
#include "Scene/Entity.h"
#include "Scene/Scene.h"
#include "Core/Base.h"

namespace Lithium
{
	class SceneHierachyPanel
	{
	public:
		void OnCreate();
		void OnUpdate(Entity& _Selection);
		void DrawEntity(Entity entity,Entity& _Selection);
		void SetScene(Ref<Scene> scene)
		{
			_Scene = scene;
		}
	private:
		Ref<Scene> _Scene;
	};
}