#pragma once
#include "LayerStack.h"
#include "Window.h"
#include "Base.h"
namespace Lithium
{
	class Application
	{
	private:
		LayerStack _Stack;
		Scope<Window> _Window;
		bool _running = true;
	public:
		Application();
		~Application();
		void PushLayer(Layer* layer);
		void Run();
	};

	//gets defined in the editor
	Application* CreateApp();
}
