workspace "PKEngine"
	architecture "x64"

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

include "PKEngine/vendor/GLFW"
include "PKEngine/vendor/Glad"
include "PKEngine/vendor/imgui"

project "PKEngine"
	location "PKEngine"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/"..outputdir.."/%{prj.name}")
	objdir ("bin-int/"..outputdir.."/%{prj.name}")

	pchheader "pkpch.h"
	pchsource "PKEngine/src/pkpch.cpp"

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	includedirs{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.glm}"
	}

	links{
		"GLFW",
		"Glad",
		"opengl32.lib",
		"imgui"
	}

	filter "system:windows"
		cppdialect "C++14"
		staticruntime "On"
		systemversion "latest"

		defines{
			"PK_PLATFORM_WINDOWS",
			"PK_BUILD_DLL",
		}

		postbuildcommands{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

		filter "configurations:Debug"
			defines "PK_DEBUG"
			buildoptions "/MDd"
			symbols "On"

		filter "configurations:Release"
			defines "PK_RELEASE"
			buildoptions "/MD"
			optimize "On"

		filter "configurations:Dist"
			defines "PK_DIST"
			buildoptions "/MD"
			optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

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
		"%{IncludeDir.glm}"
	}



	links{
		"PKEngine"
	}

	filter "system:windows"
		cppdialect "C++14"
		staticruntime "On"
		systemversion "latest"

		defines{
			"PK_PLATFORM_WINDOWS"
		}

		filter "configurations:Debug"
			defines "PK_DEBUG"
			symbols "On"

		filter "configurations:Release"
			defines "PK_RELEASE"
			optimize "On"

		filter "configurations:Dist"
			defines "PK_DIST"
			optimize "On"