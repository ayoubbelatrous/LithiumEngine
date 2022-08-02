#pragma once
#include "Renderer/FrameBuffer.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/VertexArray.h"

namespace Lithium
{
	class Bloom
	{
	public:
		Bloom();
		~Bloom();
		uint32_t Update(const glm::vec2 Size, const Ref<FrameBuffer>& framebuffer);
	private:
		void Prefilter();
		float m_Threshold = 1.0f;
		float m_Intensity = 1.5f;
		int m_DownSamplePasses = 3;
		Ref<Shader> m_PrefilterShader;
		Ref<Shader> m_DownSampleShader;
		Ref<Shader> m_UpSampleShader;
		Ref<VertexArray> m_VertexArray;
		Ref<VertexBuffer> m_VertexBuffer;
		std::vector<Ref<FrameBuffer>> m_FrameBuffers;
	};
}