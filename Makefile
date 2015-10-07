OBJECTS = build/AlgorithmSolver.o build/Input.o build/SolverError.o \
build/Cube.o build/MagicCube.o build/StepReduce.o \
build/CubeError.o build/RandomSolver.o build/Vertices.o \
build/CubeSolver.o build/Rendering.o build/stdafx.o \
build/Graphics.o build/RotationAnimation.o build/utilities.o

MagicCube: $(OBJECTS)
	$(CC) -Llib -o build/MagicCube -lc++ -lSystem -lglfw3 -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo $(OBJECTS)
build/stdafx.o: MagicCube/stdafx.cpp
	$(CC) -IMagicCube/include -o $@ -c $< 
build/%.o: MagicCube/%.cpp
	clang++ -std=c++11 -IMagicCube/include -o $@  -c $<

.PHONY: clean
clean:
	-rm build/*
