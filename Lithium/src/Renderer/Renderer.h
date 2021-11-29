#pragma once


namespace Lithium
{
	class RendererCommand
	{
	public:
		static void Clear();
		static void DrawIndexed(uint32_t count);
		static void Draw(uint32_t count);
	};
}
