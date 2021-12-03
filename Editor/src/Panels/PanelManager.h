#pragma once
#include <vector>
#include "Panel.h"
#include "Core/Base.h"
#include "Scene/Scene.h"


namespace Lithium
{
	class PanelManager
	{
	public:
		PanelManager();
		std::vector<Ref<Panel>>::iterator begin() { return _Panels.begin(); }
		std::vector<Ref<Panel>>::iterator end() { return _Panels.end(); }
		void onUpdate();
		void PushPanel(Ref<Panel> panel);
		void SetScene(Ref<Scene> scene);
		
	private:
		std::vector<Ref<Panel>> _Panels;
		Ref<Scene> _Scene;
	};
}