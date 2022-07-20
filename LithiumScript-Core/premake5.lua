project "LithiumScript-Core"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"

	targetdir ("%{wks.location}/Editor/Library")
	objdir ("%{wks.location}/Editor/Library/int")

	files 
	{
		"src/**.cs"
	}
	
	filter "configurations:Debug"
		optimize "Off"
		symbols "Default"

	filter "configurations:Release"
		optimize "On"
		symbols "Default"
