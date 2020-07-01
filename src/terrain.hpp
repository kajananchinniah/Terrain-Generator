#pragma once

#include <libnoise/noise.h>

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
        float freq_1 = 1; 
        float freq_2 = 3.5;
        float freq_3 = 2.25;

        float octave_1 = 1;
        float octave_2 = 0.75;
        float octave_3 = 0.25;

        float exponent = 1.75; 
};

