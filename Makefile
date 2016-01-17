PROJECT = MagicCube

SOURCES := $(shell ls $(PROJECT)/*.cpp)
OBJECTS := $(patsubst $(PROJECT)/%.cpp, build/%.o, $(SOURCES))

ifeq ($(shell uname),Linux)
	# compiler
	CC = g++
	INC_DIR = include/osx
	CC_FLAGS = -O2 -Wall -fno-strict-aliasing -std=c++11 -I $(INC_DIR) -I include

	# linker
	LD = g++
	LIB_DIR = lib/linux
	LIBS = -lCubeCommon -levent -lglfw3 -lstdc++ -lc -lm -ldl -lX11 -lpthread -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor -lGL -lGLU
	LINKER_FLAGS = -L $(LIB_DIR) -L CubeCommon/build
else

ifeq ($(shell uname),Darwin)
	# compiler
	INC_DIR = include/osx
	CC_FLAGS = -O2 -Wall -fno-strict-aliasing -std=c++11 -I $(INC_DIR) -I include

	# linker
    LD = cc
	LIB_DIR = lib/osx
	LIBS = -lCubeCommon -levent -lc++ -lm -lSystem -lglfw3 -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo
	LINKER_FLAGS = -L $(LIB_DIR) -L CubeCommon/build
else
	# Unsupported operating system.
	CC = echo && echo "******** Unsupported operating system! ********" && echo && exit 1 ||
endif

endif

.PHONY: client
client:
	-mkdir build
	make common
	make $(PROJECT)

.PHONY: all
all:
	-mkdir build
	make common
	make $(PROJECT)
	make server

.PHONY: common
common: 
	cd CubeCommon && make

.PHONY: server
server: 
	-mkdir build
	make common
	cd MagicCubeServer && make

$(PROJECT): $(OBJECTS)
	$(LD) $(LINKER_FLAGS) -o build/$(PROJECT) $(OBJECTS) $(LIBS)

build/%.o: $(PROJECT)/%.cpp $(PROJECT)/%.h $(PROJECT)/Config.h $(PROJECT)/utilities.h CubeCommon/NetworkConfig.h
	$(CC) -c $(CC_FLAGS) -o $@ $<

.PHONY: release
release: client
	cd build && tar czvf $(PROJECT)_$(shell uname).tar.gz $(PROJECT)

.PHONY: clean
clean:
	-rm build/*
	-rm -rf build
	cd CubeCommon && make clean
	cd MagicCubeServer && make clean
