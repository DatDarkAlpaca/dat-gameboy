project "tinyfiledialogs"
    kind "StaticLib"
    language "C"
    warnings "off"
    staticruntime "off"
    systemversion "latest"
    
    targetdir(binaries_path .. "/%{prj.name}")
    objdir(intermediate_path .. "/%{prj.name}")

    files {
        "tinyfiledialogs/tinyfiledialogs.c",
        "tinyfiledialogs/tinyfiledialogs.h",
    }

    includedirs {
        "tinyfiledialogs/"
    }
    
    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
    filter { }
    
    filter "configurations:Release"
        runtime "Release"
        optimize "on"
        inlining "auto"
    filter { }
