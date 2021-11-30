project "IMGUI"
	kind "StaticLib"
	language "C++"
    systemversion "latest"
		cppdialect "C++17"
		staticruntime "On"
	targetdir ("bin/%{prj.name}")
	objdir ("bin-int/%{prj.name}")

	files
	{
		"imconfig.h",
		"imgui.h",
		"imgui.cpp",
		"imgui_draw.cpp",
		"imgui_internal.h",
		"imgui_widgets.cpp",
		"imstb_rectpack.h",
		"imstb_textedit.h",
		"imstb_truetype.h",
		"imgui_demo.cpp"
	}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
