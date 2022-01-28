#pragma once

#include "VertexBufferLayout.h"
#include "Core/Base.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"


namespace Lithium
{
	class VertexArray {
	public:
		VertexArray();
		~VertexArray();
		void UnBind() const;
		void Bind() const;
		void AddBuffer(Ref<VertexBuffer> vb, Ref<VertexBufferLayout> layout);
	private:
		unsigned int _id;
	};
}
