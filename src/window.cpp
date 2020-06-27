#include "window.hpp"

Window* Window::instance = NULL;

Window* Window::getInstance()
{
    if (!Window::instance)
    {
        Window::instance = new Window;
    }
    return Window::instance;
}

Window::Window()
{
    instance->camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
    instance->last_x = WINDOW_WIDTH / 2.0f;
    instance->last_y = WINDOW_HEIGHT / 2.0f;
    instance->first_mouse = true;
    instance->delta_time = 0.0f;
    instance->last_frame = 0.0f; 
}

int Window::init()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create window\n";
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to start GLAD\n";
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    return 0;
}

int Window::shouldClose()
{
    return glfwWindowShouldClose(window);
}

void Window::render()
{
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Window::processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, delta_time);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, delta_time);
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void Window::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (instance->first_mouse)
    {
        instance->last_x = xpos;
        instance->last_y = ypos;
        instance->first_mouse = false;
    }

    float xoffset = xpos - instance->last_x;
    float yoffset = instance->last_y - ypos; // reversed since y-coordinates go from bottom to top

    instance->last_x = xpos;
    instance->last_y = ypos;

    instance->camera.ProcessMouseMovement(xoffset, yoffset);
}