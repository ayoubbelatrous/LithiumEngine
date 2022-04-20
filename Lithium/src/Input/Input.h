#pragma once
#include "InputKeyCodes.h"
#include "glm/glm.hpp"

namespace Lithium
{
	class Input
	{
	public:
		static bool IsKeyPressed(const KeyCode keycode);
		static bool IsKeyDown(const KeyCode keycode);
		static bool IsMouseKeyPressed(int keycode);
		static bool IsMouseKeyDown(int keycode);
		static glm::vec2 MousePosition();
	};
}