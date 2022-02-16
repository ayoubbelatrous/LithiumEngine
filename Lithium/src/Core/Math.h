#pragma once
#include "glm.hpp"
#include <random>

namespace Lithium::Math
{
	bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);
	//converts mouse coordinates to normalized device coordinates("opengl")
	glm::vec3 GetNormalizedDeviceCoords(const glm::vec2& mouse,const glm::vec2& viewportsize);

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
