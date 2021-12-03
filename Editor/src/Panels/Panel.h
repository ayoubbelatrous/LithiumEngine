#pragma once
#include "Scene/Scene.h"
#include "Core/Base.h"

namespace Lithium
{
	class Panel
	{
	public:
		virtual void onCreate() {};
		virtual void onUpdate() {};
		virtual void SetScene(Ref<Scene> scene) {};
	};
}