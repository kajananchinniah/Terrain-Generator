#pragma once 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "terrain.hpp"

#include <iostream>

class TerrainManager
{
    public:
        static TerrainManager* getInstance();
        void draw(const glm::vec3&, const glm::mat4&, const glm::mat4&);

    private:
        static TerrainManager *terrain_manager_instance;
        TerrainManager();

        void setupRender(Terrain*, float, float, unsigned int&);

        const static unsigned int GRID_SIZE_X = 30;
        const static unsigned int GRID_SIZE_Z = 30;

        // DO NOT MODIFY THESE 3 PARAMETERS 
        const static unsigned int NUM_TERRAINS = 9;
        const static unsigned int NUM_TERRAINS_SQRT = 3;
        const static unsigned int NUM_VBO_MODES = 3;

        Terrain *terrains[NUM_TERRAINS];
        unsigned int terrain_VAOs[NUM_TERRAINS], terrain_VBOs[NUM_VBO_MODES], EBO;
        /*
         * VAO: 0 - terrain 0, 1 - terrain 1, etc 
         * VBO: 0 - vertices, 1 - colours, 2 - normals 
         */

        std::vector<float> terrain_vertices;
        std::vector<unsigned int> terrain_indices;
        std::vector<float> terrain_normals;
        std::vector<float> terrain_colours;

        // Shader 
        Shader *terrain_shader_ptr;
        const char* TERRAIN_VERTEX_SHADER = "shaders/terrain.vs";
        const char* TERRAIN_FRAGMENT_SHADER = "shaders/terrain.fs";
 
};
