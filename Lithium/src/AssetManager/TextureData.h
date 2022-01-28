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
		TextureData():
			_Mode(TextureMode::Single)
		{
		}
		glm::vec2 GetCellSize();
		int GetCellSizeX() const { return _CellsizeX; }
		int GetCellSizeY() const { return _CellsizeY; }
		int GetWidth() const { return _Width; };
		int GetHeight() const { return _Height; };
		glm::vec2* CalculateUVs(int indexY = 0,int indexX = 0);

		TextureMode GetMode();
		void SliceByCount(int x,int y);
	private:
		TextureMode _Mode = TextureMode::Single;
		glm::vec2* coords;
		int _CellsizeX;
		int _CellsizeY;
		int _Width;
		int _Height;
	};
}