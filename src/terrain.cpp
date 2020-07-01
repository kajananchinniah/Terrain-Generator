#include "terrain.hpp"

void Terrain::generate()
{
    // Calculate the vertex positions of all the triangles 
    for (int z = 0; z <= grid_size_z; z++)
    {
        for (int x = 0; x <= grid_size_x; x++)
        {
            float x_ = (float)(x - grid_size_x/2) / grid_size_x;
            float z_ = (float)(z - grid_size_z/2) / grid_size_z;
            
            float y_ = octave_1 * noise(freq_1 * x_, freq_2 * z_);
            y_ += octave_2 * noise(freq_2 * x_, freq_2 * z_);
            y_ += octave_3 * noise(freq_3 * x_, freq_3 * z_); 
           
            // Water 
            if (y_ < 0.1)
            {
                y_ = 0.1;
                colours.push_back(0.00f);
                colours.push_back(0.467f);
                colours.push_back(0.745f);
            }

            // Sand 
            else if (y_ < 0.2)
            {
                colours.push_back(0.761f);
                colours.push_back(0.698f);
                colours.push_back(0.502f);
            }

            // Grass 
            else if (y_ < 0.7)
            {
                colours.push_back(0.486f);
                colours.push_back(0.988f);
                colours.push_back(0.00f);
            }

            // Rock
            else if (y_ 0.9)
            {
                colours.push_back(0.584f);
                colours.push_back(0.580f);
                colours.push_back(0.545f);
            }
            
            // Snow 
            else
            {
                colours.push_back(1.00f);
                colours.push_back(1.00f);
                colours.push_back(1.00f);
            }

            float y = pow(y_, exponent);

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
        
    }

    // Calculate the indices associated with each triangle for drawing 
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


    // TODO: clean this area; it's written messy 
    std::vector<glm::vec3> extracted_vertices;
    for (int i = 0; i < indices.size(); i+=3)
    {
        // Get the indices associated with a triangles
        unsigned int idx_1 = indices[i];
        unsigned int idx_2 = indices[i+1];
        unsigned int idx_3 = indices[i+3];

        // Get the vertices associated with the indices 
        // Multiply by 3 because each vertex has 3 points associated to it 
        unsigned int vertex_pos = 3 * idx_1; 
        glm::vec3 t1 = glm::vec3(vertices[vertex_pos], vertices[vertex_pos + 1], vertices[vertex_pos + 2]);

        vertex_pos = 3 * idx_2;
        glm::vec3 t2 = glm::vec3(vertices[vertex_pos], vertices[vertex_pos + 1], vertices[vertex_pos + 2]);

        vertex_pos = 3 * idx_3;
        glm::vec3 t3 = glm::vec3(vertices[vertex_pos], vertices[vertex_pos + 1], vertices[vertex_pos + 2]);

        glm::vec3 v1 = t2 - t1;
        glm::vec3 v2 = t3 - t1;

        glm::vec3 normal = glm::normalize(glm::cross(v1, v2));
       
        normals.push_back(normal.x); 
        normals.push_back(normal.y);
        normals.push_back(normal.z);
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

std::vector<float> Terrain::getColours()
{
    return colours;
}

std::vector<float> Terrain::getLightingNormals()
{
    return normals;
}

float Terrain::noise(float x, float y)
{
    // Ensures value is between 0 - 1 
    return perlin.GetValue(x, y, 0) / 2.0 + 0.5; 
}
