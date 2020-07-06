# Terrain-Generator 
This is a terrain generator implemented in C++ using OpenGL. 

## Installing
TODO
Note: I include some libraries that I didn't write in the include (this is bad practice but I did it anyways). 

## Getting started 
Simply clone the repository, with any dependencies, and compile it using: `g++ src/main.cpp src/glad.c src/window.cpp src/terrain.cpp -I include/ -I include/glm/ -lglfw -ldl -lnoise`. I plan on adding a makefile later on.

## Things I have to do
- Clean up code (I hope to make a terrain manager class that deals with all the 9 terrains I generate)
- Add a skybox (likely will be following a tutorial to do this)
- Figure out why the lighting looks really bad 


## Acknowledgements / References 
To learn OpenGL, I used this resource: https://learnopengl.com/. I also took the camera.h, shader.h and some Window methods (processInput, framebuffer_size_callback, mouse_callback). My implementation for my shaders are also heavily based on the examples found in this book.  

To learn about terrain generation, I consulted three resources:
- https://www.youtube.com/watch?v=64NblGkAabk 
    - I based the vertex and index generation from this resource 
- https://www.redblobgames.com/maps/terrain-from-noise
-   - This outlined basically everything I needed to know 
- http://www.lighthouse3d.com/opengl/terrain/index.php?normals
-   - This helped me calculate the normals for lighting purposes 
- https://gamedev.stackexchange.com/questions/174176/flat-shading-does-not-work-correctly-opengl
-   - I plan on changing my normal calculation into this
