project "Lua"
	kind "StaticLib"
	language "C"
	targetdir ("bin/%{prj.name}")
	objdir ("bin-int/%{prj.name}")

	files
	{
		"src/**.c",
		"src/**.hpp",
		"src/**.h",
	}
	filter "configurations:Debug"
	runtime "Debug"
	symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
