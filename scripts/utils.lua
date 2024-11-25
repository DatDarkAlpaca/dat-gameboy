test_folder_path = "%{vendor_path}/gameboy_tests/v2/"

function setup_resources()
    postbuildcommands "{COPYDIR} \"%{wks.location}res\" \"%{binaries_path}/%{prj.name}/res\""
	postbuildcommands "{COPYDIR} \"%{wks.location}res\" \"%{prj.location}/res\""
end

function prepare_test()
	postbuildcommands "{COPYDIR} %{test_folder_path} %{resources_path}/v2/"
	postbuildcommands "{COPYDIR} %{wks.location}/roms %{resources_path}/roms/"
end