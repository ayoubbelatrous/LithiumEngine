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

		window = glfwCreateWindow(1280, 780, "Lithium", NULL, NULL);
		glfwMakeContextCurrent(window);
		
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cout << "Failed to initialize OpenGL context" << std::endl;
		}

		glViewport(0, 0, 1280, 780);
		
		CORE_LOG(glGetString(GL_VERSION));

		glfwSetWindowUserPointer(window, &_Data);
		

		glfwSetWindowCloseCallback(window, [](GLFWwindow* win)
		{
			WindowData& wdata = *(WindowData*)glfwGetWindowUserPointer(win);
			WindowCloseEvent ev;
			wdata.callback(ev);
		});


		glfwSetWindowSizeCallback(window, [](GLFWwindow* win,int w,int h)
		{
			WindowData& wdata = *(WindowData*)glfwGetWindowUserPointer(win);
			WindowResizeEvent ev(w,h);
			wdata.callback(ev);
			});
		glfwSetKeyCallback(window, [](GLFWwindow* win,int key, int scancode, int action, int mods)
		{
			WindowData& wdata = *(WindowData*)glfwGetWindowUserPointer(win);
			KeyEvent ev = KeyEvent(key, action, mods, scancode);
			wdata.callback(ev);
		});

	}

	void Window::OnUpdate()
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


}
