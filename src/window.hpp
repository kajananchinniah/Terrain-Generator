#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "terrain.hpp"

#include <iostream>

class Window
{
    public:
        static Window* getInstance(); 
        int init();
        int shouldClose();
        void setupRender();
        void render();

    private:
        static Window* instance;
        Window();
        GLFWwindow *window; 
        const unsigned int WINDOW_WIDTH = 800;
        const unsigned int WINDOW_HEIGHT = 600;
        const char* WINDOW_TITLE = "Terrain Generator";

        Terrain terrain; 

        unsigned int VAO, EBO;
        unsigned int VBO[3]; // 0 - vertices, 1 - colours, 2 - normals 
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        std::vector<float> normals;
        std::vector<float> colours;

        //Shader 
        Shader *shader_ptr; 

        // Camera 
        Camera camera;
        float last_x;
        float last_y;
        bool first_mouse;
        float camera_near;
        float camera_far;

        // Timing 
        float delta_time;
        float last_frame;

        // Process Input 
        void processInput(GLFWwindow *window);

        //Callback functions 
        static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
        static void mouse_callback(GLFWwindow *window, double xpos, double ypos);
        
};

