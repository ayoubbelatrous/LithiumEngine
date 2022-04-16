project "Editor"

	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/".. outputdir .. "/%{prj.name}")
	
	pchheader "epch.h"
	pchsource "src/epch.cpp"
	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
	     "%{wks.location}/Editor/src",
		 "%{wks.location}/Lithium/src",
		 "%{wks.location}/Lithium/vendor",
		 "%{IncludeDir.GLFW}",
		 "%{IncludeDir.stb_image}",
		 "%{IncludeDir.glm}",
		  "%{IncludeDir.imgui}",
		  "%{IncludeDir.entt}",
		  "%{IncludeDir.imguizmo}",
		  "%{IncludeDir.lua533}",
		  "%{IncludeDir.mono}",
		  "%{IncludeDir.spdlog}",
		  
	}

	links
	{
		"Lithium",
		
	}
	
	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize(omptimizationLevel)
		
	filter "system:windows"
     files { 'resources.rc', '**.ico' }
     vpaths { ['Resources/*'] = { '*.rc', '**.ico' }}

