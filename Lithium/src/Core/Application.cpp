#include "lipch.h"
#include "Application.h"

namespace Lithium
{
	
	Application::Application()
	{
		wind = CreateRef<Window>();
		wind->Init();
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

			wind->OnUpdate();

			if (wind->WindowClosing())
				_running = false;
		}
	}

}
