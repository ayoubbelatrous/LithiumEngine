#include "lipch.h"
#include "Core/Base.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/UIRenderer.h"
#include "Renderer/Renderer.h"
#include <array>
#include "glm/gtc/matrix_transform.hpp"

namespace Lithium
{
	struct UIVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TextureCoords;
		float TextureIndex;
		float EntityID;
	};
	struct UIRendererData
	{
		static const uint32_t MaxQuads = 5000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 18;

		Ref<Shader> StandardShader;
		glm::mat4 Projection;
		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<IndexBuffer> QuadIndexBuffer;
		UIVertex* QuadVertexBufferBase = nullptr;
		UIVertex* QuadVertexBufferPtr = nullptr;
		std::vector<Ref<Texture>> Textures;
		Ref<Texture> WhiteTexture;
		uint32_t textureIndex;
		uint32_t IndexCount;
		glm::vec4 VertexPositions[4];
	};

	static UIRendererData s_UIData;

	void UIRenderer::Init()
	{
		s_UIData.Textures.reserve(s_UIData.MaxTextureSlots);
		s_UIData.QuadVertexArray = CreateRef<VertexArray>();
		s_UIData.QuadVertexBuffer = CreateRef<VertexBuffer>(s_UIData.MaxVertices * sizeof(UIVertex));
		Ref<VertexBufferLayout> pLayout = CreateRef<VertexBufferLayout>();
		pLayout->Push<float>(3);
		pLayout->Push<float>(4);
		pLayout->Push<float>(2);
		pLayout->Push<float>(1);
		pLayout->Push<float>(1);
		s_UIData.QuadVertexArray->AddBuffer(s_UIData.QuadVertexBuffer, pLayout);

		s_UIData.QuadVertexBufferBase = new UIVertex[s_UIData.MaxVertices];

		uint32_t* Indices = new uint32_t[s_UIData.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_UIData.MaxIndices; i += 6)
		{
			Indices[i + 0] = offset + 0;
			Indices[i + 1] = offset + 1;
			Indices[i + 2] = offset + 2;
			Indices[i + 3] = offset + 2;
			Indices[i + 4] = offset + 3;
			Indices[i + 5] = offset + 0;
			offset += 4;
		}
		s_UIData.QuadIndexBuffer = CreateRef<IndexBuffer>(s_UIData.MaxIndices, Indices);

		delete[]Indices;

		s_UIData.WhiteTexture = CreateRef<Texture>(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_UIData.WhiteTexture->SetData(&whiteTextureData);
		s_UIData.Textures.push_back(s_UIData.WhiteTexture);
		s_UIData.VertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_UIData.VertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_UIData.VertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		s_UIData.VertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
		s_UIData.Projection = glm::mat4(0);
		s_UIData.StandardShader = CreateRef<Shader>("assets/shaders/uishader.shader");
	}

	void UIRenderer::BeginScene(const glm::mat4& projection)
	{
		s_UIData.QuadVertexBufferPtr = s_UIData.QuadVertexBufferBase;
		s_UIData.IndexCount = 0;
		s_UIData.textureIndex = 0;
		s_UIData.Projection = projection;
	}

	void UIRenderer::DrawQuad(const glm::mat4& transform, const glm::vec4 color, int entityID)
	{
		LT_PROFILE_FUNCTION("Renderer Draw Quad ScreenSpace");
		constexpr int VertexCount = 4;

		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		float textureIndex = 0.0f;
		for (size_t i = 0; i < VertexCount; i++)
		{
			s_UIData.QuadVertexBufferPtr->Position = transform * s_UIData.VertexPositions[i];
			s_UIData.QuadVertexBufferPtr->Color = color;
			s_UIData.QuadVertexBufferPtr->TextureCoords = textureCoords[i];
			s_UIData.QuadVertexBufferPtr->TextureIndex = textureIndex;
			s_UIData.QuadVertexBufferPtr->EntityID = (float)entityID;
			s_UIData.QuadVertexBufferPtr++;
		}

		s_UIData.IndexCount += 6;
	}

	void UIRenderer::EndScene()
	{
		DrawData();
	}

	void UIRenderer::DrawData()
	{
		if (s_UIData.IndexCount)
		{
			LT_PROFILE_FUNCTION("UIRenderer Draw Data");
			uint32_t dataSize = (uint32_t)((uint8_t*)s_UIData.QuadVertexBufferPtr - (uint8_t*)s_UIData.QuadVertexBufferBase);



			int32_t samplers[UIRendererData::MaxTextureSlots];
			for (uint32_t i = 0; i < UIRendererData::MaxTextureSlots; i++)
				samplers[i] = i;
			for (uint32_t i = 0; i < s_UIData.textureIndex; i++)
				s_UIData.Textures[i]->Bind(i);

			s_UIData.QuadVertexArray->Bind();
			s_UIData.QuadVertexBuffer->Bind();
			s_UIData.QuadVertexBuffer->SetData(s_UIData.QuadVertexBufferBase, dataSize);
			s_UIData.StandardShader->Bind();
			s_UIData.StandardShader->SetUniformMat4f("u_projection", s_UIData.Projection);

			s_UIData.StandardShader->SetUniformiv("u_textures", samplers);

			s_UIData.QuadIndexBuffer->Bind();
			RendererCommand::DrawIndexed(s_UIData.IndexCount);
		}

	}

}