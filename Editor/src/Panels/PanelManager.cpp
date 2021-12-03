#include "PanelManager.h"



namespace Lithium
{

	PanelManager::PanelManager()
		:_Panels(std::vector<Ref<Panel>>())
	{
		
	}

	void PanelManager::onUpdate()
	{
		for (Ref<Panel> panel: _Panels)
		{
			panel->SetScene(_Scene);
			panel->onUpdate();
		}
	}

	void PanelManager::PushPanel(Ref<Panel> panel)
	{
	
		_Panels.emplace(_Panels.begin(), panel);

	}

	void PanelManager::SetScene(Ref<Scene> scene)
	{
		_Scene = scene;
	}

}