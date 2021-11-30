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
		instance = this;
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

			if (_Window->WindowClosing())
				_running = false;
		}
	}


	Window& Application::GetWindow()
	{
		return *_Window;
	}

	

}
