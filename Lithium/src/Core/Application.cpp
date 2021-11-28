#include "lipch.h"
#include "Application.h"

namespace Lithium
{
	
	Application::Application()
	{
		wind = Window();
		wind.Init();
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
		while (true)
		{
			for (Layer* layer:_Stack)
			{
				layer->OnUpdate();
			}

			
			wind.OnUpdate();
		}
	}

}
