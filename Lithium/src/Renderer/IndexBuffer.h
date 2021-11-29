#pragma once


namespace Lithium
{
	class IndexBuffer
	{
	private:
		unsigned int _id;
		unsigned int _count;
	public:
		IndexBuffer(const unsigned int* data, unsigned int count);
		IndexBuffer();
		~IndexBuffer();
		void Bind() const;
		void UnBind() const;

		inline unsigned int GetCount() const { return _count; }
	};
}