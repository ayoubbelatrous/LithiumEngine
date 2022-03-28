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
#include "gtc/matrix_transform.hpp"
#include "Scene/Components.h"

namespace Lithium
{
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
		const static uint32_t MaxQuadCount = 1000;
		const static uint32_t MaxVertices = MaxQuadCount * 4;
		const static uint32_t MaxIndices = MaxQuadCount * 6;
		const static uint32_t MaxTextures = 18;
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
		_Data._VertexBufferBase = new Vertex[_Data.MaxQuadCount];
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

		_Data.projection = projection * transform;
		_Data._IndexCount = 0;
		_Data._VertexBufferPtr = _Data._VertexBufferBase;
		_Data.TextureSlotIndex = 1;
		_Data._VertexArray->Bind();
		_Data._VertexBuffer->Bind();
		_Data._IndexBuffer->Bind();

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


	void BatchRenderer::DrawData()
	{
		LT_PROFILE_FUNCTION("Renderer Draw Data");
		uint32_t dataSize = (uint32_t)((uint8_t*)_Data._VertexBufferPtr- (uint8_t*)_Data._VertexBufferBase);

		_Data._VertexBuffer->SetData(_Data._VertexBufferBase, dataSize);
		int32_t samplers[RendererData::MaxTextures];
		for (uint32_t i = 0; i < RendererData::MaxTextures; i++)
			samplers[i] = i;
		for (uint32_t i = 0; i < _Data.TextureSlotIndex; i++)
			_Data.TextureSlots[i]->Bind(i);
		_Data._VertexArray->Bind();
		_Data._VertexBuffer->Bind();
		_Data._shader->Bind();
		_Data._shader->SetUniformMat4f("u_projection",_Data.projection);

		_Data._shader->SetUniformiv("u_textures", samplers);
		
		_Data._IndexBuffer->Bind();
		RendererCommand::DrawIndexed(_Data._IndexCount);
	}


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
		const static uint32_t MaxQuadCount = 1000;
		const static uint32_t MaxVertices = MaxQuadCount * 4;
		const static uint32_t MaxIndices = MaxQuadCount * 6;
		const static uint32_t MaxTextures = 18;
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
	};
	

	static FontRendererData _FontData;

	void FontRenderer::Init()
	{
		LT_PROFILE_FUNCTION("Renderer Init");
		_FontData._VertexArray = CreateRef<VertexArray>();
		_FontData._VertexBuffer = CreateRef<VertexBuffer>(_FontData.MaxVertices * sizeof(Vertex));

		Ref<VertexBufferLayout> Layout = CreateRef<VertexBufferLayout>();
		Layout->Push<float>(3);
		Layout->Push<float>(4);
		Layout->Push<float>(2);
		Layout->Push<float>(1);
		Layout->Push<float>(1);
		_FontData._VertexArray->AddBuffer(_FontData._VertexBuffer, Layout);
		_FontData._shader = CreateRef<Shader>("assets/shaders/font.shader");
		_FontData._VertexBufferBase = new Vertex[_FontData.MaxQuadCount];
		uint32_t* Indices = new uint32_t[_FontData.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < _FontData.MaxIndices; i += 6)
		{
			Indices[i + 0] = offset + 0;
			Indices[i + 1] = offset + 1;
			Indices[i + 2] = offset + 2;
			Indices[i + 3] = offset + 2;
			Indices[i + 4] = offset + 3;
			Indices[i + 5] = offset + 0;
			offset += 4;
		}
		_FontData._IndexBuffer = CreateRef<IndexBuffer>(_FontData.MaxIndices, Indices);

		delete[]Indices;


		_FontData.whitetexture = CreateRef<Texture>(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		_FontData.whitetexture->SetData(&whiteTextureData);
		_FontData.TextureSlots[0] = _FontData.whitetexture;
		_FontData._VertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		_FontData._VertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		_FontData._VertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		_FontData._VertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
		_FontData.projection = glm::mat4(0);
	}

	void FontRenderer::Begin(const glm::mat4& transform, const glm::mat4& projection)
	{
		_FontData.projection = projection * transform;
		_FontData._IndexCount = 0;
		_FontData._VertexBufferPtr = _FontData._VertexBufferBase;
		_FontData.TextureSlotIndex = 1;
	}

	void FontRenderer::End()
	{
		DrawData();
	}

	void FontRenderer::DrawData()
	{
		uint32_t dataSize = (uint32_t)((uint8_t*)_FontData._VertexBufferPtr - (uint8_t*)_FontData._VertexBufferBase);

		_FontData._VertexBuffer->SetData(_FontData._VertexBufferBase, dataSize);
		int32_t samplers[RendererData::MaxTextures];
		for (uint32_t i = 0; i < RendererData::MaxTextures; i++)
			samplers[i] = i;
		for (uint32_t i = 0; i < _FontData.TextureSlotIndex; i++)
			_FontData.TextureSlots[i]->Bind(i);
		_FontData._VertexArray->Bind();

		_FontData._shader->Bind();
		_FontData._shader->SetUniformMat4f("u_projection", _FontData.projection);

		_FontData._shader->SetUniformiv("u_textures", samplers);

		_FontData._IndexBuffer->Bind();
		RendererCommand::DrawIndexed(_FontData._IndexCount);
	}

}