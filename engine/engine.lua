
project "engine"
    language "C++"
    kind "StaticLib"

    targetdir(targetDir)
    objdir(objDir)

    pchheader "pch.h"
	pchsource "src/pch.cpp"

    files
    {
        "engine.lua",
        "src/**.h",
        "src/**.cpp"
    }

    includedirs 
    {
        "src",
    }

    defines
    {
        "GLFW_INCLUDE_NONE"
    }

    externalincludedirs
    {
        "%{includeDirs.spdlog}",
        "%{includeDirs.glfw}",
        "%{includeDirs.glad}"
    }

    links
    {
        "glfw",
        "glad"
    }