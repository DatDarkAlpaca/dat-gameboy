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

    startproject "dat-gameboy"

    flags {
		"MultiProcessorCompile"
	}

output_path         = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
binaries_path       = "%{wks.location}/build/bin/" .. "%{output_path}"
intermediate_path   = "%{wks.location}/build/intermediate/" .. "%{output_path}"
vendor_path         = "%{wks.location}/vendor/"
resources_path      = "%{wks.location}/res/"
scripts_path        = "%{wks.location}/scripts/"

group "dependencies"
	include "vendor/tinyfiledialogs.lua"
	include "vendor/glfw.lua"
    include "vendor/glad.lua"
    include "vendor/imgui.lua"
	include "vendor/imgui-glfw.lua"
    include "vendor/imgui-opengl.lua"
    include "vendor/googletest.lua"
group ""

include "dat-gameboy-core/build.lua"
include "dat-gameboy-test/build.lua"
include "dat-gameboy/build.lua"
