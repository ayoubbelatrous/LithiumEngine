#pragma once
class VertexBuffer
{
private:
	unsigned int _id;
	
public:

	VertexBuffer(const void* data,unsigned int size);
	VertexBuffer();
	VertexBuffer(unsigned int size);

	~VertexBuffer();
	void Bind() const;
	void UnBind() const;
	void SetData(void* data, unsigned int size);
};