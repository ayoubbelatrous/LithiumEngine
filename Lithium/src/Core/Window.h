#pragma once
#include <GLFW/glfw3.h>
#include "Renderer/VertexBuffer.h"
#include "Base.h"
#include "Event/AppEvents.h"
#include "Event/Event.h"

namespace Lithium
{
	class Window
	{
	public:

		using EventCallback = std::function<void(Event&)>;

		Window();
		~Window();
		void Init();
		void OnUpdate();
		bool WindowClosing()
		{
			return glfwWindowShouldClose(window);
		}

		
		GLFWwindow* GetglfwWindowptr()
		{
			return window;
		}


		GLFWwindow* GETWindow()
		{
			return window;
		}

		void SetAppEventCallback(const EventCallback& e)  { _Data.callback = e; }
	private:
		GLFWwindow* window;



		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallback callback;
		};
		WindowData _Data;
	};

}
