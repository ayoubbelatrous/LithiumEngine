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

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

		window = glfwCreateWindow(500, 500, "Lithium", NULL, NULL);
		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cout << "Failed to initialize OpenGL context" << std::endl;
		}

		glViewport(0, 0, 500, 500);
		
		CORE_LOG(glGetString(GL_VERSION));

	}

	void Window::OnUpdate()
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

}
