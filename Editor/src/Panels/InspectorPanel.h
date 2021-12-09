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
		void OnUpdate();
		void SetScene(Ref<Scene> scene)
		{
			_Scene = scene;
		}
		void SetSelection(Entity& selection)
		{
			_Selection = selection;
		}
	private:
		Ref<Scene> _Scene;
		Entity _Selection;
	};
}
