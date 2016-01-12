PROJECT = MagicCubeServer

SOURCES := $(shell ls *.cpp)
OBJECTS := $(patsubst %.cpp, build/%.o, $(SOURCES))
ifeq ($(shell uname),Linux)
	# compiler
	CC = g++
	INC_DIR = ../include/osx
	CC_FLAGS = -g -O2 -Wall -std=c++11 -I $(INC_DIR) -I ../include

	# linker
	LD = g++
	LIB_DIR = lib/linux
	LIBS = -lCubeCommon -lstdc++ -lc -lm -ldl -levent -lpthread
	LINKER_FLAGS = -L $(LIB_DIR) -L ../CubeCommon/build
else

ifeq ($(shell uname),Darwin)
# compiler
INC_DIR = ../include/osx
CC_FLAGS = -g -O2 -Wall -std=c++11 -I $(INC_DIR) -I ../include

# linker
LIB_DIR = lib/osx
LIBS = -lCubeCommon -lc++ -lm -lSystem -levent
LINKER_FLAGS = -L $(LIB_DIR) -L ../CubeCommon/build
else
	# Unsupported operating system.
	CC = echo && echo "******** Unsupported operating system! ********" && echo && exit 1 ||
endif

endif

.PHONY: all
all:
	-mkdir build
	make $(PROJECT)

build/%.o: %.cpp %.h Config.h
	$(CC) -c $(CC_FLAGS) -o $@ $<

$(PROJECT): $(OBJECTS)
	$(CC) $(LINKER_FLAGS) -o build/$(PROJECT) $(OBJECTS) $(LIBS)

.PHONY: clean
clean:
	-rm build/*
	-rm -rf build
