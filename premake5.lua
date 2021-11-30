workspace "Lithium"
	architecture "x64"
	startproject "Editor"

	configurations
	{
		"Debug",
		"Release"
	}

	flags
	{
		"MultiProcessorCompile"
	}


IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Lithium/vendor/glfw/include"
IncludeDir["GLAD"] = "%{wks.location}/Lithium/vendor/glad/include"
IncludeDir["glm"] = "%{wks.location}/Lithium/vendor/glm/glm"
IncludeDir["stb_image"] = "%{wks.location}/Lithium/vendor/stb_image"
IncludeDir["imgui"] = "%{wks.location}/Lithium/vendor/imgui"


group "Dependencies"

	include "Lithium/vendor/glfw"
	include "Lithium/vendor/glad"
	include "Lithium/vendor/imgui"
group ""

include "Lithium"
include "Editor"
