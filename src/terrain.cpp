#include "terrain.hpp"

void Terrain::generate()
{
    for (int z = 0; z <= grid_size_z; z++)
    {
        for (int x = 0; x <= grid_size_x; x++)
        {
            float x_ = (float)(x - grid_size_x/2) / grid_size_x;
            float z_ = (float)(z - grid_size_z/2) / grid_size_z;
            float y_ = 0;

            vertices.push_back(x_);
            vertices.push_back(y_);
            vertices.push_back(z_);
        }
        
    }

    int vert = 0;
    for (int z = 0; z < grid_size_z; z++)
    {
        for (int x = 0; x < grid_size_x; x++)
        {
            indices.push_back(vert);
            indices.push_back(vert + grid_size_x + 1);
            indices.push_back(vert + 1);
            indices.push_back(vert + 1);
            indices.push_back(vert + grid_size_x + 1);
            indices.push_back(vert + grid_size_x + 2);
            vert++;
        }
        vert++;
    }
}

std::vector<float> Terrain::getVertices()
{
    return vertices; 
}

std::vector<unsigned int> Terrain::getIndices()
{
    return indices;
}
