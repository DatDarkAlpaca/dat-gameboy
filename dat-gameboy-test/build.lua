create_project("dat-gameboy-test", "ConsoleApp")
includedirs { "%{wks.location}/dat-gameboy/src" }
links { "dat-gameboy" }

prepare_test()
setup_resources()