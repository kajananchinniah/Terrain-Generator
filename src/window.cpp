#include "window.hpp"
 #define STB_IMAGE_IMPLEMENTATION
 #include <stb_image.h>

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
    instance->camera_far = std::min(terrains[0].getGridSizeX(), terrains[0].getGridSizeZ());
    

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

    // instance->skybox = new Skybox();

    // Enable depth 
    glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Initialize shader class with shaders from before 
    instance->shader_ptr = new Shader("shaders/vertex.vs", "shaders/fragment.fs");

     skybox_vertices =
     {
         -1.0f,  1.0f, -1.0f,
         -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         -1.0f,  1.0f, -1.0f,
 
         -1.0f, -1.0f,  1.0f,
         -1.0f, -1.0f, -1.0f,
         -1.0f,  1.0f, -1.0f,
         -1.0f,  1.0f, -1.0f,
         -1.0f,  1.0f,  1.0f,
         -1.0f, -1.0f,  1.0f,
 
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
 
         -1.0f, -1.0f,  1.0f,
         -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
         -1.0f, -1.0f,  1.0f,
 
         -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         -1.0f,  1.0f,  1.0f,
         -1.0f,  1.0f, -1.0f,
 
         -1.0f, -1.0f, -1.0f,
         -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
     };

     skybox_faces =
     {
         "resources/skybox/right.jpg",
         "resources/skybox/left.jpg",
         "resources/skybox/top.jpg",
         "resources/skybox/bottom.jpg",
         "resources/skybox/front.jpg",
         "resources/skybox/back.jpg"
     };
 
     loadCubemap();
     setupSkyRender();
 
     skybox_shader = new Shader("shaders/skybox.vs", "shaders/skybox.fs");
     skybox_shader->use();
     skybox_shader->setInt("skybox", 0);
  
    return 0;
}

int Window::shouldClose()
{
    return glfwWindowShouldClose(window);
}

void Window::setupRender(Terrain &terrain, float x, float z, unsigned int &VAO)
{
       // Generate terrain attributes 
       terrain.generate(x, z);
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
    shader_ptr->setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
    shader_ptr->setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
    shader_ptr->setVec3("light.specular", 1.0f, 1.0f, 1.0f);

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, camera_near, camera_far);
    shader_ptr->setMat4("projection", projection);

    glm::mat4 view = camera.GetViewMatrix();
    shader_ptr->setMat4("view", view);
    //TODO : clean this part up
    for (int i = 0; i < 3; i++)
    {
        float z_trans = floor(camera.Position.z / terrains[i].getGridSizeZ()) * terrains[i].getGridSizeZ();
        if (i == 0)
            z_trans += terrains[i].getGridSizeZ();
        if (i == 1)
            z_trans += 0.00f;
        if (i == 2)
            z_trans += -1.0f * terrains[i].getGridSizeZ();

        for (int j = 0; j < 3; j++)
        {
            float x_trans = floor(camera.Position.x / terrains[i].getGridSizeX()) * terrains[i].getGridSizeX(); 
            if (j == 0)
                x_trans += -1.0f * terrains[i].getGridSizeX();
            if (j == 1)
                x_trans += 0.00f;
            if (j == 2)
                x_trans += terrains[i].getGridSizeX();
          
           int idx = 3 * i + j;
           setupRender(terrains[idx], x_trans, z_trans, VAOs[idx]);
           glBindVertexArray(VAOs[idx]);
           glm::mat4 model = glm::mat4(1.0f);
           model = glm::translate(model, glm::vec3(x_trans, 0, z_trans));
           shader_ptr->setMat4("model", model);
           glDrawElements(GL_TRIANGLES, terrains[i].getIndices().size() * sizeof(unsigned int), GL_UNSIGNED_INT, 0);

        }
    }

    draw(view, projection);
    // skybox->draw(view, projection);
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

 void Window::loadCubemap()
 {
     glGenTextures(1, &cube_map_texture);
     glBindTexture(GL_TEXTURE_CUBE_MAP, cube_map_texture);

     int width, height, n_Channels;
     for (int i = 0; i < skybox_faces.size(); i++)
     {
         unsigned char *data = stbi_load(skybox_faces[i].c_str(), &width,        &height, &n_Channels, 0);
         if (data)
         {
             glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width,  height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
             stbi_image_free(data);
         }

         else
         {
             std::cout << "Failed to load cube map at " << skybox_faces[i] <<    "\n";
             stbi_image_free(data);
         }
     }
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
 }

 void Window::setupSkyRender()
 {
     glGenVertexArrays(1, &skybox_VAO);
     glGenBuffers(1, &skybox_VBO);
     glBindVertexArray(skybox_VAO);
     glBindBuffer(GL_ARRAY_BUFFER, skybox_VBO);
     glBufferData(GL_ARRAY_BUFFER, skybox_vertices.size() * sizeof(float),       &skybox_vertices.front(), GL_STATIC_DRAW);
     glEnableVertexAttribArray(0);
     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*   )0);
 }

 void Window::draw(const glm::mat4 &view, const glm::mat4 &projection)
 {
     glDepthFunc(GL_LEQUAL);
     skybox_shader->use();

     glm::mat4 tmp_view = glm::mat4(glm::mat3(view));
     skybox_shader->setMat4("view", tmp_view);
     skybox_shader->setMat4("projection", projection);

     glBindVertexArray(skybox_VAO);
     glActiveTexture(GL_TEXTURE0);
     glBindTexture(GL_TEXTURE_CUBE_MAP, cube_map_texture);
     glDrawArrays(GL_TRIANGLES, 0, 36);
     glDepthFunc(GL_LESS);
 }

