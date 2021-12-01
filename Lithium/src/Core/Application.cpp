#include "lipch.h"
#include "Application.h"
#include "Debug/Profiler.h"
#include "glm.hpp"
#include "../Imgui/ImguiLayer.h"

namespace Lithium
{
	Application* Application::instance = nullptr;
	Application::Application()
	{
		_Window = CreateScope<Window>();
		_Window->Init();
		PushLayer(new GUIlayer);
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
		for (Layer* layer : _Stack)
		{
			layer->onEvent(e);
		}
		if (e.GetEventType() == EventType::WindowClose)
		{
			_running = false;
		}
	}

	Window& Application::GetWindow()
	{
		return *_Window;
	}

	

}
