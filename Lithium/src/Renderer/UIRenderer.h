#pragma once

#include "Texture.h"
#include "Core/Base.h"
#include "Renderer.h"
#include "glm/glm.hpp"


namespace Lithium
{
	
	class UIRenderer
	{
	public:
		static void Init();
		static void BeginScene(const glm::mat4& projection);
		static void DrawQuad(const glm::mat4& transform, const glm::vec4 color, int entityID);
		static void EndScene();
		static void DrawData();
	};
}