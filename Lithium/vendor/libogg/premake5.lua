project "libogg"
	kind "StaticLib"
	language "C"
	

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	includedirs
	{
		"include"
	}

	files
	{
		"include/**.h",
		"src/**.c"
	}

	defines
	{
		"LIBOGG_EXPORTS"
	}
	filter "system:windows"
		systemversion "latest"
		staticruntime "on"
	    defines
	    {
	    	"WIN32",
	    	"NDEBUG",
	    	"_WINDOWS",
	    	"_USRDLL"
	    }
	filter "system:linux"
		staticruntime "on"
	    defines
	    {
	    	"NDEBUG",
	    }

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize(omptimizationLevel)
