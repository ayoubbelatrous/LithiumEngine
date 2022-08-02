#include "lipch.h"
#include "Renderer/PostProcessing.h"
#include "Renderer/Renderer.h"

namespace Lithium
{
	static float QuadVertices[] =
	{
		// Coords    // texCoords
		 1.0f, -1.0f,  1.0f, 0.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		-1.0f,  1.0f,  0.0f, 1.0f,

		 1.0f,  1.0f,  1.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f,  0.0f, 1.0f
	};
	Bloom::Bloom()
	{
		m_PrefilterShader = CreateRef<Shader>("assets/shaders/bloom_prefilter.shader");
		m_DownSampleShader = CreateRef<Shader>("assets/shaders/bloom_downsample.shader");
		m_UpSampleShader = CreateRef<Shader>("assets/shaders/bloom_upsample.shader");

		m_VertexArray = CreateRef<VertexArray>();
		m_VertexBuffer = CreateRef<VertexBuffer>(QuadVertices, sizeof(QuadVertices));
		Ref<VertexBufferLayout> layout = CreateRef<VertexBufferLayout>();
		layout->Push<float>(2);
		layout->Push<float>(2);
		m_VertexArray->AddBuffer(m_VertexBuffer, layout);

		FrameBufferAttachmentDescriptor desc(
			{
				FramebufferTextureFormat::RGBA16F,
			}
		,false);
		for (int i = 0; i < m_DownSamplePasses * 2 + 1; i++)
		{
			m_FrameBuffers.push_back(CreateRef<FrameBuffer>(desc));
		}
	}

	uint32_t Bloom::Update(const glm::vec2 Size, const Ref<FrameBuffer>& MainFrameBuffer)
	{

		m_FrameBuffers[0]->resize(Size.x / 2, Size.y / 2);
		MainFrameBuffer->BindTexture(0, 0);
		m_PrefilterShader->SetUniform1i("u_Input", MainFrameBuffer->GetColorAttachmentID());
		m_PrefilterShader->SetUniform1f("u_Threshold", m_Threshold);
		m_PrefilterShader->Bind();
		m_FrameBuffers[0]->Bind();
		m_VertexArray->Bind();
		m_VertexBuffer->Bind();
		RendererCommand::Draw(6);

		for (int i = 1; i < m_DownSamplePasses; i++)
		{
			m_FrameBuffers[i]->resize(Size.x / 2 * i, Size.y / 2 * i);
			MainFrameBuffer->BindTexture(0, 0);

			m_PrefilterShader->Bind();
			m_PrefilterShader->SetUniform1i("u_Input", m_FrameBuffers[i-1]->GetColorAttachmentID());

			m_FrameBuffers[i]->Bind();
			m_VertexArray->Bind();
			m_VertexBuffer->Bind();
			RendererCommand::Draw(6);
		}
		m_FrameBuffers[0]->BindTexture(0, 0);
		return m_FrameBuffers[0]->GetColorAttachmentID();
	}

	void Bloom::Prefilter()
	{

	}

	Bloom::~Bloom()
	{

	}
}