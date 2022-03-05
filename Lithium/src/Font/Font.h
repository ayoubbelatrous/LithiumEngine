#pragma once
#include <tuple>


namespace Lithium::Font {
	std::tuple<unsigned char*,int> GenFonts(const char* fontFilename);
}
