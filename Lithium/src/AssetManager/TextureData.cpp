#include "lipch.h"
#include "TextureData.h"

namespace Lithium
{

	TextureData::TextureData(TextureMode mode, int CellsizeX, int CellsizeY, int width, int height)
		:_Mode(mode), _CellsizeX(CellsizeX), _CellsizeY(CellsizeY), _Width(width), _Height(height)
	{

	}

	glm::vec2 TextureData::GetCellSize()
	{
		return glm::vec2(_CellsizeX, _CellsizeY);
	}

	glm::vec2* TextureData::CalculateUVs(int indexY /*= 0*/, int indexX /*= 0*/)
	{

		glm::vec2 cellsize = glm::vec2(_CellsizeX, _CellsizeY);
		glm::vec2 index = glm::vec2(indexX, indexY);

		coords = new glm::vec2[]{
			{ (index.x * cellsize.x) / _Width, (index.y * cellsize.y) / _Height},
			{ ((index.x + 1) * cellsize.x) / _Width, (index.y * cellsize.y) / _Height},
			{ ((index.x + 1) * cellsize.x) / _Width, ((index.y + 1) * cellsize.y) / _Height},
			{ (index.x * cellsize.x) / _Width, ((index.y + 1) * cellsize.y) / _Height},
		};

		
	
		return coords;
	}

	TextureMode TextureData::GetMode()
	{
		return _Mode;
	}

	void TextureData::SliceByCount(int x, int y)
	{
		_CellsizeX = _Width / x;
		_CellsizeY = _Width / y;
	}

}



