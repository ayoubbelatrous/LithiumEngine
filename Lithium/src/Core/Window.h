#pragma once
#include <GLFW/glfw3.h>


namespace Lithium
{
	class Window
	{
	public:
		Window();
		~Window();
		void Init();
		void OnUpdate();
	private:
		GLFWwindow* window;
	};
}
