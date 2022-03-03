#include "lipch.h"
#include "Application.h"
#include "Debug/Profiler.h"
#include "glm.hpp"
#include "Math.h"

namespace Lithium
{
	Application* Application::instance = nullptr;
	Application::Application()
	{
		_Window = CreateScope<Window>();
		_Window->Init();
#ifndef STATIC_RUNTIME
		_ImguiLayer = new GUIlayer();
		PushOverlay(_ImguiLayer);
#endif
		_Window->SetAppEventCallback(BIND_EVENT(Application::OnEvent));
		Math::Random::Init();
		assetManager = CreateRef<AssetManager>();
		sceneManager = CreateRef<SceneManager>();
	}
	Application::~Application()
	{
		for (Layer* layer : _Stack)
		{
			layer->OnDestroy();
		}
		
	}

	void Application::PushLayer(Layer* layer)
	{
		instance = this;
		_Stack.PushLayer(layer);
		layer->OnCreate();
	}

	void Application::PushOverlay(Layer* layer)
	{
		instance = this;
		_Stack.PushOverlay(layer);
		layer->OnCreate();
	}

	void Application::Run()
	{
		

		while (_running)
		{
			double time = (double)glfwGetTime();
			m_DeltaTime = time - m_PreviousTime;
			m_PreviousTime = time;
			for (Layer* layer:_Stack)
			{
				layer->OnUpdate();
			}

			_Window->OnUpdate();
		}
	}


	void Application::OnEvent(Event& e)
	{


		for (auto it = _Stack.rbegin(); it != _Stack.rend(); ++it)
		{
			if (e._Handeled)
				break;
			(*it)->onEvent(e);
		}
		if (e.GetEventType() == EventType::WindowClose)
		{
			_running = false;
		}
		if (e.GetEventType() == EventType::WindowResize)
		{
			WindowResizeEvent& resizeevent = static_cast<WindowResizeEvent&>(e);
			int height = resizeevent.height;
			int width = resizeevent.width;
		
		}
	}

	void Application::SceneOnEvent(Event& e)
	{
		for (auto it = _Stack.rbegin(); it != _Stack.rend(); ++it)
		{
			if (e._Handeled)
				break;
			(*it)->onEvent(e);
		}
	}

	Window& Application::GetWindow()
	{
		return *_Window;
	}
}
