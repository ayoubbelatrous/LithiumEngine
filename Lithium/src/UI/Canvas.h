#pragma once
#include "glm/glm.hpp"
#include "Core/UUID.h"

namespace Lithium
{
	class Canvas
	{
	public:

	private:
		bool m_ScreenSpace = true;
		std::vector<UUID> m_Children;
		glm::vec2 m_Size;
	};
}