#pragma once
#include <vector>

namespace Lithium
{

	struct VertexBufferElement
	{

		unsigned int count;
		unsigned int type;
		bool normalize;

		static unsigned int GetSizeOfType(unsigned int type);
	};

	class VertexBufferLayout
	{
	private:
		std::vector<VertexBufferElement> elements;
		unsigned int Stride;
	public:

		VertexBufferLayout()
			:Stride(0) {}
		~VertexBufferLayout() {}


		template<typename T>
		void Push(unsigned int count);

		inline unsigned int GetStride() const { return Stride; }
		inline std::vector<VertexBufferElement> GetElements() const { return elements; }
	};

}