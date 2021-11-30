#pragma once
#include "glm.hpp"
#include "Texture.h"
namespace Lithium
{
	class Renderer2D
	{
	public:
		static void Init();
		static void StartBatch();
		static void NextBatch();
		static void BeginScene(glm::mat4 proj, glm::mat4 view);
		static void EndScene();
		static void Flush();
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture>& texture);
	};
}