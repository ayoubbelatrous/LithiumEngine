#pragma once
namespace Lithium
{
	class IndexBuffer
	{
	public:
		IndexBuffer() = default;
		IndexBuffer(uint32_t size);
		IndexBuffer(uint32_t size,const void* data);
		~IndexBuffer();
		void Bind();
		void UnBind();
		void SetData(uint32_t size, const void* data);
	private:
		uint32_t _Id;
	};
}