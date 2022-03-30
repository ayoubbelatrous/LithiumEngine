#include "lipch.h"
#include "AssetPacker.h"
#include <map>
#include <xhash>

namespace Lithium
{
	AssetPacker::AssetPacker(const std::string& path, std::vector<std::string> PathVector)
	{

		std::map<std::string, uint32_t> pMap;
		for (auto path : PathVector)
		{
			pMap[path] = std::filesystem::file_size(path);
		}

		m_Stream = std::ofstream(path, std::ios::binary);

		const char* HEADER = "LIAP";
		m_Stream.write(HEADER, 4);
		uint32_t size = 2;
		m_Stream.write((char*)&size, 4);

		
		for (auto& entry : pMap)
		{
			size_t hash = std::hash<std::string>{}(entry.first);


			m_Stream.write((char*)&hash, 8); //path hash
			m_Stream.write((char*)&entry.second, 4);
		}
		for (auto& entry : pMap)
		{
			std::ifstream currentFile(entry.first, std::ios::binary);
			int fileSize = entry.second;
			char* buffer = new char[fileSize];
			currentFile.read(buffer, fileSize);

			m_Stream.write(buffer,fileSize);
		}
		m_Stream.close();
	}

	void AssetPacker::Read()
	{

		std::ifstream stream = std::ifstream("assets/testPack.bin", std::ios::binary);

		char* data = new char[4];
		memset(data, 0, 4);
		stream.read(data, 4);
		if (memcmp(data, "LIAP",4) == 0)
		{
			
		}
		else
		{
			CORE_LOG("asset pack not correct");
		}


		uint32_t IndexSize = 0;
		stream.read((char*)&IndexSize, 4);

		CORE_LOG("index size" << IndexSize);
		std::map<uint64_t, uint32_t> pMap;
		for (size_t i = 0; i < IndexSize; i++)
		{
			uint64_t hash = 0;
			uint32_t size = 0;
			stream.read((char*)&hash, 8);
			stream.read((char*)&size, 4);
			pMap[hash] = size;
			m_Index[hash] = size;
		}
	}

	char* AssetPacker::Get(uint32_t* size, const std::string path)
	{
		std::ifstream stream = std::ifstream("assets/testPack.bin", std::ios::binary);
		int skip = CalculateIndexSize() + 4 + 4;
		int offset = 0;
		for (auto key : m_Index)
		{
			CORE_LOG("offset" << offset)
				offset += key.second;
			if (key.first == std::hash<std::string>{}(path))
			{
				stream.seekg(offset + skip - key.second);

				char* buffer = new char[key.second];

				stream.read(buffer, key.second);
				*size = key.second;
				return buffer;
			}

		}
		
		
	}

	int AssetPacker::CalculateIndexSize()
	{
		return m_Index.size() * (8 + 4);
	}

}