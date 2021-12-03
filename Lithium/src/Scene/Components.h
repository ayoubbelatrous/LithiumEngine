#pragma once
#include <string>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "Renderer/Texture.h"
#include "Core/Base.h"

namespace Lithium
{
	struct NameComponent
	{
		std::string Name = "New Entity";
		NameComponent() = default;
		NameComponent(const std::string& name)
			:Name(name) {}
		NameComponent(const NameComponent& other) = default;
		std::string& GetName()
		{
			return Name;
		}
	};

	struct TransformComponent
	{

		glm::vec3 Position;
		glm::vec3 Rotation;
		glm::vec3 Scale;	

		TransformComponent() = default;
		TransformComponent(const TransformComponent& other) = default;

		glm::mat4 GetMatrix()
		{
			glm::mat4 transform;
			transform = glm::translate(glm::mat4(1),Position);
			return transform;
		}
	};


	struct SpriteRendererComponent
	{
		glm::vec4 Color;
		Ref<Texture> tex;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const glm::vec4 color)
			:Color(color) {
			tex = CreateRef<Texture>();
		}
		SpriteRendererComponent(const SpriteRendererComponent& other) = default;
		glm::vec4 GetColor() const { return Color; }
		Ref<Texture> GetTexture() const { return tex; }
	};
}
