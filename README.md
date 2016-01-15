# MagicCube
### Great homework for Fundamentals of Programming course.

![](https://raw.githubusercontent.com/twd2/MagicCube/master/Doc/image1.png)

![](https://raw.githubusercontent.com/twd2/MagicCube/master/Doc/image2.png)

# Dependencies

1. Your interest.
2. [GLFW](http://www.glfw.org/) if compile without NOGL option.
3. [libevent](https://github.com/libevent/libevent) 2.0+
4. [RapidJSON](https://github.com/miloyip/rapidjson)
5. [cmdline](https://github.com/tanakh/cmdline) (for server)

## Head Files And Precompiled Libraries

TODO

# Solvers

1. General solver(general): Fast, ~130 steps.
2. Random solver(random, monkey): Very very slow, infinite steps.
3. Brute force solver(bruteforce, bf): Very slow, less than 21 steps.

# Building

## Windows - Visual Studio

0. Put downloaded head files directory (`include`) and precompiled libraries directory (`lib`) into the root of the project directory.
1. Open `MagicCube.sln`.
2. Build!

## OS X & Linux

0. If you are using Linux, install `xorg-dev`, `libgl1-mesa-dev` and `libglu1-mesa-dev` packages.
1. Build glfw and libevent, and make static libraries.
2. Put `libglfw3.a` and `libevent.a` into `lib/osx` or `lib/linux` (you may need create this directory before).
3. Execute `make -j8` and `build/MagicCube` is the binary.

# Building - Server

## Windows - Visual Studio

See #Building.

## OS X & Linux

1. Build libevent, and make a static library.
2. Put `libevent.a` into `lib/osx/` or `lib/linux/` (you may need create this directory before).
3. Execute `make -j8 server` and `MagicCubeServer/build/MagicCubeServer` is the binary.

# Data Format

## Format1

see: PROBLEM.md

## Format2

按“前、后、左、右、上、下”的次序来输入魔方的各面状态，其中“后左右”三面是“前”面保持上下面在同一水平面上分别旋转180度、90度、270度得到的，“上下”两面分别是“前”面向下和向上翻转90度（保持左右面在同一垂直平面上）得到的。

每面9个字符，代表颜色。

# Usage - GUI

打开程序之后，会显示一个窗口，窗口里面有一个大方块，上面有27个小方块，其中一个基本看不见。

用鼠标在窗口内拖动可以以不同视角观看魔方。

焦点在窗口上，输入文字，会回显在控制台中。

现在支持如下命令:

	U, Ui, D, Di, L, Li, R, Ri, F, Fi, B, Bi, X, Xi, Y, Yi, Z, Zi: 参考PROBLEM.md中说明，其中“正对魔方”操作应将视角调整为程序打开时视角的初始位置。 
	CHECK: 检查上下左右前后六个面每个小方块的颜色相等情况。
	ABOUT: 输出“Wandai :)”，没有实际用处，但拥有超级牛力。
	RESET: 新魔方。
	SOLVE: 解当前状态的魔方，输出参考步骤，窗口表现为魔方颜色复原。
	PLAY: 解当前状态的魔方，输出参考步骤，窗口表现为魔方颜色复原每一步的动画。
	SET_SOLVER <solver_name>: 设置求解算法(general/bruteforce/random)。
	RANDOM: 随机转动魔方，“打乱”。
	FILE <filename>: 从文件<filename>加载魔方表示。
	LOAD CMD: 从控制台输入魔方表示并加载。
	LOAD <data>: 加载魔方表示数据<data>。
	LOAD FORMAT2: 从控制台输入format2表示的魔方。
	SAVE: 保存当前魔方装态为魔方表示。
	TEST: 测试(不好玩)。

# Usage - Text mode

1. Input cube data in format2.
2. Press `Enter`.
3. See the steps.
