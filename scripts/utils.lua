test_folder_path = "%{vendor_path}/gameboy_tests/v2/"

function prepare_test()
	postbuildcommands "{COPYDIR} %{test_folder_path} %{resources_path}/tests/"
end

function setup_resources()
	postbuildcommands "py %{scripts_path}/resources.py"
end