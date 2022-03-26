project "IMGUI"
	kind "StaticLib"
	language "C++"
		cppdialect "C++17"
		staticruntime "on"
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"**.cpp",
		"**.h",

	}
		filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize(omptimizationLevel)
