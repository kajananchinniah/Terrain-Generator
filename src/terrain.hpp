#pragma once

#include <libnoise/noise.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <iostream>
#include <math.h>

class Terrain
{
    public:
        void generate();
        
        std::vector<float> getVertices();
        std::vector<unsigned int> getIndices();
        std::vector<float> getColours();
        std::vector<float> getLightingNormals();

        int getGridSizeX();
        int getGridSizeZ();

    private:
        // Generate colour 
        void generateColourAt(float y);

        // Noise function
        float noise(float, float);

        // Parameters that should be used for opengl drawing 
        std::vector<float> vertices;
        std::vector<unsigned int> indices; 
        std::vector<float> colours;
        std::vector<float> normals;

        // Parameters related to grid size of terrain 
        int grid_size_x = 20;
        int grid_size_z = 20;

        // Parameters related to noise generation 
        noise::module::Perlin perlin;
        float freq_1 = 1.00f; 
        float freq_2 = 2.00f;
        float freq_3 = 4.00f;

        float octave_1 = 0.15f;
        float octave_2 = 1.00f;
        float octave_3 = 0.11f;

        float exponent = 7.00f; 
};

