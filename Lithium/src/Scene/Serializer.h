#pragma once

#include "Scene.h"

#include "Core/Base.h"

namespace Lithium
{
	class Serializer
	{
	public:
		void SerializeScene(const Ref<Scene>& scene, const std::string& path);
	};
}