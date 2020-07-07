#pragma once 

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "terrain.hpp"

#include <iostream>

class TerrainManager
{
    public:

    private:
        Terrain terrains[9];
        unsigned int terrain_VAOs[9], terrain_VBOs[3], EBO;
        /*
         * VAO: 0 - terrain 0, 1 - terrain 1, etc 
         * VBO: 0 - vertices, 1 - colours, 2 - normals 
         */

        std::vector<float> terrain_vertices;
        std::vector<unsigned int> terrain_indices;
        std::vector<float> terrain_normals;
        std::vector<float> terrain_colours;

        Shader *terrain_shader_ptr;
};
