create_project("dat-gameboy-test", "ConsoleApp")
includedirs { "%{wks.location}/dat-gameboy-core/src" }
links { "dat-gameboy-core" }

prepare_test()
setup_resources()