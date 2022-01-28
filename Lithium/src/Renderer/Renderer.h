#pragma once
#include "glm.hpp"

namespace Lithium
{
	class RendererCommand
	{
	public:
		static void Clear();
		static void ClearColor(const glm::vec4& color);
		static void DrawIndexed(uint32_t count);
		static void Draw(uint32_t count);
	};
}
