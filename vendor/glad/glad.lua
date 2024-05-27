
project "glad"
    kind "StaticLib"
    language "C++"

    targetdir(targetDir)
    objdir(objDir)

    files
    {
        "glad.lua",
        "include/**.h",
        "src/**.c"
    }

    includedirs 
    {
        "include"
    }