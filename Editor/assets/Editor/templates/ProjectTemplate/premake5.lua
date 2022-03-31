workspace "Game"
    architecture "x64"
    startproject "Assembly"

    configurations
    {
        "Release"
    }


-- create c# project
project "Assembly"
    location ""
    kind "SharedLib"
    language "C#"
    framework "4.5"
    targetdir ("Library/")
	objdir ("Library/int/")
    links
    {
    }

    files
    {
        "assets/**.cs"
    }

    filter "configurations:Release"
        defines
        {
            "NDEBUG"
        }
        optimize "On"