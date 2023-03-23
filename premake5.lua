workspace "CircleCollision"
	configurations { "Debug", "Release" }
	platforms { "x64" }

project "CircleCollision"
	location ""
	language "C++"
	kind "ConsoleApp"

	targetdir ("")
	objdir ("obj")

	files 
	{ 
		"CircleCollision/**.h", 
		"CircleCollision/**.hpp",
		"CircleCollision/**.cpp",
	}

	includedirs
	{
		"../../Libraries/GLFW/include",
		"../../Libraries/GLEW/include",
		"../../Libraries/glfw-3.3.8.bin.WIN64/include",
		"../../Libraries/glm",
		"../../Libraries/stb_image"
		
	}

	libdirs
	{
		"../../Libraries/glfw-3.3.8.bin.WIN64/lib-vc2015/",
		"../../Libraries/GLEW/lib/Release/x64/"
	}

	links
	{
		"glfw3",
		"glew32s",
		"opengl32.lib"
	}

	defines
	{
		"GLEW_STATIC",
		"_MBCS"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "Off"
		systemversion "latest"

	filter "configurations:Debug"
		defines "DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "RELEASE"
		optimize "On"
	
	filter "platforms:Shared64"
		kind "SharedLib"
		architecture "x64"