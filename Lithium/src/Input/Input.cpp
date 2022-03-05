#include "lipch.h"
#include "Input.h"
#include "InputKeyCodes.h"
#include "Core//Application.h"
#include <GLFW/glfw3.h>


namespace Lithium
{

	bool Input::IsKeyPressed(const KeyCode keycode)
	{
		auto* window = Application::Get().GetWindow().GETWindow();
		auto state = glfwGetKey(window, static_cast<int32_t>(keycode));
		
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseKeyPressed(int mousekey)
	{
		auto* window = Application::Get().GetWindow().GETWindow();
		auto state = glfwGetMouseButton(window, static_cast<int32_t>(mousekey));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
		
	}

	glm::vec2 Input::MousePosition()
	{
		auto* window = Application::Get().GetWindow().GETWindow();
		double x;
		double y;
		glfwGetCursorPos(window, &x, &y);
		return {x,y};
	}

}
