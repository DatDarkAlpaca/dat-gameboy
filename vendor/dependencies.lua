vendor_include_dirs = { }
vendor_include_dirs["tinyfiledialogs"]  = "%{vendor_path}/tinyfiledialogs"
vendor_include_dirs["glad"]             = "%{vendor_path}/glad/include"
vendor_include_dirs["glfw"]             = "%{vendor_path}/glfw/include"
vendor_include_dirs["glm"]              = "%{vendor_path}/glm"
vendor_include_dirs["imgui"]            = "%{vendor_path}/imgui"
vendor_include_dirs["imgui-glfw"]       = "%{vendor_path}/imgui/backends"
vendor_include_dirs["imgui-opengl"]     = "%{vendor_path}/imgui/backends"
vendor_include_dirs["spdlog"]           = "%{vendor_path}/spdlog/include"
vendor_include_dirs["nlohmann"]         = "%{vendor_path}/json/single_include"
vendor_include_dirs["stb"]         		= "%{vendor_path}/stb"
vendor_include_dirs["googletest"]       = "%{vendor_path}/googletest/googletest/include"

vendor_lib_dirs = { }

vendor_links = {}
vendor_links[0] = "glad"
vendor_links[1] = "glfw"
vendor_links[2] = "imgui"
vendor_links[3] = "imgui-glfw"
vendor_links[4] = "imgui-opengl"
vendor_links[5] = "googletest"
vendor_links[6] = "tinyfiledialogs"

vendor_defines = {}
vendor_defines[0] = "GLFW_INCLUDE_NONE"

function set_vendor_include_dirs()
    for i, vendor_paths in pairs(vendor_include_dirs) do
        includedirs { vendor_paths }
    end
end

function set_vendor_lib_dirs()
    for i, vendor_paths in pairs(vendor_lib_dirs) do
        libdirs { vendor_paths }
    end
end

function set_vendor_links()
    for i, vendor_name in pairs(vendor_links) do
        links { vendor_name }
    end
end

function set_vendor_defines()
    for i, vendor_define in pairs(vendor_defines) do
        defines { vendor_define }
    end
end

function setup_vendors()
    set_vendor_include_dirs()
	set_vendor_lib_dirs()
	set_vendor_links()
    set_vendor_defines()
end