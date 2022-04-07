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

    "vendor/spdlog/**/**.hpp",

    "vendor/stb_image/**.h",
	"vendor/stb_image/**.cpp",

    
    "vendor/imguizmo/**.h",
	"vendor/imguizmo/**.cpp",
}
includedirs
{

    "%{wks.location}/Lithium/vendor",
    "%{wks.location}/Lithium/vendor/spdlog/include",
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
    "%{IncludeDir.libogg}",
    "%{IncludeDir.Vorbis}",
    "%{IncludeDir.OpenAlSoft}/include",
    "%{IncludeDir.OpenAlSoft}/src",
    "%{IncludeDir.OpenAlSoft}/src/common",
}

defines
{
   "_CRT_SECURE_NO_WARNINGS",
   		"GLFW_INCLUDE_NONE",
           "AL_LIBTYPE_STATIC",
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
    "%{LibDir.mono}/libmono-static-sgen.lib",
    "Mswsock.lib",
    "ws2_32.lib",
    "psapi.lib",
    "version.lib",
    "winmm.lib",
    "Bcrypt.lib",
    "OpenAl-Soft",
    "Vorbis",
}


	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize(omptimizationLevel)