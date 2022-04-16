#pragma once
#include "Font/Font.h"

namespace Lithium
{
	class FontRenderer
	{
	public:
		static void Init();
		static void BeginScene(const glm::mat4& projection);
		static void BeginScene(const glm::mat4& transform, const glm::mat4& projection);
		static void DrawString(const glm::mat4& transform,const std::string& text,const Ref<Font>& font,const glm::vec4& color);
		static void DrawString(const glm::vec2& Position,float Scale,const std::string& text,const Ref<Font>& font, const glm::vec4& color, float Spacing = 0, float LineSpacing = 0);
		static void DrawData();
		static void EndScene();

		static void SetPixelRange(float pxr);
		static float GetPixelRange();
	};
}