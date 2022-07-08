#pragma once

#include "Texture.h"
#include "Core/Base.h"
#include "Renderer.h"
#include "glm/glm.hpp"

namespace Lithium {

	class BatchRenderer
	{
	public:
		static void Init();
		static void Begin(const glm::mat4& transform, const glm::mat4& projection);
		static void End();
		static void DrawQuad(const glm::mat4& transform, const glm::vec4 color, const Ref<Texture>& texture, int entityID);
		static void DrawQuad(const glm::mat4& transform, const glm::vec4 color, int entityID);
		static void DrawQuadSubTexture(const glm::mat4& transform, const glm::vec4 color,const glm::vec2 coords[], const Ref<Texture>& texture, int entityID);

		static void DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color);
		static void DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawRect(const glm::mat4& transform, const glm::vec4& color);
		static void DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness = 1.0f, float fade = 0.005f, int entityID = -1);


		static void DrawData();
	};
}