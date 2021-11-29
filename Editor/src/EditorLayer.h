#pragma once
#include "Lithium.h"
#include "Core/Base.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "Renderer/Shader.h"

namespace Lithium
{
	class EditorLayer : public Lithium::Layer
	{
	public:
		EditorLayer()
			:Layer("hi")
		{
		}
		void OnCreate() override;
		void OnUpdate() override;
		void OnDestroy() override;

	private:
		Ref<Texture> tex;
		Ref<Shader> shader;
		glm::vec3 pos;
		glm::vec4 color;
		glm::mat4 quad;
		glm::mat4 projection;
		glm::mat4 view;
	};

}
