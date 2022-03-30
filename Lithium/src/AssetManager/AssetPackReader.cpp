#include "lipch.h"
#include "AssetPackReader.h"

namespace Lithium
{

	AssetPackReader::AssetPackReader(const std::string path)
	{
		m_Stream = std::ifstream(path, std::ios::binary);
		m_Path = path;
		char* data = new char[4];
		memset(data, 0, 4);
		m_Stream.read(data, 4);
		if (memcmp(data, "LIAP", 4) == 0)
		{
			CORE_LOG("reading .. " << path)
		}
		else
		{
			CORE_LOG("asset pack corrupted");
		}


		uint32_t IndexSize = 0;
		m_Stream.read((char*)&IndexSize, 4);
		for (size_t i = 0; i < IndexSize; i++)
		{
			uint64_t hash = 0;
			uint32_t size = 0;
			m_Stream.read((char*)&hash, 8);
			m_Stream.read((char*)&size, 4);
			m_Index[hash] = size;
		}
		m_Stream.close();
	}

	char* AssetPackReader::Get(uint32_t* size, const std::string& path)
	{
		std::ifstream stream = std::ifstream(m_Path, std::ios::binary);

		int skip = CalculateIndexSize() + 4 + 4;
		int offset = 0;


		for (auto key : m_Index)
		{
			uint64_t hash = key.first;
			uint32_t Isize = key.second;
			uint64_t secondHash = std::hash<std::string>{}(path);

			offset += Isize;
			if (hash == secondHash)
			{
				stream.seekg(offset + skip - Isize);

				char* buffer = new char[Isize];

				stream.read(buffer, Isize);
				*size = Isize;
				return buffer;
				break;
			}

		}
	}

	int AssetPackReader::CalculateIndexSize()
	{
		return m_Index.size() * (8 + 4);
	}

}