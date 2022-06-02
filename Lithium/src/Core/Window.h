#pragma once
#include <GLFW/glfw3.h>
#include "Renderer/VertexBuffer.h"
#include "Base.h"
#include "Event/AppEvents.h"
#include "Event/Event.h"
#include "glm/glm.hpp"

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
		void SetVsync(bool vsync);
		bool IsVsync();
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

		glm::vec2 getSize()
		{
			int width, height;

			glfwGetWindowSize(window, &width, &height);
			return glm::vec2(width, height);
		}


		void SetAppEventCallback(const EventCallback& e)  { _Data.callback = e; }
		void SetWindowName(const std::string& name);
		void* GetNativeWindow() const { return window; }
	private:
		GLFWwindow* window;
		bool m_Vsync = true;


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
