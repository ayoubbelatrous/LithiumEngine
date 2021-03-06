#pragma once

namespace Lithium
{
	class TextureMetaData
	{
	public:
		enum class TextureMode
		{
			Single = 0,
			Multiple = 1,
		};

		enum class TextureType
		{
			None = 0,
			Sprite = 1,
		};
		enum class FilteringMode
		{
			Linear = 0,
			Nearest = 1,
		};

		TextureMetaData() = default;
		TextureMetaData(TextureMode mode,TextureType type,int cellsizeX, int cellsizeY)
			:Mode(mode),Type(type),CellsizeX(cellsizeX), CellsizeY(cellsizeY)
		{}

		TextureMetaData(TextureMode mode, TextureType type,FilteringMode filteringmode, int cellsizeX, int cellsizeY)
			:Mode(mode), Type(type), CellsizeX(cellsizeX), CellsizeY(cellsizeY), filteringMode(filteringmode)
		{}

		TextureMode Mode = TextureMode::Single;
		TextureType Type = TextureType::Sprite;
		FilteringMode filteringMode = FilteringMode::Linear;
		int CellsizeX, CellsizeY = 0;
	};
}