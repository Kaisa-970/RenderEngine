workspace "PKEngine"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

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
		"%{prj.name}/src/**.cpp"
	}

	includedirs{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/GLFW/include"
	}

	libdirs{
		"%{prj.name}/vendor/GLFW/lib"
	}

	filter "system:windows"
		cppdialect "C++14"
		staticruntime "On"
		systemversion "10.0"

		defines{
			"PK_PLATFORM_WINDOWS",
			"PK_BUILD_DLL",
		}

		postbuildcommands{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
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
		"PKEngine/src"
	}



	links{
		"PKEngine"
	}

	filter "system:windows"
		cppdialect "C++14"
		staticruntime "On"
		systemversion "10.0"

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