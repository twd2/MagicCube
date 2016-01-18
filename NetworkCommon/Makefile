PROJECT = NetworkCommon

SOURCES := $(shell ls *.cpp)
OBJECTS := $(patsubst %.cpp, build/%.o, $(SOURCES))

# compiler
INC_DIR = ../include/osx
CC_FLAGS = -O2 -Wall -fno-strict-aliasing -std=c++11 -I $(INC_DIR) -I ../include

.PHONY: all
all:
	-mkdir build
	make lib$(PROJECT).a

build/%.o: %.cpp %.h NetworkConfig.h
	$(CC) -c $(CC_FLAGS) -o $@ $<

lib$(PROJECT).a: $(OBJECTS)
	$(AR) rcs build/lib$(PROJECT).a $(OBJECTS)

.PHONY: clean
clean:
	-rm build/*
	-rm -rf build
