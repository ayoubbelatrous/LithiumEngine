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
#include "Core/UUID.h"

namespace Lithium
{
	struct PhysicsProperties
	{
		glm::vec2 Gravity = { 0,-9.8 };
	};
	struct ProjectSettings
	{
		PhysicsProperties PhysicsProps;
	};
	struct ApplicationProperties
	{
		std::string WorkingDirectory;
		ProjectSettings ProjSettings; 
	};
	class Application
	{
	private:
		LayerStack _Stack;
		Scope<Window> _Window;
		bool _running = true;
		GUIlayer* _ImguiLayer;
		double m_PreviousTime = 0;
		double m_DeltaTime = 0;
		bool m_Minimized = false;

		ApplicationProperties m_ApplicationProps;
		void OnProjectPropertiesChanged();
	public:
		Application();
		~Application();

		double GetDeltaTime() const { return m_DeltaTime; }
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		void Run();
		void OnEvent(Event& e);
		void SceneOnEvent(Event& e);
		void SetVsync(bool vsync);
		bool IsVsync();
		GUIlayer* GetImguiLayer() { return _ImguiLayer; }
		Window& GetWindow();
		Ref<AssetManager> assetManager;
		Ref<SceneManager> sceneManager;
		Ref<MonoServer> Monoserver;
		static Application& Get() { return *instance; };
		static Application* instance;
		void SetApplicationProperties(ApplicationProperties props);
		ApplicationProperties GetApplicationProperties();
		void ReloadAssetManager();
	};

	//gets defined in the editor
	Application* CreateApp();
}
