#pragma once
#include "Lithium.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

#include "Renderer/FrameBuffer.h"
#include "Renderer/Shader.h"

#include "Renderer/VertexBuffer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/VertexBufferLayout.h"
#include <vector>
#include "Renderer/Texture.h"


namespace Lithium
{

	class RuntimeLayer : public Layer
	{
	public:
		RuntimeLayer()
			:Layer("hi")
		{
		}
		void OnCreate() override;
		void OnUpdate() override;
		void OnDestroy() override;
		void onEvent(Event& e) override;
		void onKeyEvent(KeyEvent& e);
		void RenderImgui();
		void SceneEvent(Event& e);
	private:
		Ref<FrameBuffer> m_MainFrameBuffer;
		Ref<Shader> frameshader;
		Ref<Scene> m_ActiveScene;
		Ref<VertexArray> m_Varray;
		Ref<VertexBuffer> m_VBuffer;
	};
}