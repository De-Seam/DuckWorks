workspace "DuckWorks"
    configurations { "Debug", "Release" }
    location "./"
	startproject "DuckWorks"

    -- Define the platform condition
    if os.istarget("windows") then
        platforms { "x86_64" }  -- Only support win64
    end

	include("DuckCore/Premake5.lua")
	include("Game/Premake5.lua")
	include("Renderer/Premake5.lua")
	include("Engine/Premake5.lua")
	include("Editor/Premake5.lua")

project "DuckWorks"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    cdialect "C17"
    targetdir "/Build/Out/%{prj.name}/%{cfg.buildcfg}/"
    objdir "/Build/Int/%{prj.name}/%{cfg.buildcfg}/"
    location "DuckWorks"

    files { "DuckWorks/**.h", "DuckWorks/**.cpp" }
    includedirs { _WORKING_DIR, _WORKING_DIR .. "/DuckCore/", _WORKING_DIR .. "/DuckCore/External/" }
	libdirs { "DuckCore/External/lib/%{cfg.buildcfg}", "DuckCore/External/lib/Shared" }
    links { "DuckCore", "External", "Renderer", "Engine", "Editor", "Game", "SDL2" }

    filter "configurations:Debug"
        defines { "DEBUG" }
		warnings "Extra"
        symbols "On"
		flags { "MultiProcessorCompile" }
        filter "system:windows"
            buildoptions { "/MP", "/W4" }  -- Enable multi-processor and set warning level to 4

    filter "configurations:Release"
        defines { "NDEBUG" }
		warnings "Extra"
        optimize "On"
		flags { "MultiProcessorCompile" }
        filter "system:windows"
            buildoptions { "/MP", "/W4" }  -- Enable multi-processor and set warning level to 4
