function setup_resources()
    postbuildcommands "{COPYDIR} \"%{wks.location}res\" \"%{binaries_path}/%{prj.name}/res\""
	postbuildcommands "{COPYDIR} \"%{wks.location}res\" \"%{prj.location}/res\""
end