#include "window.hpp"

Window* Window::window_instance = NULL;

Window* Window::getInstance()
{
    if (!Window::window_instance)
    {
        Window::window_instance = new Window();
    }
    return Window::window_instance;
}

Window::Window()
{
    // Leave this blank 
}

int Window::init()
{
    // Initialize GLFW 
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

    // Set callback functions for resizing window and moving around mouse 
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    // Enable mouse use 
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to start GLAD\n";
        return -1;
    }


    window_instance->skybox_ptr = skybox_ptr->getInstance();
    window_instance->terrain_manager_ptr = terrain_manager_ptr->getInstance();

    // Initialize some parameters 
    window_instance->camera = Camera(glm::vec3(0.0f, 1.0f, 0.0f));
    window_instance->last_x = WINDOW_WIDTH / 2.0f;
    window_instance->last_y = WINDOW_HEIGHT / 2.0f;
    window_instance->first_mouse = true;
    window_instance->delta_time = 0.0f;
    window_instance->last_frame = 0.0f;
    window_instance->camera_near = 0.1f;
    window_instance->camera_far = std::min(window_instance->terrain_manager_ptr->getGridSizeX(), window_instance->terrain_manager_ptr->getGridSizeX());

    // Enable depth 
    glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    return 0;
}

int Window::shouldClose()
{
    return glfwWindowShouldClose(window);
}

void Window::render()
{
    float current_frame = glfwGetTime();
    delta_time = current_frame - last_frame;
    last_frame = current_frame; 


    processInput(window);

    glClearColor(0.3f, 0.2f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, camera_near, camera_far);

    glm::mat4 view = camera.GetViewMatrix();

    terrain_manager_ptr->draw(camera.Position, view, projection);
    skybox_ptr->draw(view, projection);
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Window::processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(FORWARD, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(BACKWARD, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(LEFT, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(RIGHT, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(UP, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(DOWN, delta_time);
    }
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void Window::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (window_instance->first_mouse)
    {
        window_instance->last_x = xpos;
        window_instance->last_y = ypos;
        window_instance->first_mouse = false;
    }

    float xoffset = xpos - window_instance->last_x;
    float yoffset = window_instance->last_y - ypos; // reversed since y-coordinates go from bottom to top

    window_instance->last_x = xpos;
    window_instance->last_y = ypos;

    window_instance->camera.ProcessMouseMovement(xoffset, yoffset);
}
