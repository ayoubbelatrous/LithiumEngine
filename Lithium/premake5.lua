project "Lithium"

kind "StaticLib"
language "C++"
cppdialect "C++17"
staticruntime "off"


	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/".. outputdir .. "/%{prj.name}")

--pchheader "lipch.h"
--pchsource "src/lipch.cpp"

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
    "Vorbis",
}
defines
{
    "GLFW_INCLUDE_NONE",
     "AL_LIBTYPE_STATIC",
}
 
    filter "files:vendor/imguizmo/**.cpp"
	   flags { "NoPCH" }


    filter "system:linux"
       defines
       {
            "LT_PLATFORM_LINUX",
            "_LINUX",
            "AL_BUILD_LIBRARY",
            "AL_ALEXT_PROTOTYPES",
            "NOMINMAX",
            "CMAKE_INTDIR=\"Debug\"",
            "OpenAL_EXPORTS"
       }
       
    filter "system:linux"
       links
       {
           "%{LibDir.mono}/linux/libmono-static-sgen.a",
           "%{LibDir.OpenAlSoft}/lib/libopenal.so",
       }
    filter "system:windows"
       systemversion "latest"
       defines
       {
            "LT_PLATFORM_WINDOWS",
            "_CRT_SECURE_NO_WARNINGS",
       }
    filter "system:windows"
       systemversion "latest"
       links
       {
           "%{LibDir.mono}/libmono-static-sgen.lib",
          "Mswsock.lib",
          "ws2_32.lib",
          "psapi.lib",
          "version.lib",
          "winmm.lib",
          "Bcrypt.lib",
       }

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize(omptimizationLevel)