workspace "PKEngine"
	architecture "x64"
	startproject "PK-Editor"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "PKEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "PKEngine/vendor/Glad/include"
IncludeDir["imgui"] = "PKEngine/vendor/imgui"
IncludeDir["glm"] = "PKEngine/vendor/glm"
IncludeDir["stb_image"] = "PKEngine/vendor/stb_image"
IncludeDir["entt"] = "PKEngine/vendor/entt/include"
IncludeDir["assimp"] = "PKEngine/vendor/assimp/include"

group "Dependency"
	include "PKEngine/vendor/GLFW"
	include "PKEngine/vendor/Glad"
	include "PKEngine/vendor/imgui"
group ""

project "PKEngine"
	location "PKEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/"..outputdir.."/%{prj.name}")
	objdir ("bin-int/"..outputdir.."/%{prj.name}")

	pchheader "pkpch.h"
	pchsource "PKEngine/src/pkpch.cpp"

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	includedirs{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.assimp}",
	}

	links{
		"GLFW",
		"Glad",
		"opengl32.lib",
		"imgui"
	}

	filter "system:windows"
		systemversion "latest"

		defines{
			"PK_PLATFORM_WINDOWS",
			"PK_BUILD_DLL",
		}

		filter "configurations:Debug"
			libdirs{"PKEngine/vendor/assimp/lib"}
			links{"assimp-vc140-mt.lib"}	
			defines "PK_DEBUG"
			runtime "Debug"
			symbols "On"

		filter "configurations:Release"
			defines "PK_RELEASE"
			runtime "Release"
			optimize "On"

		filter "configurations:Dist"
			defines "PK_DIST"
			runtime "Release"
			optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/"..outputdir.."/%{prj.name}")
	objdir ("bin-int/"..outputdir.."/%{prj.name}")

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs{
		"PKEngine/vendor/spdlog/include",
		"PKEngine/vendor/GLFW/include",
		"PKEngine/src",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}"
	}

	links{
		"PKEngine"
	}

	filter "system:windows"
		systemversion "latest"

		defines{
			"PK_PLATFORM_WINDOWS"
		}

		filter "configurations:Debug"
			defines "PK_DEBUG"
			runtime "Debug"
			symbols "On"

		filter "configurations:Release"
			defines "PK_RELEASE"
			runtime "Release"
			optimize "On"

		filter "configurations:Dist"
			defines "PK_DIST"
			runtime "Release"
			optimize "On"



project "PK-Editor"
	location "PK-Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/"..outputdir.."/%{prj.name}")
	objdir ("bin-int/"..outputdir.."/%{prj.name}")

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs{
		"PKEngine/vendor/spdlog/include",
		"PKEngine/vendor/GLFW/include",
		"PKEngine/src",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}"
	}

	links{
		"PKEngine"
	}

	filter "system:windows"
		systemversion "latest"

		defines{
			"PK_PLATFORM_WINDOWS"
		}

		filter "configurations:Debug"
			defines "PK_DEBUG"
			runtime "Debug"
			symbols "On"

		filter "configurations:Release"
			defines "PK_RELEASE"
			runtime "Release"
			optimize "On"

		filter "configurations:Dist"
			defines "PK_DIST"
			runtime "Release"
			optimize "On"