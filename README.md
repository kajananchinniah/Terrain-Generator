# Terrain-Generator 
This is a terrain generator implemented in C++ using OpenGL. Perlin noise (via libnoise) is used to generate the heighmaps. I chose perlin noise because it's a common method of height map generation, and produces coherent noise dependent on position only. In order to emulate infinite terrain, a 3x3 grid is created around the player that updates in real time.  

In insights, the number of additions / deletions look insanely high because libraries were commit by accident.

## Screenshots / Videos 
Note: this gif contains an example where lighting is disabled. The version commit to the repo uses lighting
![](resources/Sample_Video.gif)

Here is an example using lighting (prior to fixing it):
![](resources/Sample_Video_Lighting.gif)

Note both gifs contain some strange artifacts for some reason.

## Prerequisites
These instructions are specific towards Ubuntu. Specifically, Ubuntu 18.04. Note they have not been tested yet.

First clone the repo wherever you want. 

Now, install openGL. This can be done using the following command (I think, it's been a while).
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

In this repo, I include camera.h and shader.h in include. Note that I did not write these, and they were taken from learnopengl (https://learnopengl.com/). The camera.h is slightly modified to allow for moving directly up and down.

## Getting Started 
After having all the prerequisites, compile using: `make TerrainGenerator.sh`. You can remove the file afterwards manually or use `make clean` to cleanup your workspace. Since only one file is generated, either approach is fine. 

In terms of controls, the standard wasd is used to move. Using those keys, you will move in the direction the camera is facing. q is used to go up and e is to go down in the y-direction only.

Note: this has only been tested on Ubuntu 18.04. 

## Details to Note 
If you plan on using the skybox class or the terrain_manager class, first make sure to initialize opengl and then use it. To avoid initialization, use a pointer to the class and then initialization it when necessary. See window.cpp's init function for an example. 

Currently stb_image.h is associated with the skybox class (see skybox.cpp). If you include it elsewhere, you might run into compiling issues. I likely won't do this yet, but in the future, creating an image class that uses stb_image.h would be a good idea. This way textures can be added to other resources when necessary. 

## Limitations 
There are a few major limitations associated with my implementation. These are outlined here. 
- With certain viewing distances, it's possible to see the terrain generate unrealistically (e.g. the terrain will generate instantly). This can be avoided by limiting the camera viewing distance to less than equal to the smallest grid size. 
- I just clear the vectors but since the vectors are reused and should be a fixed size (for a given grid size), I could just reuse them as fixed arrays instead of vectors. Vectors were easier to work with so that's why I went with it.
- I regenerate each of the terrains each time, but some of the terrains within the 3x3 grid should be reusable. 
- No instance culling & LOD optimizations. Basically, chunks that don't need to be rendered are being rendered at the same detail as close objects.

## Potential Plans in the Near Future
These are some features that I think would be nice to add eventually, but I won't be doing this in the short term at least. 
- Add a toon like shading or use textures to make the terrains look more life like 
- Generate realistic water rather than static water 
- Optimize terrain generation (fix the limitations and any other bottlenecks) 

## Acknowledgements / References 
To learn OpenGL, I used this resource: https://learnopengl.com/. I also took the camera.h, shader.h and some Window methods (processInput, framebuffer_size_callback, mouse_callback). My implementation for my terrain shaders are also heavily based on the examples found in this book with minor adjustments made to make them work for non-textured objects. My implementation of the skybox class is basically all the skybox associated functions from here, but in a class. I also took the skybox shaders directly from here without modification.

For getting started with terrain generation, I consulted this great resource by Brackeys: https://www.youtube.com/watch?v=64NblGkAabk . In particular, this helped me with vertex and index generation. 

For creating my terrain generation algorithms, I consulted this resource: https://www.redblobgames.com/maps/terrain-from-noise . This contains basically everything you need to know about basic terrain generation from a high level. 

For computing normals, I used this resource: https://stackoverflow.com/questions/13983189/opengl-how-to-calculate-normals-in-a-terrain-height-grid 
I used this resource as well: https://community.khronos.org/t/lighting-terrain-issue-diffuse-and-spec-because-of-normals/105510/5 (this helped fix lighting issues)

I used this tool to generate my skybox: https://github.com/wwwtyro/space-3d/
