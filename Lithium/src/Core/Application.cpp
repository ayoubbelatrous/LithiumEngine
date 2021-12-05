#include "lipch.h"
#include "Application.h"
#include "Debug/Profiler.h"
#include "glm.hpp"

namespace Lithium
{
	Application* Application::instance = nullptr;
	Application::Application()
	{
		_Window = CreateScope<Window>();
		_Window->Init();
		_ImguiLayer = new GUIlayer();
		PushOverlay(_ImguiLayer);
		_Window->SetAppEventCallback([this](auto&&... args) -> decltype(auto)
		{ 
				return this->Application::OnEvent(std::forward<decltype(args)>(args)...);
		});

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
		
		}
	}

	Window& Application::GetWindow()
	{
		return *_Window;
	}
}
