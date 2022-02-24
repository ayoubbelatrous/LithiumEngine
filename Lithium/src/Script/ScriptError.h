#pragma once
#define SCRIPT_ERROR(x) {std::cout << x << std::endl;}

enum class ScriptError
{

	None = 0,
	NotFound,
};