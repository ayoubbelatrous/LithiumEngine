#pragma once
#include "LayerStack.h"


namespace Lithium
{
	class Application
	{
	private:
		LayerStack _Stack;
	public:
		Application();
		~Application();
		void PushLayer(Layer* layer);
		void Run();
	};

	//gets defined in the editor
	Application* CreateApp();
}
