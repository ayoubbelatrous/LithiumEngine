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
		void SetSelection(Entity& selection)
		{
			m_Selection = selection;
		}
	private:
		Entity m_Selection;
	};
}
