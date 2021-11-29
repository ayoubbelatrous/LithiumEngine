project "GLAD"
    kind "StaticLib"
    language "C"
    staticruntime "on"
    
    targetdir ("bin/%{prj.name}")
    objdir ("bin-int/%{prj.name}")

    files
    {
        "include/glad/glad.h",
        "src/glad.c"
    }

    includedirs
    {
        "include"
    }