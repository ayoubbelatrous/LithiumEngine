#pragma once
#include <string>

namespace Lithium
{
	class FileDialogs
	{
	public:
		static std::string OpenFile(const char* filter);
	};
}