#pragma once
#include "Font/Font.h"

namespace Lithium
{
	class FontRenderer
	{
	public:
		static void Init();
		static void BeginScene(const glm::mat4& projection);
		static void DrawString(const glm::mat4& transform,const std::string& text,const Ref<Font>& font);
		static void DrawData();
		static void EndScene();
	};
}