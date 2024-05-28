
project "imgui"
	kind "StaticLib"
	language "C++"

	targetdir(targetDir)
    objdir(objDir)

	files
	{
		"imgui.lua",
		"include/imgui/imconfig.h",
		"include/imgui/imgui.h",
		"include/imgui/imgui_internal.h",
		"include/imgui/imstb_rectpack.h",
		"include/imgui/imstb_textedit.h",
		"include/imgui/imstb_truetype.h",

		"src/imgui.cpp",
		"src/imgui_draw.cpp",
		"src/imgui_widgets.cpp",
		"src/imgui_demo.cpp"
	}

	includedirs
	{
		"include/imgui",
		"src",
		"%{includeDirs.glfw}",
		"%{includeDirs.glad}"
	}

	defines
	{
		IMGUI_IMPL_OPENGL_LOADER_GLAD
	}

	filter "system:windows"
	    files
	    {
			"include/imgui/imgui_impl_dx9.h",
			"include/imgui/imgui_impl_dx10.h",
			"include/imgui/imgui_impl_dx11.h",
			"include/imgui/imgui_impl_dx12.h",

			"src/imgui_impl_dx9.cpp",
			"src/imgui_impl_dx10.cpp",
			"src/imgui_impl_dx11.cpp",
			"src/imgui_impl_dx12.cpp",

			"include/imgui/imgui_impl_glfw.h",
			"include/imgui/imgui_impl_opengl3.h",

			"src/imgui_impl_glfw.cpp",
			"src/imgui_impl_opengl3.cpp"
	    }