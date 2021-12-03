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
namespace Lithium
{
	
	struct QuadVertex
	{
		glm::vec3 pos;
		glm::vec4 color;
		glm::vec2 texCourd;
		float texSlotIndex;
	};

	struct Renderer2DSpecs
	{
		static const int maxQuads = 1000;
		static const uint32_t maxVertices = maxQuads * 4;
		static const uint32_t maxIndices = maxQuads * 6;
		static const uint32_t MaxTextureSlots = 18; // TODO: RenderCaps


		Ref<VertexArray> vertexArray;
		Ref<VertexBuffer> vertexBuffer;
		Ref<VertexBufferLayout> layout;
		Ref<IndexBuffer> quadIB;

		Ref<Shader> textureShader;
		Ref<Texture> WhiteTexture;
		uint32_t QuadIndexCount = 0;

		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;
		std::array<Ref<Texture>, MaxTextureSlots> slots;
		uint32_t textureIndex = 0;
		glm::vec4 QuadVertexPositions[4];
		glm::mat4 viewProjection;
		glm::mat4 _proj;
		glm::mat4 _view;
	};

	static Renderer2DSpecs data;



	void Renderer2D::Init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		data.vertexArray = CreateRef<VertexArray>();
		data.vertexBuffer = CreateRef<VertexBuffer>(data.maxVertices * sizeof(QuadVertex));
		data.layout = CreateRef<VertexBufferLayout>();
		data.layout->Push<float>(3);
		data.layout->Push<float>(4);
		data.layout->Push<float>(2);
		data.layout->Push<float>(1);
		data.vertexArray->AddBuffer(data.vertexBuffer, data.layout);
		data.QuadVertexBufferBase = new QuadVertex[data.maxVertices];

		uint32_t* quadIndices = new uint32_t[data.maxIndices];



		uint32_t offset = 0;
		for (uint32_t i = 0; i < data.maxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		data.quadIB = CreateRef<IndexBuffer>(data.maxIndices,quadIndices);
		delete[] quadIndices;

		data.WhiteTexture = Texture::Create(1, 1);
		unsigned int white = 0xfffffffff;
		data.WhiteTexture->SetData(&white);


		int32_t samplers[data.MaxTextureSlots];

		for (uint32_t i = 0; i < data.MaxTextureSlots; i++)
			samplers[i] = i;

		data.textureShader = CreateRef<Shader>("assets/shaders/main.shader");
		data.slots[0] = data.WhiteTexture;


		data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		data.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

	}




	void Renderer2D::StartBatch()
	{
		data.QuadIndexCount = 0;
		data.QuadVertexBufferPtr = data.QuadVertexBufferBase;

		data.textureIndex = 1;
	}


	void Renderer2D::BeginScene(glm::mat4 proj, glm::mat4 view)
	{
		data._proj = proj;
		data._view = view;
		data.viewProjection = proj * view;
		Renderer2D::StartBatch();
	}
	void Renderer2D::Flush()
	{
		if (data.QuadIndexCount == 0)
			return; // Nothing to draw

		uint32_t dataSize = (uint32_t)((uint8_t*)data.QuadVertexBufferPtr - (uint8_t*)data.QuadVertexBufferBase);
		data.vertexBuffer->SetData(data.QuadVertexBufferBase, dataSize);
		data.vertexArray->Bind();
		data.textureShader->Bind();
		data.textureShader->SetUniformMat4f("viewProjection", data.viewProjection);

		// Bind textures
		for (uint32_t i = 0; i < data.textureIndex; i++)
		{
			data.slots[i]->Bind(i);
			
		}
		int sampler2D[data.MaxTextureSlots];
		for (uint32_t i = 0; i < data.MaxTextureSlots; i++)
		{
			sampler2D[i] = i;
		}


		data.textureShader->SetUniformiv("u_texture",sampler2D);
		data.quadIB->Bind();
		RendererCommand::DrawIndexed(data.QuadIndexCount);
	}


	void Renderer2D::EndScene()
	{


		Renderer2D::Flush();
	}


	void Renderer2D::NextBatch()
	{
		Renderer2D::Flush();
		Renderer2D::StartBatch();
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{


		constexpr size_t quadVertexCount = 4;
		const float textureIndex = 0.0f; // White Texture
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		const float tilingFactor = 1.0f;

		if (data.QuadIndexCount >= Renderer2DSpecs::maxIndices)
			Renderer2D::NextBatch();

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			data.QuadVertexBufferPtr->pos = transform * data.QuadVertexPositions[i];
			data.QuadVertexBufferPtr->color = color;
			data.QuadVertexBufferPtr->texCourd = textureCoords[i];
			data.QuadVertexBufferPtr->texSlotIndex = textureIndex;
			data.QuadVertexBufferPtr++;
		}

		data.QuadIndexCount += 6;
	}



	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture>& texture)
	{
		
			constexpr size_t quadVertexCount = 4;
			constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

			if (data.QuadIndexCount >= Renderer2DSpecs::maxIndices)
				NextBatch();

			float textureIndex = 0.0f;
			for (uint32_t i = 0; i < data.textureIndex; i++)
			{
				if (data.slots[i]->GetID() == texture->GetID())
				{
					textureIndex = (float)i;
					break;
				}
			}

			if (textureIndex == 0.0f)
			{
				if (data.textureIndex >= Renderer2DSpecs::MaxTextureSlots)
					NextBatch();

				textureIndex = (float)data.textureIndex;
				data.slots[data.textureIndex] = texture;
				data.textureIndex++;
			}

			for (size_t i = 0; i < quadVertexCount; i++)
			{
				data.QuadVertexBufferPtr->pos = transform * data.QuadVertexPositions[i];
				data.QuadVertexBufferPtr->color = color;
				data.QuadVertexBufferPtr->texCourd = textureCoords[i];
				data.QuadVertexBufferPtr->texSlotIndex = textureIndex;
				data.QuadVertexBufferPtr++;
			}

			data.QuadIndexCount += 6;

	}
}