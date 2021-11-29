#include "lipch.h"
#include "Application.h"
#include "Debug/Profiler.h"
#include "glm.hpp"

namespace Lithium
{
	
	Application::Application()
	{
		_Window = CreateScope<Window>();
		_Window->Init();

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

}
