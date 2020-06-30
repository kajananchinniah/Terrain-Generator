#include "terrain.hpp"

void Terrain::generate()
{
    for (int z = 0; z <= grid_size_z; z++)
    {
        for (int x = 0; x <= grid_size_x; x++)
        {
            float x_ = (float)(x - grid_size_x/2) / grid_size_x;
            float z_ = (float)(z - grid_size_z/2) / grid_size_z;
            
            float y_ = octave_1 * noise(freq_1 * x_, freq_2 * z_);
            y_ += octave_2 * noise(freq_2 * x_, freq_2 * z_);
            y_ += octave_3 * noise(freq_3 * x_, freq_3 * z_); 
            y_ = pow(y_, exponent);
            float y = y_;

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
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

float Terrain::noise(float x, float y)
{
    // Ensures value is between 0 - 1 
    return perlin.GetValue(x, y, 0) / 2.0 + 0.5; 
}
