# MagicCube
Great homework for Fundamentals of Programming course.

![](https://raw.githubusercontent.com/twd2/MagicCube/master/Doc/image.png)

# Dependencies

1. [GLEW](http://glew.sourceforge.net/)
2. [GLFW](http://www.glfw.org/)

# Building

## Windows - Visual Studio

1. Open `MagicCube.sln`.
2. Configure the include directories and libraries directories.
3. Add dependencies `glu32.lib;opengl32.lib;glfw3dll.lib` to the linker.
4. Build!

##OS X

1. Build glfw (BUILD\_SHARED\_LIBS=OFF).
2. Put `libglfw3.a` into `lib` (you need create this directory before).
3. Create a directory named `build`.
4. Execute `make` and `build/MagicCube` is the binary.