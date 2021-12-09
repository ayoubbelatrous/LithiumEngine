project "Runtime"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/")
	objdir ("%{wks.location}/bin-int/")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/Lithium/src",
		"%{wks.location}/Lithium/vendor",
		 "%{IncludeDir.GLFW}",
		 "%{IncludeDir.stb_image}",
		 "%{IncludeDir.glm}",
		  "%{IncludeDir.imgui}",
		  "%{IncludeDir.entt}",
		  "%{IncludeDir.imguizmo}",
		  
	}

	links
	{
		"Lithium",
		
	}