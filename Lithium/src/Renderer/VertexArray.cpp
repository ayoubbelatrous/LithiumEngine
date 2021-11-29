#include "lipch.h"
#include "VertexArray.h"

#include <glad/glad.h>

namespace Lithium
{
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
	}

	void VertexArray::AddBuffer(Ref<VertexBuffer> vb, Ref<VertexBufferLayout> layout)
	{
		Bind();
		vb->Bind();
		const auto& elements = layout->GetElements();
		unsigned int offset = 0;
		for (unsigned int i = 0; i < elements.size(); i++)
		{

			const auto& element = elements[i];

			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, element.count, element.type, false, layout->GetStride(), (const void*)offset);
			offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
		}
	}

}



