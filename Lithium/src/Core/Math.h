#pragma once
#include "glm.hpp"

namespace Lithium::Math
{
	bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);
	//converts mouse coordinates to normalized device coordinates("opengl")
	glm::vec3 GetNormalizedDeviceCoords(const glm::vec2& mouse,const glm::vec2& viewportsize);

}