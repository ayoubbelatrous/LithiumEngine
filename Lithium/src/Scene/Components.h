#pragma once
#include <string>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"


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
}
