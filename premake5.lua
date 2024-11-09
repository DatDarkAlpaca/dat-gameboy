require "scripts/utils"
require "scripts/clear"
require "scripts/project"
require "scripts/platform"
require "vendor/dependencies"

workspace "dat-gameboy"
    architecture "x64"
    configurations {
        "Debug",
        "Release"
    }

    startproject "dat-gameboy-testbed"

    flags {
		"MultiProcessorCompile"
	}

output_path         = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
binaries_path       = "%{wks.location}build/bin/" .. "%{output_path}"
intermediate_path   = "%{wks.location}build/intermediate/" .. "%{output_path}"
vendor_path         = "%{wks.location}/vendor/"

group "dependencies"
	include "vendor/glfw.lua"
    include "vendor/glad.lua"
    include "vendor/imgui.lua"
	include "vendor/imgui-glfw.lua"
    include "vendor/imgui-opengl.lua"
    include "vendor/googletest.lua"
group ""

include "dat-gameboy/build.lua"
include "dat-gameboy-test/build.lua"
include "dat-gameboy-testbed/build.lua"
