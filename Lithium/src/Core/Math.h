#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include <random>

namespace Lithium::Math
{
	bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);
	//converts mouse coordinates to normalized device coordinates("opengl")
	glm::vec3 GetNormalizedDeviceCoords(const glm::vec2& mouse,const glm::vec2& viewportsize);

	glm::vec4 Lerp(const glm::vec4& a, const glm::vec4& b, float t);

	glm::vec3 Lerp(const glm::vec3& a, const glm::vec3& b, float t);
	glm::vec3 QuaternionToEulerAngles(const glm::quat& quaterion);
}

namespace Lithium::Math
{
	class Random
	{
	public:
		static void Init()
		{
			s_RandomEngine.seed(std::random_device()());
		}

		static float Float()
		{
			return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
		}

		static float Int()
		{
			return s_Distribution(s_RandomEngine) / std::numeric_limits<uint32_t>::max();
		}

	private:
		static std::mt19937 s_RandomEngine;
		static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
	};
}
