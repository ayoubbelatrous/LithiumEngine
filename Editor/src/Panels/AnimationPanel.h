#pragma once
#include "AssetManager/Asset.h"

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
		int TimeLineScale = 5;
	};
}