#pragma once
#include <GLFW/glfw3.h>
#include "Renderer/VertexBuffer.h"

#include "Base.h"


namespace Lithium
{
	class Window
	{
	public:
		Window();
		~Window();
		void Init();
		void OnUpdate();
		bool WindowClosing()
		{
			return glfwWindowShouldClose(window);
		}

		void SetResizeCallback(const void* func);
		GLFWwindow& GetglfwWindowptr()
		{
			return *window;
		}
	private:
		GLFWwindow* window;

	};



}
