create_project("dat-gameboy", "ConsoleApp")

includedirs { 
	"%{wks.location}/dat-gameboy-core/src" 
}
links { 
	"dat-gameboy-core",
}

setup_vendors()
setup_resources()