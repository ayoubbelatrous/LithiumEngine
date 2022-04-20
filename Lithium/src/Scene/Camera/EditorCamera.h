#pragma once

#include "Renderer/Camera.h"
#include "Core/Application.h"
#include "Event/AppEvents.h"
#include "Event/EventDispatcher.h"
#include <glm/glm.hpp>

namespace Lithium {

	class EditorCamera : public Camera
	{
	public:

		EditorCamera() = default;
	private:
		void CalculateProjection();
		float m_Zoom;
		glm::vec2 m_FocalPoint;

	};

}