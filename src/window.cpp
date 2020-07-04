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
    // Leave this blank 
}

int Window::init()
{
    // Initialize constants 
    instance->camera = Camera(glm::vec3(0.0f, 1.0f, 0.0f));
    instance->last_x = WINDOW_WIDTH / 2.0f;
    instance->last_y = WINDOW_HEIGHT / 2.0f;
    instance->first_mouse = true;
    instance->delta_time = 0.0f;
    instance->last_frame = 0.0f;
    instance->camera_near = 0.1f;
    instance->camera_far = 100.0f; //TODO :update these values 

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

    // Enable depth 
    glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Initialize shader class with shaders from before 
    instance->shader_ptr = new Shader("../shaders/vertex.vs", "../shaders/fragment.fs");
   
    // Setup rendering stuff 
    setupRender();
    return 0;
}

int Window::shouldClose()
{
    return glfwWindowShouldClose(window);
}

void Window::setupRender()
{
       // Generate terrain attributes 
       terrain.generate();
       vertices = terrain.getVertices();
       indices = terrain.getIndices();
       colours = terrain.getColours();
       normals = terrain.getLightingNormals();

       // Generate all buffers and VAO necessary 
       glGenVertexArrays(1, &VAO);
       glGenBuffers(3, VBO);
       glGenBuffers(1, &EBO);
   
       // Bind VAO 
       glBindVertexArray(VAO);
  
       // Bind vertices to the first VBO 
       glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
       glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices.front(), GL_STATIC_DRAW);

       // Bind indices to EBO for drawing 
       // Not too sure why but this must be done before doing things with colours 
       glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
       glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float), &indices.front(), GL_STATIC_DRAW);
       
       // Configure vertex attributes for vertices 
       glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
       glEnableVertexAttribArray(0);


       // Do the same as above but for colours 
       glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
       glBufferData(GL_ARRAY_BUFFER, colours.size() * sizeof(float), &colours.front(), GL_STATIC_DRAW);

       glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
       glEnableVertexAttribArray(1);

       // Do the same as above but for normals 
       glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
       glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), &normals.front(), GL_STATIC_DRAW);

       glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
       glEnableVertexAttribArray(2);

       glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Window::render()
{
    float current_frame = glfwGetTime();
    delta_time = current_frame - last_frame;
    last_frame = current_frame; 

    processInput(window);

    glClearColor(0.3f, 0.2f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    shader_ptr->use();
    shader_ptr->setVec3("light.direction", -0.2f, -1.0f, -0.3f);
    shader_ptr->setVec3("viewPos", camera.Position);

    // light properties 
    shader_ptr->setVec3("light.ambient", 1.0:, 1.0f, 1.0f);
    shader_ptr->setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);
    shader_ptr->setVec3("light.specular", 1.0f, 1.0f, 1.0f);

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, camera_near, camera_far);
    shader_ptr->setMat4("projection", projection);

    glm::mat4 view = camera.GetViewMatrix();
    shader_ptr->setMat4("view", view);

    glm::mat4 model = glm::mat4(1.0f);
    shader_ptr->setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, terrain.getIndices().size() * sizeof(unsigned int), GL_UNSIGNED_INT, 0);
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Window::processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        instance->camera.ProcessKeyboard(FORWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        instance->camera.ProcessKeyboard(BACKWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, delta_time);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        instance->camera.ProcessKeyboard(RIGHT, delta_time);
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
