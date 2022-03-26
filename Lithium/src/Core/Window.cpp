#include "lipch.h"
#include "Window.h"
#include "glad/glad.h"
#include "Input/Input.h"




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

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

		window = glfwCreateWindow(1280, 780, "Lithium", NULL, NULL);
		glfwMakeContextCurrent(window);
		
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cout << "Failed to initialize OpenGL context" << std::endl;
		}
		
		GLuint id = 0;
		const char* message​ = "hello";
		glViewport(0, 0, 1280, 780);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		int number;
		//glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &number);
		//CORE_LOG(number);
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

		glfwSetScrollCallback(window, [](GLFWwindow* win, double xoffset,double yoffset)
		{
				WindowData& wdata = *(WindowData*)glfwGetWindowUserPointer(win);
				MouseWheelEvent ev = MouseWheelEvent((float)xoffset,(float)yoffset);
				wdata.callback(ev);
		});
		glfwSetDropCallback(window, [](GLFWwindow* win, int count, const char** paths)
		{
				CORE_LOG(paths[0]);
				WindowData& wdata = *(WindowData*)glfwGetWindowUserPointer(win);
				WindowFileDropEvent ev = WindowFileDropEvent(count, paths);
				wdata.callback(ev);
		});

	}

	void Window::OnUpdate()
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	void Window::SetVsync(bool vsync)
	{
		m_Vsync = vsync;
		glfwSwapInterval(vsync);
	}

	bool Window::IsVsync()
	{
		return m_Vsync;
	}

}
