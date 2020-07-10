#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "camera.h"
#include "shader.h"

#include "window.hpp"
#include "terrain.hpp"

int main()
{
    int success = Window::getInstance()->init();
    if (success == -1)
    {
        return -1;
    }

    while (!Window::getInstance()->shouldClose())
    {
        Window::getInstance()->render();
    }


    return 0;
}
