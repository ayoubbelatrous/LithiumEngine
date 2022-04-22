#include "lipch.h"
#include "Texture.h"
#include "glad/glad.h"
#include "stb_image.h"


namespace Lithium
{
	Texture::Texture(const std::string& path)
	{
		m_Type = TextureType::RGBA;

		_Path = path;
		stbi_set_flip_vertically_on_load(1);
		_localBuffer = stbi_load(path.c_str(), &_width, &_height, &_BPP, 4);
		glGenTextures(1, &_id);
		glBindTexture(GL_TEXTURE_2D, _id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _localBuffer);
		//glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(_localBuffer);
		
		loaded = true;
	}

	Texture::Texture(int width, int height, TextureType type /*TextureType::RGBA*/)
	{
		m_Type = type;
		if (type == TextureType::RGBA)
		{
			_width = width;
			_height = height;
			_localBuffer = nullptr;
			glGenTextures(1, &_id);
			glBindTexture(GL_TEXTURE_2D, _id);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			loaded = true;
		}
		else if(type == TextureType::RGB)
		{
			
			_width = width;
			_height = height;
			_localBuffer = nullptr;
			glGenTextures(1, &_id);
			glBindTexture(GL_TEXTURE_2D, _id);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_BYTE, NULL);
			loaded = true;
		}
		
	}

	Texture::Texture(char* data,uint32_t size)
	{
		m_Type = TextureType::RGBA;
		stbi_set_flip_vertically_on_load(1);
		_localBuffer = stbi_load_from_memory((stbi_uc*)data,size, &_width, &_height, &_BPP, 4);

		glGenTextures(1, &_id);
		glBindTexture(GL_TEXTURE_2D, _id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _localBuffer);
		//glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(_localBuffer);

		loaded = true;
	}


	Texture::~Texture()
	{
		glDeleteTextures(1, &_id);
	}

	void Texture::Bind(unsigned int slot) const
	{
		
		//glActiveTexture(GL_TEXTURE0+ slot);
		//glBindTexture(GL_TEXTURE_2D, _id);
		glBindTextureUnit(slot, _id);
	}

	void Texture::UnBind() const
	{
	
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	unsigned int Texture::GetID() const
	{
		return _id;
	}

	void Texture::SetData(const void* data)
	{
		switch (m_Type)
		{
		case Lithium::Texture::TextureType::RGBA:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			break;
		case Lithium::Texture::TextureType::RGB:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			break;
		}
	}

	Ref<Texture> Texture::Create(int width, int height)
	{
		return Lithium::CreateRef<Texture>(width, height);
	}

	Ref<Texture> Texture::Load(const std::string& path)
	{
		return CreateRef<Texture>(path);
	}

}