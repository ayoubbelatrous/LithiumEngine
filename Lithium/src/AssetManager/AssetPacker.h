#pragma once


namespace Lithium
{
	class AssetPacker {
	public:
		AssetPacker(const std::string& path,std::vector<std::string> PathVector);
	private:
		char* Get(uint32_t* size,const std::string path);
		std::map<uint64_t, uint32_t> m_Index;
		void Read();
		int CalculateIndexSize();
		std::ofstream m_Stream;
		std::ifstream m_IStream;
	};
}