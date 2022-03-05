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
		void OnUpdate();
		void DrawEntity(Entity entity);
		void SetScene(Ref<Scene> scene)
		{
			_Scene = scene;
			_Selection = {};
		}
		Entity GetSelection()
		{
			return _Selection;
		}

		void SetSelection(Entity selec)
		{
			_Selection = selec;
		}
	private:
		Ref<Scene> _Scene;
		Entity _Selection;
	};
}