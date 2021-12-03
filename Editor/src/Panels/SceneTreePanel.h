#pragma once
#include "Panel.h"
#include "Scene/Scene.h"
#include "imgui.h"
#include "Scene/Entity.h"

namespace Lithium
{
	class SceneTreePanel : public Panel
	{
	public:
		void onCreate() override;
		void onUpdate() override;
		void SetScene(Ref<Scene> scene) override;
		void DrawEntity(Entity entity);
		void SetSelection(Entity& entity)
		{
			entity =  _Selection;
		}
	private:
		Ref<Scene> _Scene;
		Entity _Selection;
	};
}