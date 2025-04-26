project "Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    cdialect "C17"
    targetdir "/Build/Out/%{prj.name}/%{cfg.buildcfg}/"
    objdir "/Build/Int/%{prj.name}/%{cfg.buildcfg}/"
    location "./"

    files { "./**.h", "./**.cpp" }
    includedirs { _WORKING_DIR, _WORKING_DIR .. "/DuckCore/", _WORKING_DIR .. "/DuckCore/External/" }
    links { }

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
