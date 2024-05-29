
project "yaml"
	kind "StaticLib"
	language "C++"

	targetdir(targetDir)
    objdir(objDir)

	files
	{
		"yaml.lua",
		"src/**.h",
		"src/**.cpp",
		
		"include/**.h"
	}

	includedirs
	{
		"include"
	}