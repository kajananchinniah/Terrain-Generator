# set the compiler 
CC := g++

# set any compiler flags 
CFLAGS := -lglfw -ldl -lnoise 

# include directory here
INCL := -I include/ -I include/glm 

# add header files here 
HDRS := src/window.hpp src/terrain.hpp src/skybox.hpp 

# add source files here
SRCS := src/main.cpp src/window.cpp src/terrain.cpp src/glad.c src/skybox.cpp

# name of executable
EXEC := TerrainGenerator.sh 

# default recipe
all: $(EXEC)

# recipe for building the final executable
$(EXEC): makefile 
		$(CC) $(SRCS) -o $@ $(INCL) $(CFLAGS)

# clean workspace 
clean:
	rm -f $(EXEC)

.PHONY: all clean
