#pragma once
#include "Lithium.h"
#include "Core/Base.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "Renderer/VertexArray.h"
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
		Ref<VertexBuffer> vb;
		Ref<VertexArray> vao;
		Ref<IndexBuffer> ibo;
		Ref<VertexBufferLayout> layout;
		Ref<Shader> shader;
	};

}
