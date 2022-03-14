project "Lithium"

kind "StaticLib"
language "C++"
cppdialect "C++17"
staticruntime "off"


	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/".. outputdir .. "/%{prj.name}")

pchheader "lipch.h"
pchsource "src/lipch.cpp"

files
{
    "src/**.h",
    "src/**.cpp",

    "vendor/glm/glm/**.hpp",
	"vendor/glm/glm/**.inl",


    "vendor/stb_image/**.h",
	"vendor/stb_image/**.cpp",

    
    "vendor/imguizmo/**.h",
	"vendor/imguizmo/**.cpp",
}
includedirs
{
    "%{wks.location}/Lithium/src",
    "%{IncludeDir.GLFW}",
    "%{IncludeDir.GLAD}",
    "%{IncludeDir.glm}",
    "%{IncludeDir.stb_image}",
    "%{IncludeDir.imgui}",
    "%{IncludeDir.entt}",
    "%{IncludeDir.imguizmo}",
    "%{IncludeDir.yaml}",
    "%{IncludeDir.msdf}",
    "%{IncludeDir.msdfatlasgen}",
    "%{IncludeDir.assimp}",
    "%{IncludeDir.mono}",
    "%{IncludeDir.Box2D}",

    
}

defines
{
   "_CRT_SECURE_NO_WARNINGS",
   		"GLFW_INCLUDE_NONE"
}
links
{
	"GLFW",
    "GLAD",
    "IMGUI",
    "yaml-cpp",
	"opengl32.lib",
    "msdf-atlas-gen",
    "Box2D",
    "assimp",
    "%{LibDir.mono}/mono-2.0-sgen.lib",
}


	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"