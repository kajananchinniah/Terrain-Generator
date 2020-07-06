# Terrain-Generator 
This is a terrain generator implemented in C++ using OpenGL. Perlin noise (via libnoise) is used to generate the heighmaps. I chose perlin noise because it's a common method of height map generation, and produces coherent noise dependent on position only. In order to emulate infinite terrain, a 3x3 grid is created around the player that updates in real time. 

## Prerequisites
These instructions are specific towards Ubuntu. Specifically, they have been tested on Ubuntu 18.04. 

First, install openGL. This can be done using the following command (I think, it's been a while).
```
sudo apt-get install mesa-utils
```

Now install glfw using the following command.
```
sudo apt-get install libglfw3-dev libglfw3 
```

Now install libnoise using the following command.
```
sudo apt-get install libnoise-dev
```

Note that for this step, there is a better way to install it using instructions found here: http://libnoise.sourceforge.net/index.html . If you choose this alternative, make sure that you modify `#include <libnoise/noise.h` in terrain.hpp into `#include <noise/noise.h>`, remove the `-lnoise` flag in the makefile and modify it to compile with however you setup libnoise.


After that's done, go to http://glad.dav1d.de/ and leave everything as the default except the following options:
- `Language`: C++
- `API section`: Make sure the version is at least 3.3 or higher
- `profile`: Core
- `Generate a loader`: Should be ticked 


Now click `Generate` and download the generated zip file. Place the folders inside the include folder of the zip files inside `include` of this project. Afterwards, place `glad.c` inside the `src` folder of this project. 

Now, you will need glm. Simply download the header files found here: https://glm.g-truc.net/0.9.8/index.html and then place them in `include` of this project.

The final step is downloading `stb_image.h` from https://github.com/nothings/stb/blob/master/stb_image.h and placing it in `include` of this project.

Now, you should be ready to compile this project. Note that I haven't tested this yet, since I have all the dependencies installed. 

Also note: I include some libraries that I didn't write in the include (this is bad practice but I did it anyways. These will be removed once the project is more stablized). 

## Getting Started 
After having all the prerequisites, Simply clone the repository and compile using: `make TerrainGenerator.sh`. Note: this has only been tested on Ubuntu 18.04. 

## TODO
- Clean up code (I hope to make a terrain manager class that deals with all the 9 terrains I generate)
- Add a skybox (likely will be following a tutorial to do this)
- Figure out why the lighting looks really bad 

## Acknowledgements / References 
To learn OpenGL, I used this resource: https://learnopengl.com/. I also took the camera.h, shader.h and some Window methods (processInput, framebuffer_size_callback, mouse_callback). My implementation for my shaders are also heavily based on the examples found in this book. I likely will be taking my skybox code from here as well once it's ready. 

For getting started with terrain generation, I consulted this great resource by Brackeys: https://www.youtube.com/watch?v=64NblGkAabk . In particular, this helped me with vertex and index generation. 

For creating my terrain generation algorithms, I consulted this resource: https://www.redblobgames.com/maps/terrain-from-noise . This contains basically everything you need to know about basic terrain generation from a high level. 

For computing normals, I used these two resources: http://www.lighthouse3d.com/opengl/terrain/index.php?normals and https://gamedev.stackexchange.com/questions/174176/flat-shading-does-not-work-correctly-opengl . Note: I haven't used the latter yet, but plan on it once the project is in a more stable form.
