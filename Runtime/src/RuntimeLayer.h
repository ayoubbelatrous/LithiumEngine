#pragma once
#include "Lithium.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

#include "Renderer/FrameBuffer.h"
#include "Renderer/Shader.h"

#include "Renderer/VertexBuffer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/VertexBufferLayout.h"
#include "Mesh/Mesh.h"
#include <vector>
#include "Renderer/Texture.h"


namespace Lithium
{

	class RuntimeLayer : public Lithium::Layer
	{
	public:
		RuntimeLayer()
			:Layer("hi")
		{
			//sz = Serializer(_MainScene);
		}
		void OnCreate() override;
		void OnUpdate() override;
		void OnDestroy() override;
		void onEvent(Event& e) override;
		void onKeyEvent(KeyEvent& e);
		void RenderImgui();
		void SceneEvent(Event& e);
	private:
		glm::mat4 proj;
		glm::mat4 view;
		glm::mat4 model;
		glm::vec3 pos;
		Ref<FrameBuffer> _framebuffer;
		Ref<Shader> shader;
		Ref<Shader> frameshader;
		std::vector<Ref<Mesh>> meshs;
		int rot;
		Ref<VertexArray> _vertexarray;
		Ref<VertexBuffer> _vertexbuffer;
		glm::vec3 LightPos;
		Ref<Texture> uvTEST;
	
	};
}