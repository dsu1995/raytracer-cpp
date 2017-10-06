#!lua

includeDirList = {
    "../libs",
    "../libs/include",
    "../libs/gl3w",
    "../libs/imgui"
}

libDirectories = { 
    "../lib"
}


if os.get() == "macosx" then
    linkLibs = {
        "imgui",
        "glfw3",
        "lua",
		"lodepng"
    }
end

if os.get() == "linux" then
    linkLibs = {
        "imgui",
        "glfw3",
        "lua",
        "lodepng",
        "GL",
        "Xinerama",
        "Xcursor",
        "Xxf86vm",
        "Xi",
        "Xrandr",
        "X11",
        "stdc++",
        "dl",
        "pthread"
    }
end

if os.get() == "macosx" then
    linkOptionList = { "-framework IOKit", "-framework Cocoa", "-framework CoreVideo", "-framework OpenGL" }
end

if os.get() == "linux" then
    linkOptionList = { "-fopenmp" }
end

buildOptions = {"-std=c++11 -O2 -fopenmp"}

solution "raytracer-cpp"
    configurations { "Debug", "Release" }

    project "Tracer"
        kind "ConsoleApp"
        language "C++"
        location "build"
        objdir "build"
        targetdir "."
        buildoptions (buildOptions)
        libdirs (libDirectories)
        links (linkLibs)
        linkoptions (linkOptionList)
        includedirs (includeDirList)
        files { "**.cpp" }
        excludes { "Assets/**", "build/**", "cmake-build-debug/**" }

    configuration "Debug"
        defines { "DEBUG" }
        flags { "Symbols" }

    configuration "Release"
        defines { "NDEBUG" }
        flags { "Optimize" }
