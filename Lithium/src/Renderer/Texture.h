#pragma once

#include <string>
#include "../Lithium.h"
class Texture
{
private:
	unsigned int _id;
	unsigned char* _localBuffer;
	int _width;
	int _height;
	int _BPP;
public:
	Texture(const std::string& path);
	Texture(int width, int height);
	Texture(){}
	~Texture();
	void Bind(unsigned int slot = 0) const;
	void UnBind() const;
	unsigned int GetID() const;
	void SetData(void* data);
	static Lithium::Ref<Texture> Create(int width,int height);
};