#include "lipch.h"
#include "Application.h"
#include "Base.h"
#include <GLFW/glfw3.h>

namespace Lithium
{
	
	Application::Application()
	{
		GLFWwindow* window;

		/* Initialize the library */
		glfwInit();
			

		/* Create a windowed mode window and its OpenGL context */
		window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

		/* Make the window's context current */
		glfwMakeContextCurrent(window);

		/* Loop until the user closes the window */
	

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
		}
	}

}
