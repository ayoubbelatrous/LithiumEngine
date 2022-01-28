#pragma once

namespace Lithium
{
	class VertexBuffer
	{
	public:
		//VertexBuffer();
		VertexBuffer(uint32_t size);
		VertexBuffer(const void* data, uint32_t size);
		void SetData(const void* data,uint32_t size);
		void Bind();
		void UnBind();
		uint32_t GetID();
		~VertexBuffer();
	private:
		uint32_t _Id;
	};
}










