PROJECT = CubeCommon

SOURCES := $(shell ls *.cpp)
OBJECTS := $(patsubst %.cpp, build/%.o, $(SOURCES))

# compiler
INC_DIR = 
CC_FLAGS = -O2 -Wall -std=c++11

.PHONY: all
all:
	-mkdir build
	make lib$(PROJECT).a

build/%.o: %.cpp %.h CubeTypes.h utilities.h
	$(CC) -c $(CC_FLAGS) -o $@ $<

lib$(PROJECT).a: $(OBJECTS)
	$(AR) rcs build/lib$(PROJECT).a $(OBJECTS)

.PHONY: clean
clean:
	-rm build/*
	-rm -rf build
