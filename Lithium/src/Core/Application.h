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
		Ref<Window> wind;
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
