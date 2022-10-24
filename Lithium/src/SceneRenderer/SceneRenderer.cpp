#include "lipch.h"
#include "SceneRenderer/SceneRenderer.h"

namespace Lithium::Rendering
{

	SceneRenderer::SceneRenderer()
	{}

	SceneRenderer::~SceneRenderer()
	{}

	void SceneRenderer::Init()
	{
		m_RendererData = CreateRef<Data::SceneRendererData>();
		Init_RT();

		VertexBufferLayout vertexBufferLayout = VertexBufferLayout();

		vertexBufferLayout.Push<float>(3);
		vertexBufferLayout.Push<float>(4);
		vertexBufferLayout.Push<float>(2);

		m_RendererData->vertexBufferLayout = vertexBufferLayout;
	}

	void SceneRenderer::Init_RT()
	{
	}

	void SceneRenderer::OnUpdate()
	{
		OnRender_RT();
	}

	void SceneRenderer::OnRender_RT()
	{
	}

	UUID SceneRenderer::Submit(const MeshResource& meshRecource)
	{
		UUID rendererID = UUID();
		m_RendererData->MeshIDs.push_back(rendererID);

		auto vertexData = meshRecource.GetVertexData();
		auto indexData = meshRecource.GetIndexData();

		auto vertexBuffer = VertexBuffer(vertexData.data(), vertexData.size());
		m_RendererData->VertexBuffers[rendererID] = vertexBuffer;

		auto vertexArray = VertexArray();
		vertexArray.AddBuffer(m_RendererData->VertexBuffers[rendererID], m_RendererData->vertexBufferLayout);

		m_RendererData->VertexArrays[rendererID] = vertexArray;

		auto indexBuffer = IndexBuffer(indexData.size(), vertexData.data());
		m_RendererData->IndexBuffers[rendererID] = indexBuffer;

		return rendererID;
	}
}