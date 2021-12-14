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

		glm::vec2 cellsize = glm::vec2(128, 256);
		glm::vec2 index = glm::vec2(5, 0);

		glm::vec2 textureCoords[] = {
			{ (index.x * cellsize.x) / _Width, (index.y * cellsize.y) / _Width},
			{ ((index.x + 1) * cellsize.x) / _Width, (index.y * cellsize.y) / _Width},
			{ ((index.x + 1) * cellsize.x) / _Width, ((index.y + 1) * cellsize.y) / _Width},
			{ ((index.x + 1) * cellsize.x) / _Width, ((index.y + 1) * cellsize.y) / _Width},
		};

		return textureCoords;
	}

	Lithium::TextureMode TextureData::GetMode()
	{
		return _Mode;
	}

}



