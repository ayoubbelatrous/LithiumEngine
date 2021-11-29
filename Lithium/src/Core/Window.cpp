#include "lipch.h"
#include "Window.h"
#include "glad/glad.h"

namespace Lithium
{

	Window::Window()
	{
		
	}

	Window::~Window()
	{

	}

	void Window::Init()
	{
		glfwInit();
		window = glfwCreateWindow(500, 500, "Lithium", NULL, NULL);
		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cout << "Failed to initialize OpenGL context" << std::endl;
		}

		glViewport(0, 0, 500, 500);
	}

	void Window::OnUpdate()
	{

		glClearColor(.25,.25, .25, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

}
