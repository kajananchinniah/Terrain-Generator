#pragma once 
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <vector>

#include "shader.h"
#include "camera.h"

class Skybox
{
    public:
        Skybox();
        void draw(const glm::mat4&, const glm::mat4&);
        void done();

    private:
        void setupRender(); 
        void loadCubemap(); 

        unsigned int cube_map_texture;

        Shader* skybox_shader; 

        unsigned int skybox_VAO, skybox_VBO;

        std::vector<float> skybox_vertices;
        std::vector<std::string> skybox_faces; 
};
