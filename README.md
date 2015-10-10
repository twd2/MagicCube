# MagicCube
Great homework for Fundamentals of Programming course.

![](https://raw.githubusercontent.com/twd2/MagicCube/master/Doc/image.png)

# Dependencies

1. [GLFW](http://www.glfw.org/)

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

# Usage

不好意思其实我不会说英语。

如果是NOGL编译，那么该程序为PROBLEM.md中描述的题目的一个解答。

否则如下。

打开程序之后，会显示一个窗口，窗口里面有一个大方块，上面有27个小方块，其中一个基本看不见。

用鼠标在窗口内拖动可以以不同视角观看魔方。

焦点在窗口上，输入文字，会回显在控制台中。

现在支持如下命令:

	U, Ui, D, Di, L, Li, R, Ri, F, Fi, B, Bi, X, Xi, Y, Yi, Z, Zi: 参考PROBLEM.md中说明，其中“正对魔方”操作应将视角调整为程序打开时视角的初始位置。 
	CHECK: 检查上下左右前后六个面每个小方块的颜色相等情况。
	ABOUT: 输出“Wandai :)”，没有实际用处，但拥有超级牛力。
	SOLVE: 解当前状态的魔方，输出参考步骤，窗口表现为魔方颜色复原。
	PLAY: 解当前状态的魔方，输出参考步骤，窗口表现为魔方颜色复原每一步的动画。
	RANDOM: 随机转动魔方，“打乱”。
	FILE <filename>: 从文件<filename>加载魔方表示。
	LOAD CMD: 从控制台输入魔方表示并加载。
	LOAD <data>: 加载魔方表示数据<data>。
	SAVE: 保存当前魔方装态为魔方表示。
	TEST: 测试(不好玩)。