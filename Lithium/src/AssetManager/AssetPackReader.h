#pragma once


namespace Lithium
{
	class AssetPackReader
	{
	public:
		AssetPackReader(const std::string reader);
		char* Get(uint32_t* size, const std::string& path);
	private:
		int CalculateIndexSize();
		std::map<uint64_t, uint32_t> m_Index;
		std::ifstream m_Stream;
		std::string m_Path;
	};
}