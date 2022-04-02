#pragma once
#include "Core/Base.h"
#include <string>

namespace Lithium
{
	class Texture
	{
		
	private:
		unsigned int _id;
		unsigned char* _localBuffer;
		int _width;
		int _height;
		int _BPP;
		std::string _Path;
		
	public:
		enum class TextureType
		{
			RGBA = 0,
			RGB = 1,
		};
		Texture() = default;
		Texture(const std::string& path);
		Texture(char* data,uint32_t size);
		Texture(int width, int height,TextureType type = TextureType::RGBA);
		//Texture():_id(0) {}
		~Texture();
		void Bind(unsigned int slot = 0) const;
		void UnBind() const;
		unsigned int GetID() const;
		void SetData(const void* data);
		static Ref<Texture> Create(int width, int height);
		static Ref<Texture> Load(const std::string& path);
		bool operator==(const Texture& other) const
		{
			return _id == ((Texture&)other)._id;
		}
		bool loaded = false;
		std::string GetPath() { return _Path; }
		int GetWidth() const { return _width; }
		int GetHeight() const { return _height; }
	private:
		TextureType m_Type;
	};
}