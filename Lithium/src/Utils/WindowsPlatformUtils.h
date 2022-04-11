#pragma once
#include <string>

namespace Lithium
{
	class FileDialogs
	{
	public:
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
		static std::string OpenFolder(const char* filter);
	};
}