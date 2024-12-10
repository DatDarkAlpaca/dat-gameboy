project "googletest"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"

    targetdir(binaries_path .. "/%{prj.name}")
    objdir(intermediate_path .. "/%{prj.name}")

    files {
        "googletest/googletest/**.hpp",
        "googletest/googletest/**.h",
		"googletest/googletest/src/gtest-all.cc"
    }
    
    includedirs {
        "%{prj.location}",
        "%{prj.location}/src",
		"googletest/googletest/include/",
		"googletest/googletest/"
    }

    -- Configurations
    filter { "configurations:Debug" }
        runtime "Debug"
        symbols "on"
    filter { }
        
    filter { "configurations:Release" }
        runtime "Release"
        optimize "on"
        inlining "auto"
    filter { }
