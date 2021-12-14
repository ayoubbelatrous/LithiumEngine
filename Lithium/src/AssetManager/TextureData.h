#pragma once
#include "glm.hpp"



namespace Lithium
{
	enum class TextureMode
	{
		None,Single,Multiple
	};
	class TextureData
	{
	public:
		TextureData(TextureMode mode, int CellsizeX, int CellsizeY,int width,int height);
		glm::vec2 GetCellSize();
		glm::vec2* CalculateUVs(int indexY = 0,int indexX = 0);
		TextureMode GetMode();
	private:
		TextureMode _Mode;
		int _CellsizeX;
		int _CellsizeY;
		int _Width;
		int _Height;
	};
}