PROJECT = MagicCube

SOURCES := $(shell ls $(PROJECT)/*.cpp)
OBJECTS := $(patsubst $(PROJECT)/%.cpp, build/%.o, $(SOURCES))

#cc

INC_DIR = $(PROJECT)/include

CC_FLAGS = -O2 -Wall -std=c++11 -I $(INC_DIR)

#linker

LIB_DIR = lib

LIBS = -lc++ -lm -lSystem -lglfw3 -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo

LINKER_FLAGS = -L $(LIB_DIR) $(LIBS)

.PHONY: all
all:
	-mkdir build
	make $(PROJECT)

$(PROJECT): $(OBJECTS)
	$(CC) $(LINKER_FLAGS) -o build/$(PROJECT) $(OBJECTS)

build/%.o: $(PROJECT)/%.cpp $(PROJECT)/%.h $(PROJECT)/Config.h
	$(CC) -c $(CC_FLAGS) -o $@ $<

.PHONY: clean
clean:
	-rm build/*
	-rm -rf build
