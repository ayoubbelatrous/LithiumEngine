#pragma once
#include "LayerStack.h"
#include "Window.h"
#include "Base.h"
#include "Event/AppEvents.h"
#include "Event/Event.h"
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
		void OnEvent(Event& e);
		Window& GetWindow();
		static Application& GetInstance() { return *instance; };
		static Application* instance;
	};
	
	//gets defined in the editor
	Application* CreateApp();
}
