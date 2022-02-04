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
	defines
	{
		"BUILD_PROFILER",
		
	}

IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Lithium/vendor/glfw/include"
IncludeDir["GLAD"] = "%{wks.location}/Lithium/vendor/glad/include"
IncludeDir["glm"] = "%{wks.location}/Lithium/vendor/glm/glm"
IncludeDir["stb_image"] = "%{wks.location}/Lithium/vendor/stb_image"
IncludeDir["imgui"] = "%{wks.location}/Lithium/vendor/imgui"
IncludeDir["entt"] = "%{wks.location}/Lithium/vendor/entt"
IncludeDir["imguizmo"] = "%{wks.location}/Lithium/vendor/imguizmo"
IncludeDir["yaml"] = "%{wks.location}/Lithium/vendor/yaml/include"
IncludeDir["msdfatlasgen"] = "%{wks.location}/Lithium/vendor/msdf-atlas-gen"
IncludeDir["msdf"] = "%{wks.location}/Lithium/vendor/msdf-atlas-gen/msdfgen"
IncludeDir["assimp"] = "%{wks.location}/Lithium/vendor/assimp/include"

group "Dependencies"
	include "Lithium/vendor/glfw"
	include "Lithium/vendor/glad"
	include "Lithium/vendor/imgui"
	include "Lithium/vendor/yaml"
	include "Lithium/vendor/msdf-atlas-gen"
	include "Lithium/vendor/assimp"
group ""

include "Lithium"
include "Editor"
include "Runtime"
