#pragma once
#include "AssetManager/Asset.h"
#include "Renderer/Texture.h"
#include "Core/Base.h"
namespace Lithium
{
	class AnimationPanel
	{
	public:

		void OnCreate();
		void OnUpdate(bool* open);

		Asset CurrentAnimation;
	private:
		void DrawTimeLine();
		int m_TotalTime = 10;
		float m_ScrollBarMin = 0.0f, m_ScrollBarMax = 1.0f;
		Ref<Texture> m_KeyFrameIcon;
	};
}