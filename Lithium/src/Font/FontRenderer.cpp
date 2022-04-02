#include "lipch.h"

#include "Renderer/Renderer.h"
#include "FontRenderer.h"
#include "Core/Base.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"


#include "gtc/matrix_transform.hpp"

#include <array>

namespace Lithium
{
	struct FontVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TextureCoords;
		float TextureIndex;
		float EntityID;
	};


	struct FontRendererData
	{
		const static uint32_t MaxQuadCount = 10000;
		const static uint32_t MaxVertices = MaxQuadCount * 4;
		const static uint32_t MaxIndices = MaxQuadCount * 6;
		const static uint32_t MaxTextures = 18;

		Ref<VertexArray> FontVertexArray;
		Ref<VertexBuffer> FontVertexBuffer;
		Ref<IndexBuffer> FontIndexBuffer;
		std::array<Ref<Texture>, MaxTextures> TextureSlots;
		uint32_t TextureSlotIndex = 0;
		Ref<Shader> shader;
		glm::vec4 VertexPositions[4];
		uint32_t IndexCount = 0;
		FontVertex* VertexBufferPtr = nullptr;
		FontVertex* VertexBufferBase = nullptr;
		glm::mat4 projection;

	};
	FontRendererData s_Data;

	void FontRenderer::Init()
	{
		LT_PROFILE_FUNCTION("Renderer Init");
		s_Data.FontVertexArray = CreateRef<VertexArray>();
		s_Data.FontVertexBuffer = CreateRef<VertexBuffer>(s_Data.MaxVertices * sizeof(FontVertex));

		Ref<VertexBufferLayout> Layout = CreateRef<VertexBufferLayout>();
		Layout->Push<float>(3);
		Layout->Push<float>(4);
		Layout->Push<float>(2);
		Layout->Push<float>(1);
		Layout->Push<float>(1);
		s_Data.FontVertexArray->AddBuffer(s_Data.FontVertexBuffer, Layout);
		s_Data.shader = CreateRef<Shader>("assets/shaders/font.shader");
		s_Data.VertexBufferBase = new FontVertex[s_Data.MaxVertices];


		
		uint32_t* Indices = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			Indices[i + 0] = offset + 0;
			Indices[i + 1] = offset + 1;
			Indices[i + 2] = offset + 2;
			Indices[i + 3] = offset + 2;
			Indices[i + 4] = offset + 3;
			Indices[i + 5] = offset + 0;
			offset += 4;
		}
		s_Data.FontIndexBuffer = CreateRef<IndexBuffer>(s_Data.MaxIndices, Indices);

		delete[]Indices;

		s_Data.VertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.VertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.VertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.VertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void FontRenderer::BeginScene(const glm::mat4& projection)
	{
		s_Data.projection = projection;
		s_Data.IndexCount = 0;
		s_Data.TextureSlotIndex = 0;
		s_Data.VertexBufferPtr = s_Data.VertexBufferBase;
	}

	void FontRenderer::DrawString(const glm::mat4& transform,const std::string& text, const Ref<Font>& font)
	{

		constexpr int VertexCount = 4;

		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		float textureIndex = 0.0f;
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i] == *font->GetAtlas())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = font->GetAtlas();
			s_Data.TextureSlotIndex++;
		}


		for (size_t i = 0; i < VertexCount; i++)
		{
			s_Data.VertexBufferPtr->Position = transform * s_Data.VertexPositions[i];
			s_Data.VertexBufferPtr->Color = glm::vec4(1.0f);
			s_Data.VertexBufferPtr->TextureCoords = textureCoords[i];
			s_Data.VertexBufferPtr->TextureIndex = textureIndex;
			s_Data.VertexBufferPtr->EntityID = -1;
			s_Data.VertexBufferPtr++;
		}

		s_Data.IndexCount += 6;
	}

	void FontRenderer::DrawData()
	{

		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.VertexBufferPtr - (uint8_t*)s_Data.VertexBufferBase);



		int32_t samplers[FontRendererData::MaxTextures];
		for (uint32_t i = 0; i < FontRendererData::MaxTextures; i++)
			samplers[i] = i;
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			s_Data.TextureSlots[i]->Bind(i);

		s_Data.FontVertexArray->Bind();
		s_Data.FontVertexBuffer->Bind();
		s_Data.FontVertexBuffer->SetData(s_Data.VertexBufferBase, dataSize);
		s_Data.shader->Bind();
		s_Data.shader->SetUniformMat4f("u_projection", s_Data.projection);

		s_Data.shader->SetUniformiv("u_textures", samplers);
		s_Data.shader->SetUniform1f("pxRange", 2.0f);

		s_Data.FontIndexBuffer->Bind();
		RendererCommand::DrawIndexed(s_Data.IndexCount);
	}

	void FontRenderer::EndScene()
	{
		DrawData();
	}

}