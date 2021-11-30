project "IMGUI"
	kind "StaticLib"
	language "C++"
		cppdialect "C++17"
		staticruntime "on"
	targetdir ("bin/%{prj.name}")
	objdir ("bin-int/%{prj.name}")

	files
	{
		"**.cpp",
		"**.h",

	}
