#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"

#include <iostream>


class Window
{
    public:
        static Window* getInstance(); 
        int init();
        int shouldClose();
        void render();

    private:
        static Window* instance;
        Window();
        GLFWwindow *window; 
        const unsigned int WINDOW_WIDTH = 800;
        const unsigned int WINDOW_HEIGHT = 600;
        const char* WINDOW_TITLE = "Terrain Generator";


        // Camera 
        Camera camera;
        float last_x;
        float last_y;
        bool first_mouse;

        // Timing 
        float delta_time;
        float last_frame;


        // Process Input 
        void processInput(GLFWwindow *window);

        //Callback functions 
        static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
        static void mouse_callback(GLFWwindow *window, double xpos, double ypos);
        
};

