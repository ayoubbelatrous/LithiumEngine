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
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
omptimizationLevel = "Speed"

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
IncludeDir["lua533"] = "%{wks.location}/Lithium/vendor/lua533/src"
IncludeDir["Box2D"] = "%{wks.location}/Lithium/vendor/box2d/include"
IncludeDir["OpenAlSoft"] = "%{wks.location}/Lithium/vendor/OpenAL-Soft"
IncludeDir["libogg"] = "%{wks.location}/Lithium/vendor/libogg/include"
IncludeDir["Vorbis"] = "%{wks.location}/Lithium/vendor/Vorbis/include"

if os.getenv("MONO_DIR") then
	IncludeDir["mono"] = os.getenv("MONO_DIR") .. "include/mono-2.0/"
else
	print("please set env var for mono path called MONO_DIR or use default [y,n]")
	print("C:/Program Files/Mono")
	local answer = io.read()
	if answer == "n" then
		IncludeDir["mono"] ="mononull"	

	else
		IncludeDir["mono"] ="C:/Program Files/Mono"	
	end

end


LibDir = {}
LibDir["mono"] = os.getenv("MONO_DIR") .. "lib"

group "Dependencies"
	include "Lithium/vendor/glfw"
	include "Lithium/vendor/glad"
	include "Lithium/vendor/imgui"
	include "Lithium/vendor/yaml"
	include "Lithium/vendor/msdf-atlas-gen"
	include "Lithium/vendor/assimp"
	include "Lithium/vendor/box2d"
	include "Lithium/vendor/OpenAL-Soft"
	include "Lithium/vendor/Vorbis"
	include "Lithium/vendor/libogg"
group ""

include "Lithium"
include "Editor"
