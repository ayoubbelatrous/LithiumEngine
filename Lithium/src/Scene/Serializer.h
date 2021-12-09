#pragma once

#include "Scene.h"

#include "Core/Base.h"

namespace Lithium
{
	class Serializer
	{
	public:
		Serializer() = default;
		Serializer(const Ref<Scene>& scene)
			:_Scene(scene)
		{
			
		}

		void SerializeScene(const std::string& path);
		void DeserializeScene(const std::string& path);
	private:
		Ref<Scene> _Scene;
	};
}