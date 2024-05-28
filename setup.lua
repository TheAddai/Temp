
targetDir = "%{wks.location}/bin/%{prj.name}"
objDir = "%{wks.location}/bin-int"

include "vendor.lua"

workspace "Prime"
    startproject "editor"

    configurations
    {
        "Debug",
        "Release"
    }

    flags 
    {
        "MultiProcessorCompile",
        "FatalWarnings"
    }

    filter {"system:windows", "configurations:*"}
        architecture "x64"
        systemversion "latest"
        cppdialect "C++17"
        staticruntime "on"

    filter "configurations:Debug"
        symbols "on"
        runtime "Debug"

    filter "configurations:Release"
        symbols "off"
        runtime "Release"
        optimize "full"


    include "engine/engine.lua"
    include "editor/editor.lua"

    group "vendor"
        include "vendor/glfw/glfw.lua"
    group ""