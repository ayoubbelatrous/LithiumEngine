#pragma once
#include "InputKeyCodes.h"
#include "glm.hpp"

namespace Lithium
{
	class Input
	{
	public:
		static bool IsKeyPressed(const KeyCode keycode);
		static bool IsMouseKeyPressed(int keycode);
		static glm::vec2 MousePosition();
	};
}