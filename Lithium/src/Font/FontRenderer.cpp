#include "lipch.h"

#include "Renderer/Renderer.h"
#include "FontRenderer.h"
#include "Core/Base.h"
#include "Core/Log.h"
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
		float pxRange = 6.0f;
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

	void FontRenderer::BeginScene(const glm::mat4& transform, const glm::mat4& projection)
	{
		s_Data.projection = projection * glm::inverse(transform);
		s_Data.IndexCount = 0;
		s_Data.TextureSlotIndex = 0;
		s_Data.VertexBufferPtr = s_Data.VertexBufferBase;

	}

	void FontRenderer::DrawString(const glm::mat4& transform, const std::string& text, const Ref<Font>& font, const glm::vec4& color)
	{
		int VertexCount = 4;
		float textureIndex = 0.0f;
		float cursor = 0.0f;
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

		for (size_t i = 0; i < text.size(); i++)
		{
			Font::Character currentChar = font->GetCharacter(text.at(i));

			glm::vec2 cellsize = glm::vec2(currentChar.PackedSize.x, currentChar.PackedSize.y);
			glm::vec2 index = glm::vec2(currentChar.PackedPos.x / currentChar.PackedSize.x, currentChar.PackedPos.y / currentChar.PackedSize.y);
		
			float Width = font->GetAtlasSize().x;
			float Height = font->GetAtlasSize().y;
			float aspectRatio = currentChar.PackedSize.x / currentChar.PackedSize.y;
			glm::vec2* textureCoords = new glm::vec2[]{
				{ (index.x * cellsize.x) / Width, (index.y * cellsize.y) / Height},
				{ ((index.x + 1) * cellsize.x) / Width, (index.y * cellsize.y) / Height},
				{ ((index.x + 1) * cellsize.x) / Width, ((index.y + 1) * cellsize.y) / Height},
				{ (index.x * cellsize.x) / Width, ((index.y + 1) * cellsize.y) / Height},
			};
			float scale = 50.0f;

			float ScaledLeft = currentChar.Bounds.left * scale;
			float ScaledRight = currentChar.Bounds.right * scale;
			float ScaledTop = currentChar.Bounds.top * scale;
			float ScaledBottom = currentChar.Bounds.bottom * scale;
			float ScaledAdvance = currentChar.Advance * scale;

			glm::vec4 VertexPositions[4];
			VertexPositions[0] = {cursor + ScaledLeft,ScaledBottom + 300.0f,0.0,1.0f}; // bottom left
			VertexPositions[1] = {cursor + ScaledRight,ScaledBottom + 300.0f,0.0,1.0f }; // bottom right
			VertexPositions[2] = {cursor + ScaledRight,ScaledTop + 300.0f,0.0,1.0f }; // top right
			VertexPositions[3] = {cursor + ScaledLeft,ScaledTop + 300.0f,0.0,1.0f }; // top left

			for (size_t i = 0; i < VertexCount; i++)
			{
				
				s_Data.VertexBufferPtr->Position = VertexPositions[i];
				s_Data.VertexBufferPtr->Color = color;
				s_Data.VertexBufferPtr->TextureCoords = textureCoords[i];
				s_Data.VertexBufferPtr->TextureIndex = textureIndex;
				s_Data.VertexBufferPtr->EntityID = -1;
				s_Data.VertexBufferPtr++;
				
			}
			
			cursor += (currentChar.Advance) * scale; 
			s_Data.IndexCount += 6;
		}

	}

	void FontRenderer::DrawString(const glm::vec2& Position, float Scale, const std::string& text, const Ref<Font>& font, const glm::vec4& color, float Spacing, float LineSpacing, int EntityID)
	{
		int VertexCount = 4;
		float textureIndex = 0.0f;
		float cursor = 0.0f;
		float cursorY = 0.0f;
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

		for (size_t i = 0; i < text.size(); i++)
		{
			if (text.at(i) == '\n')
			{
				cursorY -= (font->GetLineHeight() * Scale) + (LineSpacing * Scale);
				cursor = 0;
				continue;
			}
			Font::Character currentChar = font->GetCharacter(text.at(i));

			glm::vec2 cellsize = glm::vec2(currentChar.PackedSize.x, currentChar.PackedSize.y);
			glm::vec2 index = glm::vec2(currentChar.PackedPos.x / currentChar.PackedSize.x, currentChar.PackedPos.y / currentChar.PackedSize.y);

			float Width = font->GetAtlasSize().x;
			float Height = font->GetAtlasSize().y;
			float aspectRatio = currentChar.PackedSize.x / currentChar.PackedSize.y;
			glm::vec2 textureCoords[4]  = {
				{ (index.x * cellsize.x) / Width, (index.y * cellsize.y) / Height},
				{ ((index.x + 1) * cellsize.x) / Width, (index.y * cellsize.y) / Height},
				{ ((index.x + 1) * cellsize.x) / Width, ((index.y + 1) * cellsize.y) / Height},
				{ (index.x * cellsize.x) / Width, ((index.y + 1) * cellsize.y) / Height},
			};
			float scale = Scale;

			float ScaledLeft = currentChar.Bounds.left * scale;
			float ScaledRight = currentChar.Bounds.right * scale;
			float ScaledTop = currentChar.Bounds.top * scale;
			float ScaledBottom = currentChar.Bounds.bottom * scale;
			float ScaledAdvance = currentChar.Advance * scale;

			glm::vec4 VertexPositions[4];
			VertexPositions[0] = { cursor + ScaledLeft + Position.x,ScaledBottom + Position.y + cursorY,0.0,1.0f }; // bottom left
			VertexPositions[1] = { cursor + ScaledRight + Position.x,ScaledBottom + Position.y + cursorY,0.0,1.0f }; // bottom right
			VertexPositions[2] = { cursor + ScaledRight + Position.x,ScaledTop + Position.y + cursorY,0.0,1.0f }; // top right
			VertexPositions[3] = { cursor + ScaledLeft + Position.x,ScaledTop + Position.y + cursorY,0.0,1.0f }; // top left

			for (size_t i = 0; i < VertexCount; i++)
			{

				s_Data.VertexBufferPtr->Position = VertexPositions[i];
				s_Data.VertexBufferPtr->Color = color;
				s_Data.VertexBufferPtr->TextureCoords = textureCoords[i];
				s_Data.VertexBufferPtr->TextureIndex = textureIndex;
				s_Data.VertexBufferPtr->EntityID = EntityID;
				s_Data.VertexBufferPtr++;

			}

			cursor += (currentChar.Advance * scale) + (Spacing * scale);
			s_Data.IndexCount += 6;
		}
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
		s_Data.shader->SetUniform1f("pxRange", s_Data.pxRange);
		s_Data.FontIndexBuffer->Bind();
		RendererCommand::DrawIndexed(s_Data.IndexCount);
	}

	void FontRenderer::EndScene()
	{
		DrawData();
	}

	void FontRenderer::SetPixelRange(float pxr)
	{
		s_Data.pxRange = pxr;
	}

	float FontRenderer::GetPixelRange()
	{
		return s_Data.pxRange;
	}

}