#include "VertexArray.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &_id);
	Bind();
}

VertexArray::~VertexArray()
{
	//glDeleteVertexArrays(1,&_id);
}

void VertexArray::UnBind() const
{
	glBindVertexArray(0);
}

void VertexArray::Bind() const
{
	
	glBindVertexArray(_id);
	_ibo.Bind();
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size();i++)
	{
		
		const auto& element = elements[i];
		
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count,element.type , false, layout.GetStride(), (const void*) offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::AddIndexBuffer(const IndexBuffer& ibo)
{
	
	_ibo = ibo;
}

