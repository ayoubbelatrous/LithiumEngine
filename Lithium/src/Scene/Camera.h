#pragma once
#include "glm.hpp"

namespace Lithium
{
	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& projection)
			: _Projection(projection) {}
		const glm::mat4 GetProjection() const
		{
			return _Projection;
		}
		virtual ~Camera() = default;

	private:
		glm::mat4 _Projection;
	};
}