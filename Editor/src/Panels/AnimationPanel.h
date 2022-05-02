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
		int TimeLineScale = 5;
	};
}