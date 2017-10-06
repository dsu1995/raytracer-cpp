C++ Ray Tracer

Sample images: https://dsu1995.github.io/raytracer-cpp


## Features

* Primitives: Sphere, cube, cone, cylinder, mesh
* Supersampling
* Constructive Solid Geometry
* Texture Mapping
* Bump Mapping
* Reflection
* Refraction
* Glossy reflection/transmission
* Soft shadows
* Grid acceleration 

---

## Dependencies
* OpenGL 3.2+
* GLFW
    * http://www.glfw.org/
* Lua
    * http://www.lua.org/
* Premake4
    * https://github.com/premake/premake-4.x/wiki
    * http://premake.github.io/download.html
* GLM
    * http://glm.g-truc.net/0.9.7/index.html
* ImGui
    * https://github.com/ocornut/imgui


---

## Building the Project
In root directory, compile dependencies:

    $ premake4 gmake
    $ make

Then, compile the ray tracer:

    $ cd tracer
    $ premake4 gmake
    $ make


