project "yaml-cpp"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"
	targetdir ("bin/%{prj.name}")
	objdir ("bin-int/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp",
		
		"include/**.h"
	}

	includedirs
	{
		"include"
	}

