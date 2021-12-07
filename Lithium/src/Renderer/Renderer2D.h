#pragma once

#include "Texture.h"
#include "Core/Base.h"
#include "Renderer.h"
#include "glm.hpp"

namespace Lithium {

	class BatchRenderer
	{
	public:
		static void Init();
		static void Begin(const glm::mat4& transform, const glm::mat4& projection);
		static void End();
		static void DrawQuad(const glm::mat4& transform, const glm::vec4 color, const Ref<Texture>& texture);
		static void DrawQuad(const glm::mat4& transform, const glm::vec4 color);
		static void DrawData();
	};
}