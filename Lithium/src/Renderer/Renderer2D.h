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
		static void DrawQuad(const glm::mat4& transform, const glm::vec4 color, const Ref<Texture>& texture, int entityID);
		static void DrawQuad(const glm::mat4& transform, const glm::vec4 color, int entityID);
		static void DrawQuadSubTexture(const glm::mat4& transform, const glm::vec4 color,const glm::vec2* coords, const Ref<Texture>& texture, int entityID);
		static void DrawData();
	};
}