#!lua

includeDirList = { 
    "libs",
    "libs/gl3w",
    "libs/imgui",
    "libs/include"
}

buildOptions = {"-std=c++11"}

-- Get the current OS platform
PLATFORM = os.get()

-- Build glfw3 static library and copy it into <cs488_root>/lib if it is not
-- already present.
if not os.isfile("lib/libglfw3.a") then
    os.chdir("libs/glfw-3.1.1")
    os.mkdir("build")
    os.chdir("build")
    os.execute("cmake ../")
    os.execute("make")
    os.chdir("../../../")
    os.mkdir("lib")
    os.execute("cp libs/glfw-3.1.1/build/src/libglfw3.a lib/")
end

-- Build lua-5.3.1 library and copyt it into <cs488_root>/lib if it is not
-- already present.
if not os.isfile("lib/liblua.a") then
    os.chdir("libs/lua-5.3.1")

    if PLATFORM == "macosx" then
        os.execute("make macosx")
    elseif PLATFORM == "linux" then
        os.execute("make linux")
    elseif PLATFORM == "windows" then
        os.execute("make mingw")
    end

    os.chdir("../../")
    os.execute("cp libs/lua-5.3.1/src/liblua.a lib/")
end


solution "BuildStaticLibs"
    configurations { "Debug", "Release" }

    configuration "Debug"
        defines { "DEBUG" }
        flags { "Symbols" }

    configuration "Release"
        defines { "NDEBUG" }
        flags { "Optimize" }

    -- Build imgui static library
    project "imgui"
        kind "StaticLib"
        language "C++"
        location "build"
        objdir "build"
        targetdir "lib"
        includedirs (includeDirList)
        includedirs {
            "libs/imgui/examples/opengl3_example",
            "libs/imgui/examples/libs/gl3w/",
        }
        files { 
            "libs/imgui/*.cpp",
            "libs/gl3w/GL/gl3w.c"
        }

    -- Build lodepng static library
    project "lodepng"
        kind "StaticLib"
        language "C++"
        location "build"
        objdir "build"
        targetdir "lib"
        includedirs (includeDirList)
        includedirs {
            "libs/lodepng"
        }
        files { 
            "libs/lodepng/lodepng.cpp"
        }
