#include "lipch.h"
#include "AssetPacker.h"
#include <map>
#include <xhash>

namespace Lithium
{
	AssetPacker::AssetPacker(const std::string& path, std::vector<std::string> PathVector)
	{
		int size = 0;
		std::map<std::string, uint32_t> pMap;
		for (auto path : PathVector)
		{
			size++;
			pMap[path] = std::filesystem::file_size(path);
		}

		m_Stream = std::ofstream(path, std::ios::binary);

		const char* HEADER = "LIAP";
		m_Stream.write(HEADER, 4);
		m_Stream.write((char*)&size, 4);

		
		for (auto& entry : pMap)
		{
			size_t hash = std::hash<std::string>{}(entry.first);

			CORE_LOG("path : " << entry.first)
			CORE_LOG("hash : " << hash)
			CORE_LOG("size : " << entry.second)
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
}