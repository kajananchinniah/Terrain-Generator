#pragma once

#include <vector>
#include <iostream>

class Terrain
{
    public:
        void generate();
        std::vector<float> getVertices();
        std::vector<unsigned int> getIndices();

    private:
        std::vector<float> vertices;
        std::vector<unsigned int> indices; 
        int grid_size_x = 120;
        int grid_size_z = 120;
};

