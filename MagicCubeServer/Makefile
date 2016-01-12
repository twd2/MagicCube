PROJECT = MagicCubeServer

SOURCES := $(shell ls *.cpp)
OBJECTS := $(patsubst %.cpp, build/%.o, $(SOURCES))

# compiler
INC_DIR = ../include/osx
CC_FLAGS = -g -O2 -Wall -std=c++11 -I $(INC_DIR) -I ../include

# linker
LIB_DIR = lib/osx
LIBS = -lc++ -lm -lSystem -levent
LINKER_FLAGS = -L $(LIB_DIR)

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
