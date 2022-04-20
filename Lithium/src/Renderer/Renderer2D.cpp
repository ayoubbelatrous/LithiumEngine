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
		const static uint32_t MaxQuadCount = 10000;
		const static uint32_t MaxVertices = MaxQuadCount * 4;
		const static uint32_t MaxIndices = MaxQuadCount * 6;
		const static uint32_t MaxTextures = 18;

		const static uint32_t MaxLineCount = 10000;
		const static uint32_t MaxLineVertices = MaxLineCount * 2;

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
	};

	static RendererData _Data;


	void BatchRenderer::Init()
	{
		LT_PROFILE_FUNCTION("Renderer Init");
		_Data._VertexArray = CreateRef<VertexArray>();
		_Data._VertexBuffer = CreateRef<VertexBuffer>(_Data.MaxVertices * sizeof(Vertex));
		
		Ref<VertexBufferLayout> Layout = CreateRef<VertexBufferLayout>();
		Layout->Push<float>(3);
		Layout->Push<float>(4);
		Layout->Push<float>(2);
		Layout->Push<float>(1);
		Layout->Push<float>(1);
		_Data._VertexArray->AddBuffer(_Data._VertexBuffer, Layout);
		_Data._shader = CreateRef<Shader>("assets/shaders/main.shader");
		_Data._VertexBufferBase = new Vertex[_Data.MaxVertices];


		_Data.LineVertexArray = CreateRef<VertexArray>();
		_Data.LineVertexBuffer = CreateRef<VertexBuffer>(_Data.MaxLineVertices * sizeof(LineVertex));

		Ref<VertexBufferLayout> LineVertexLayout = CreateRef<VertexBufferLayout>();
		LineVertexLayout->Push<float>(3);
		LineVertexLayout->Push<float>(4);
		_Data.LineVertexArray->AddBuffer(_Data.LineVertexBuffer, LineVertexLayout);
		_Data.LineShader = CreateRef<Shader>("assets/shaders/line.shader");
		_Data.LineVertexBufferBase = new LineVertex[_Data.MaxLineVertices];



		uint32_t* Indices = new uint32_t[_Data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < _Data.MaxIndices; i += 6)
		{
			Indices[i + 0] = offset + 0;
			Indices[i + 1] = offset + 1;
			Indices[i + 2] = offset + 2;
			Indices[i + 3] = offset + 2;
			Indices[i + 4] = offset + 3;
			Indices[i + 5] = offset + 0;
			offset += 4;
		}
		_Data._IndexBuffer = CreateRef<IndexBuffer>(_Data.MaxIndices,Indices);

		delete[]Indices;

		_Data.whitetexture = CreateRef<Texture>(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		_Data.whitetexture->SetData(&whiteTextureData);
		_Data.TextureSlots[0] = _Data.whitetexture;
		_Data._VertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		_Data._VertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		_Data._VertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		_Data._VertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
		_Data.projection = glm::mat4(0); 
	}

	void BatchRenderer::Begin(const glm::mat4& transform, const glm::mat4& projection)
	{

		_Data.projection = projection * glm::inverse(transform);
		_Data._IndexCount = 0;
		_Data.TextureSlotIndex = 1;
		_Data._VertexBufferPtr = _Data._VertexBufferBase;

		_Data.LineVertexCount = 0;
		_Data.LineVertexBufferPtr = _Data.LineVertexBufferBase;

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
		for (uint32_t i = 1; i < _Data.TextureSlotIndex; i++)
		{
			if (*_Data.TextureSlots[i] == *texture)
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)_Data.TextureSlotIndex;
			_Data.TextureSlots[_Data.TextureSlotIndex] = texture;
			_Data.TextureSlotIndex++;
		}


		for (size_t i = 0; i < VertexCount; i++)
		{
			_Data._VertexBufferPtr->Position = transform * _Data._VertexPositions[i];
			_Data._VertexBufferPtr->Color = color;
			_Data._VertexBufferPtr->TextureCoords = textureCoords[i];
			_Data._VertexBufferPtr->TextureIndex = textureIndex;
			_Data._VertexBufferPtr->EntityID = (float)entityID;
			_Data._VertexBufferPtr++;
		}

		_Data._IndexCount += 6;
	}
	void BatchRenderer::DrawQuad(const glm::mat4& transform, const glm::vec4 color,int entityID)
	{
		LT_PROFILE_FUNCTION("Renderer Draw Quad");
		constexpr int VertexCount = 4;

		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		float textureIndex = 0.0f;
		for (size_t i = 0; i < VertexCount; i++)
		{
			_Data._VertexBufferPtr->Position = transform * _Data._VertexPositions[i];
			_Data._VertexBufferPtr->Color = color;
			_Data._VertexBufferPtr->TextureCoords = textureCoords[i];
			_Data._VertexBufferPtr->TextureIndex = textureIndex;
			_Data._VertexBufferPtr->EntityID = (float)entityID;
			_Data._VertexBufferPtr++;
		}

		_Data._IndexCount += 6;
	}

	void BatchRenderer::DrawQuadSubTexture(const glm::mat4& transform, const glm::vec4 color, const glm::vec2 coords[], const Ref<Texture>& texture, int entityID)
	{
		constexpr int VertexCount = 4;


		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < _Data.TextureSlotIndex; i++)
		{
			if (*_Data.TextureSlots[i] == *texture)
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)_Data.TextureSlotIndex;
			_Data.TextureSlots[_Data.TextureSlotIndex] = texture;
			_Data.TextureSlotIndex++;
		}


		for (size_t i = 0; i < VertexCount; i++)
		{
			_Data._VertexBufferPtr->Position = transform * _Data._VertexPositions[i];
			_Data._VertexBufferPtr->Color = color;
			_Data._VertexBufferPtr->TextureCoords = coords[i];
			_Data._VertexBufferPtr->TextureIndex = textureIndex;
			_Data._VertexBufferPtr->EntityID = (float)entityID;
			_Data._VertexBufferPtr++;
		}

		_Data._IndexCount += 6;
	}


	void BatchRenderer::DrawLine(const glm::vec3& p0,const glm::vec3& p1, const glm::vec4& color)
	{
		_Data.LineVertexBufferPtr->Position = p0;
		_Data.LineVertexBufferPtr->Color = color;
		_Data.LineVertexBufferPtr++;

		_Data.LineVertexBufferPtr->Position = p1;
		_Data.LineVertexBufferPtr->Color = color;
		_Data.LineVertexBufferPtr++;

		_Data.LineVertexCount += 2;
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
			lineVertices[i] = transform * _Data._VertexPositions[i];

		DrawLine(lineVertices[0], lineVertices[1], color);
		DrawLine(lineVertices[1], lineVertices[2], color);
		DrawLine(lineVertices[2], lineVertices[3], color);
		DrawLine(lineVertices[3], lineVertices[0], color);
	}

	void BatchRenderer::DrawData()
	{
		if(_Data._IndexCount)
		{
			LT_PROFILE_FUNCTION("Renderer Draw Data");
			uint32_t dataSize = (uint32_t)((uint8_t*)_Data._VertexBufferPtr - (uint8_t*)_Data._VertexBufferBase);



			int32_t samplers[RendererData::MaxTextures];
			for (uint32_t i = 0; i < RendererData::MaxTextures; i++)
				samplers[i] = i;
			for (uint32_t i = 0; i < _Data.TextureSlotIndex; i++)
				_Data.TextureSlots[i]->Bind(i);

			_Data._VertexArray->Bind();
			_Data._VertexBuffer->Bind();
			_Data._VertexBuffer->SetData(_Data._VertexBufferBase, dataSize);
			_Data._shader->Bind();
			_Data._shader->SetUniformMat4f("u_projection", _Data.projection);

			_Data._shader->SetUniformiv("u_textures", samplers);

			_Data._IndexBuffer->Bind();
			RendererCommand::DrawIndexed(_Data._IndexCount);
		}
		

		if (_Data.LineVertexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)_Data.LineVertexBufferPtr - (uint8_t*)_Data.LineVertexBufferBase);

			_Data.LineVertexArray->Bind();
			_Data.LineVertexBuffer->Bind();
			_Data.LineVertexBuffer->SetData(_Data.LineVertexBufferBase, dataSize);
			_Data.LineShader->Bind();
			_Data.LineShader->SetUniformMat4f("u_projection", _Data.projection);
			_Data._IndexBuffer->UnBind();
			RendererCommand::SetLineWidth(_Data.LineThickness);
			RendererCommand::DrawLines(_Data.LineVertexCount);
		}
		
	}
}