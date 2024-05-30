
project "box2d"
    kind "StaticLib"
    language "C++"

    targetdir(targetDir)
    objdir(objDir)

    files
    {
        "box2d.lua",
        "include/**.h",
        "src/**.h",
        "src/**.cpp"
    }

    includedirs 
    {
        "include",
        "src"
    }