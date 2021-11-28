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
}
includedirs
{
    "%{wks.location}/Lithium/src",
    "%{IncludeDir.GLFW}",
}

defines
{
   "_CRT_SECURE_NO_WARNINGS",
   		"GLFW_INCLUDE_NONE"
}
links
{
	"GLFW",
	"opengl32.lib"
}




