project "Lithium"

kind "StaticLib"
language "C++"
cppdialect "C++17"
staticruntime "off"


	targetdir ("%{wks.location}/bin/")
	objdir ("%{wks.location}/bin-int/")

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
    "YAML",
	"opengl32.lib"
}




