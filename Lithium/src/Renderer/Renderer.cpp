#include "Renderer.h"
#include <iostream>

#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <array>
#include "Shader.h"
#include "../Lithium.h"


void ClearGLError()
{
	while (glGetError() != GL_NO_ERROR)
		;
}

bool LogGLCall(const char* name, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "{GL Error} (" << error << "): " << name << std::endl;
		return false;
	}
	return true;
}

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


	Lithium::Ref<VertexArray> vertexArray;
	Lithium::Ref<VertexBuffer> vertexBuffer;
	Lithium::Ref<VertexBufferLayout> layout;
	Lithium::Ref<IndexBuffer> quadIB;

	Lithium::Ref<Shader> textureShader;
	Lithium::Ref<Texture> WhiteTexture;
	uint32_t QuadIndexCount = 0;

	QuadVertex* QuadVertexBufferBase = nullptr;
	QuadVertex* QuadVertexBufferPtr = nullptr;
	std::array<Lithium::Ref<Texture>, MaxTextureSlots> slots;
	uint32_t textureIndex = 1;
	glm::vec4 QuadVertexPositions[4];
	glm::mat4 viewProjection;
	glm::mat4 _proj;
	glm::mat4 _view;
};

static Renderer2DSpecs data;



	void Renderer2D::Init()
	{

		data.vertexArray = Lithium::CreateRef<VertexArray>();
		data.vertexBuffer = Lithium::CreateRef<VertexBuffer>(data.maxVertices * sizeof(QuadVertex));
		data.layout = Lithium::CreateRef<VertexBufferLayout>();
		data.layout->Push<float>(3);
		data.layout->Push<float>(4);
		data.layout->Push<float>(2);
		data.layout->Push<float>(1);
		data.vertexArray->AddBuffer(*data.vertexBuffer, *data.layout);
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

		data.quadIB = Lithium::CreateRef<IndexBuffer>(quadIndices, data.maxIndices);
		delete[] quadIndices;

		data.WhiteTexture = Texture::Create(1, 1);
		unsigned int white = 0xfff;
		data.WhiteTexture->SetData(&white);


		int32_t samplers[data.MaxTextureSlots];

		for (uint32_t i = 0; i < data.MaxTextureSlots; i++)
			samplers[i] = i;

		data.textureShader = Lithium::CreateRef<Shader>("src/shaders/main.shader");
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

		// Bind textures
		for (uint32_t i = 0; i < data.textureIndex; i++)
			data.slots[i]->Bind(i);
		data.vertexArray->Bind();
		data.textureShader->Bind();
		data.textureShader->SetUniformMat4f("viewProjection", data.viewProjection);
		data.quadIB->Bind();
		Renderer::Draw(data.QuadIndexCount);
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