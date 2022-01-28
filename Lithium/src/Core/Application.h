#pragma once
#include "LayerStack.h"
#include "Window.h"
#include "Base.h"
#include "Event/AppEvents.h"
#include "Event/Event.h"
#include "../Imgui/ImguiLayer.h"

namespace Lithium
{
	class Application
	{
	private:
		LayerStack _Stack;
		Scope<Window> _Window;
		bool _running = true;
		GUIlayer* _ImguiLayer;
	public:
		Application();
		~Application();
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		void Run();
		void OnEvent(Event& e);
		void SceneOnEvent(Event& e);
		GUIlayer* GetImguiLayer() { return _ImguiLayer; }
		Window& GetWindow();
		static Application& GetInstance() { return *instance; };
		static Application* instance;
	};
	
	//gets defined in the editor
	Application* CreateApp();
}
