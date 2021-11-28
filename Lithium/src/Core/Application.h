#pragma once
#include "LayerStack.h"
#include "Window.h"

namespace Lithium
{
	class Application
	{
	private:
		LayerStack _Stack;
		Window wind;
	public:
		Application();
		~Application();
		void PushLayer(Layer* layer);
		void Run();
	};

	//gets defined in the editor
	Application* CreateApp();
}
