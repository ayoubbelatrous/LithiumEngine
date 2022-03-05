#pragma once
#include "LayerStack.h"
#include "Window.h"
#include "Base.h"
#include "Event/AppEvents.h"
#include "Event/Event.h"
#include "../Imgui/ImguiLayer.h"
#include "AssetManager/AssetManager.h"
#include "Scene/SceneManager.h"
#include "Script/MonoServer.h"

namespace Lithium
{
	class Application
	{
	private:
		LayerStack _Stack;
		Scope<Window> _Window;
		bool _running = true;
		GUIlayer* _ImguiLayer;
		double m_PreviousTime = 0;
		double m_DeltaTime = 0;
	public:
		Application();
		~Application();
	
		double GetDeltaTime() const {return m_DeltaTime; }
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		void Run();
		void OnEvent(Event& e);
		void SceneOnEvent(Event& e);
		GUIlayer* GetImguiLayer() { return _ImguiLayer; }
		Window& GetWindow();
		Ref<AssetManager> assetManager;
		Ref<SceneManager> sceneManager;
		static Application& GetInstance() { return *instance; };
		static Application* instance;
	};
	
	//gets defined in the editor
	Application* CreateApp();
}
