#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"

class VertexArray
{
private:
	unsigned int _id;
	IndexBuffer _ibo;
public:
	VertexArray();
	~VertexArray();
	void UnBind() const;
	void Bind() const;
	void AddBuffer(const VertexBuffer& vb,const VertexBufferLayout& layout);
	void AddIndexBuffer(const IndexBuffer& ibo);
};