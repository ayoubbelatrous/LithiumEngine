#include "lipch.h"
#include "Renderer.h"
#include "glad/glad.h"
#include "Core/Base.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Renderer2D.h"
#include "Renderer.h"
#include <array>
#include "glm/gtc/matrix_transform.hpp"
#include "Scene/Components.h"

namespace Lithium
{
	struct CircleVertex
	{
		glm::vec3 WorldPosition;
		glm::vec3 LocalPosition;
		glm::vec4 Color;
		float Thickness;
		float Fade;
		float EntityID;
	};
	struct LineVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
	};
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TextureCoords;
		float TextureIndex;
		float EntityID;
	};


	struct RendererData
	{
		const static uint32_t MaxQuadCount;
		const static uint32_t MaxVertices;
		const static uint32_t MaxIndices;
		const static uint32_t MaxTextures = 18;

		const static uint32_t MaxLineCount = 10000;
		const static uint32_t MaxCircleCount;
		const static uint32_t MaxLineVertices;
		const static uint32_t MaxCircleVertices;
		const static uint32_t MaxCircleIndices;
		Ref<VertexArray> _VertexArray;
		Ref<VertexBuffer> _VertexBuffer;
		Ref<IndexBuffer> _IndexBuffer;
		Ref<Texture> whitetexture;
		std::array<Ref<Texture>, MaxTextures> TextureSlots;
		uint32_t TextureSlotIndex = 1;
		Ref<Shader> _shader;
		glm::vec4 _VertexPositions[4];
		uint32_t _IndexCount = 0;
		Vertex* _VertexBufferPtr = nullptr;
		Vertex* _VertexBufferBase = nullptr;
		glm::mat4 projection;
		//debug lines
		float LineThickness = 1.0f;
		Ref<VertexArray> LineVertexArray;
		Ref<VertexBuffer> LineVertexBuffer;
		Ref<Shader> LineShader;
		uint32_t LineVertexCount = 0;
		LineVertex* LineVertexBufferBase = nullptr;
		LineVertex* LineVertexBufferPtr = nullptr;

		Ref<VertexArray> CircleVertexArray;
		Ref<VertexBuffer> CircleVertexBuffer;
		Ref<Shader> CircleShader;
		uint32_t CircleVertexCount = 0;
		CircleVertex* CircleVertexBufferBase = nullptr;
		CircleVertex* CircleVertexBufferPtr = nullptr;
		uint32_t CircleIndexCount = 0;

	};
    const  uint32_t RendererData::MaxQuadCount = 10000;
    const  uint32_t RendererData::MaxCircleCount = 10000;
	const  uint32_t RendererData::MaxVertices = RendererData::MaxQuadCount * 4;
	const  uint32_t RendererData::MaxIndices = RendererData::MaxQuadCount * 12;
	const  uint32_t RendererData::MaxLineVertices = RendererData::MaxLineCount * 2;
	const  uint32_t RendererData::MaxCircleVertices = RendererData::MaxCircleCount * 4;
	const  uint32_t RendererData::MaxCircleIndices = RendererData::MaxCircleCount * 6;

	static RendererData s_Data;


	void BatchRenderer::Init()
	{
		LT_PROFILE_FUNCTION("Renderer Init");
		s_Data._VertexArray = CreateRef<VertexArray>();
		s_Data._VertexBuffer = CreateRef<VertexBuffer>(s_Data.MaxVertices * sizeof(Vertex));
		
		Ref<VertexBufferLayout> Layout = CreateRef<VertexBufferLayout>();
		Layout->Push<float>(3);
		Layout->Push<float>(4);
		Layout->Push<float>(2);
		Layout->Push<float>(1);
		Layout->Push<float>(1);
		s_Data._VertexArray->AddBuffer(s_Data._VertexBuffer, Layout);
		s_Data._shader = CreateRef<Shader>("assets/shaders/main.shader");
		s_Data._VertexBufferBase = new Vertex[s_Data.MaxVertices];


		s_Data.LineVertexArray = CreateRef<VertexArray>();
		s_Data.LineVertexBuffer = CreateRef<VertexBuffer>(s_Data.MaxLineVertices * sizeof(LineVertex));
		s_Data.CircleVertexArray = CreateRef<VertexArray>();
		s_Data.CircleVertexBuffer = CreateRef<VertexBuffer>(s_Data.MaxVertices * sizeof(CircleVertex));

		Ref<VertexBufferLayout> LineVertexLayout = CreateRef<VertexBufferLayout>();
		LineVertexLayout->Push<float>(3);
		LineVertexLayout->Push<float>(4);
		s_Data.LineVertexArray->AddBuffer(s_Data.LineVertexBuffer, LineVertexLayout);
		s_Data.LineShader = CreateRef<Shader>("assets/shaders/line.shader");
		s_Data.LineVertexBufferBase = new LineVertex[s_Data.MaxLineVertices];

		Ref<VertexBufferLayout> CircleVertexLayout = CreateRef<VertexBufferLayout>();
		CircleVertexLayout->Push<float>(3);
		CircleVertexLayout->Push<float>(3);
		CircleVertexLayout->Push<float>(4);
		CircleVertexLayout->Push<float>(1);
		CircleVertexLayout->Push<float>(1);
		CircleVertexLayout->Push<float>(1);
		s_Data.CircleVertexArray->AddBuffer(s_Data.CircleVertexBuffer, CircleVertexLayout);
		s_Data.CircleShader = CreateRef<Shader>("assets/shaders/circle.shader");
		s_Data.CircleVertexBufferBase = new CircleVertex[s_Data.MaxVertices];

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
		s_Data._IndexBuffer = CreateRef<IndexBuffer>(s_Data.MaxIndices,Indices);

		delete[]Indices;

		s_Data.whitetexture = CreateRef<Texture>(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.whitetexture->SetData(&whiteTextureData);
		s_Data.TextureSlots[0] = s_Data.whitetexture;
		s_Data._VertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data._VertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_Data._VertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		s_Data._VertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.projection = glm::mat4(0); 
	}

	void BatchRenderer::Begin(const glm::mat4& transform, const glm::mat4& projection)
	{

		s_Data.projection = projection * glm::inverse(transform);
		s_Data._IndexCount = 0;
		s_Data.TextureSlotIndex = 1;
		s_Data._VertexBufferPtr = s_Data._VertexBufferBase;

		s_Data.LineVertexCount = 0;
		s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;

		s_Data.CircleVertexCount = 0;
		s_Data.CircleIndexCount = 0;
		s_Data.CircleVertexBufferPtr = s_Data.CircleVertexBufferBase;
	}

	void BatchRenderer::End()
	{
		DrawData();
	}

	void BatchRenderer::DrawQuad(const glm::mat4& transform, const glm::vec4 color, const Ref<Texture>& texture, int entityID)
	{
		LT_PROFILE_FUNCTION("Renderer Draw Quad");
		constexpr int VertexCount = 4;
		
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i] == *texture)
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}


		for (size_t i = 0; i < VertexCount; i++)
		{
			s_Data._VertexBufferPtr->Position = transform * s_Data._VertexPositions[i];
			s_Data._VertexBufferPtr->Color = color;
			s_Data._VertexBufferPtr->TextureCoords = textureCoords[i];
			s_Data._VertexBufferPtr->TextureIndex = textureIndex;
			s_Data._VertexBufferPtr->EntityID = (float)entityID;
			s_Data._VertexBufferPtr++;
		}

		s_Data._IndexCount += 6;
	}
	void BatchRenderer::DrawQuad(const glm::mat4& transform, const glm::vec4 color,int entityID)
	{
		LT_PROFILE_FUNCTION("Renderer Draw Quad");
		constexpr int VertexCount = 4;

		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		float textureIndex = 0.0f;
		for (size_t i = 0; i < VertexCount; i++)
		{
			s_Data._VertexBufferPtr->Position = transform * s_Data._VertexPositions[i];
			s_Data._VertexBufferPtr->Color = color;
			s_Data._VertexBufferPtr->TextureCoords = textureCoords[i];
			s_Data._VertexBufferPtr->TextureIndex = textureIndex;
			s_Data._VertexBufferPtr->EntityID = (float)entityID;
			s_Data._VertexBufferPtr++;
		}

		s_Data._IndexCount += 6;
	}

	void BatchRenderer::DrawQuadSubTexture(const glm::mat4& transform, const glm::vec4 color, const glm::vec2 coords[], const Ref<Texture>& texture, int entityID)
	{
		constexpr int VertexCount = 4;


		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i] == *texture)
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}


		for (size_t i = 0; i < VertexCount; i++)
		{
			s_Data._VertexBufferPtr->Position = transform * s_Data._VertexPositions[i];
			s_Data._VertexBufferPtr->Color = color;
			s_Data._VertexBufferPtr->TextureCoords = coords[i];
			s_Data._VertexBufferPtr->TextureIndex = textureIndex;
			s_Data._VertexBufferPtr->EntityID = (float)entityID;
			s_Data._VertexBufferPtr++;
		}

		s_Data._IndexCount += 6;
	}


	void BatchRenderer::DrawLine(const glm::vec3& p0,const glm::vec3& p1, const glm::vec4& color)
	{
		s_Data.LineVertexBufferPtr->Position = p0;
		s_Data.LineVertexBufferPtr->Color = color;
		s_Data.LineVertexBufferPtr++;

		s_Data.LineVertexBufferPtr->Position = p1;
		s_Data.LineVertexBufferPtr->Color = color;
		s_Data.LineVertexBufferPtr++;

		s_Data.LineVertexCount += 2;
	}

	void BatchRenderer::DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		glm::vec3 p0 = glm::vec3(position.x - size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p1 = glm::vec3(position.x + size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p2 = glm::vec3(position.x + size.x * 0.5f, position.y + size.y * 0.5f, position.z);
		glm::vec3 p3 = glm::vec3(position.x - size.x * 0.5f, position.y + size.y * 0.5f, position.z);

		DrawLine(p0, p1, color);
		DrawLine(p1, p2, color);
		DrawLine(p2, p3, color);
		DrawLine(p3, p0, color);
	}

	void BatchRenderer::DrawRect(const glm::mat4& transform, const glm::vec4& color)
	{
		glm::vec3 lineVertices[4];
		for (size_t i = 0; i < 4; i++)
			lineVertices[i] = transform * s_Data._VertexPositions[i];

		DrawLine(lineVertices[0], lineVertices[1], color);
		DrawLine(lineVertices[1], lineVertices[2], color);
		DrawLine(lineVertices[2], lineVertices[3], color);
		DrawLine(lineVertices[3], lineVertices[0], color);
	}

	void BatchRenderer::DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness /*= 1.0f*/, float fade /*= 0.005f*/, int entityID /*= -1*/)
	{
		for (size_t i = 0; i < 4; i++)
		{
			s_Data.CircleVertexBufferPtr->WorldPosition = transform * s_Data._VertexPositions[i];
			s_Data.CircleVertexBufferPtr->LocalPosition = s_Data._VertexPositions[i] * 2.0f;
			s_Data.CircleVertexBufferPtr->Color = color;
			s_Data.CircleVertexBufferPtr->Thickness = thickness;
			s_Data.CircleVertexBufferPtr->Fade = fade;
			s_Data.CircleVertexBufferPtr->EntityID = (float)entityID;
			s_Data.CircleVertexBufferPtr++;
		}

		s_Data.CircleIndexCount += 6;

	}

	void BatchRenderer::DrawData()
	{
		if(s_Data._IndexCount)
		{
			LT_PROFILE_FUNCTION("Renderer Draw Data");
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data._VertexBufferPtr - (uint8_t*)s_Data._VertexBufferBase);



			int32_t samplers[RendererData::MaxTextures];
			for (uint32_t i = 0; i < RendererData::MaxTextures; i++)
				samplers[i] = i;
			for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
				s_Data.TextureSlots[i]->Bind(i);

			s_Data._VertexArray->Bind();
			s_Data._VertexBuffer->Bind();
			s_Data._VertexBuffer->SetData(s_Data._VertexBufferBase, dataSize);
			s_Data._shader->Bind();
			s_Data._shader->SetUniformMat4f("u_projection", s_Data.projection);

			s_Data._shader->SetUniformiv("u_textures", samplers);

			s_Data._IndexBuffer->Bind();
			RendererCommand::DrawIndexed(s_Data._IndexCount);
		}
		

		if (s_Data.LineVertexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.LineVertexBufferPtr - (uint8_t*)s_Data.LineVertexBufferBase);

			s_Data._IndexBuffer->UnBind();
			s_Data.LineVertexArray->Bind();
			s_Data.LineVertexBuffer->Bind();
			s_Data.LineVertexBuffer->SetData(s_Data.LineVertexBufferBase, dataSize);
			s_Data.LineShader->Bind();
			s_Data.LineShader->SetUniformMat4f("u_projection", s_Data.projection);
			RendererCommand::SetLineWidth(s_Data.LineThickness);
			RendererCommand::DrawLines(s_Data.LineVertexCount);
		}

		if (s_Data.CircleIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.CircleVertexBufferPtr - (uint8_t*)s_Data.CircleVertexBufferBase);
			s_Data.CircleVertexBuffer->SetData(s_Data.CircleVertexBufferBase, dataSize);

			s_Data.CircleVertexArray->Bind();
			s_Data.CircleVertexBuffer->Bind();
			s_Data._IndexBuffer->Bind();
			s_Data.CircleShader->Bind();
			s_Data.CircleShader->SetUniformMat4f("u_projection", s_Data.projection);
			RendererCommand::DrawIndexed(s_Data.CircleIndexCount);
		}
		
	}
}