#include <main.hpp>

void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
  fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
}





const float rotate_speed = 0.5;
const int WORLD_SIZE = 8;

int window_width, window_height;

// float * vector2floats(std::vector<glm::vec3> in, float &out) {
//     out = ; 
//     for(int i = 0; i < in.size(); i++) {
//         out[i*3-2] = in[i].x;
//         out[i*3-1] = in[i].y;
//         out[i*3-0] = in[i].z;
//     }
//     return out;
// }
// void CheckGLError() {	GLuint err = glGetError();	if (err != GL_NO_ERROR)		std::cerr << std::hex << err << std::endl;}
// #ifndef NDEBUG
// #	define CHECK_GL_ERROR CheckGLError();
// #else
// #	define CHECK_GL_ERROR
// #endif

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    window_width = width;
    window_height = height;
    glViewport(0, 0, width, height);
}



glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  1.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, -1.0f,  0.0f);

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    const float cameraSpeed = 0.2f; // adjust accordingly

    glm::vec3 direction = cameraFront;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * direction;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * direction;

    
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(direction, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(direction, cameraUp)) * cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cameraPos.y += cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        cameraPos.y -= cameraSpeed;
}

float pitch = 0.0, yaw = 0.0;
float lastX = 400, lastY = 300;
bool firstMouse = true;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{   
    if(firstMouse) {
        lastX = xpos, lastY = ypos;
        firstMouse = false;
    }


    double xOffset = xpos - lastX;
    double yOffset = ypos - lastY;

    yaw -= xOffset/window_width;

    pitch += yOffset/window_height;

    if(pitch > glm::radians(89.0))
        pitch = glm::radians(89.0);

    if(pitch < glm::radians(-89.0))
        pitch = glm::radians(-89.0);
    
    glm::vec3 direction;
    
    direction.x = sin(yaw) * cos(pitch);
    // direction.y = sin(glm::radians(pitch));
    direction.z = cos(yaw) * cos(pitch); 

    cameraFront = direction;

    // code before here
    lastX = xpos;
    lastY = ypos;
}  

double scrolled = 1.0;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    scrolled += yoffset/2.0;
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);  

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    glfwGetWindowSize(window, &window_width, &window_height);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }  
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // if (glfwRawMouseMotionSupported()) glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);


    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    // During init, enable debug output
    glEnable              ( GL_DEBUG_OUTPUT );
    glDebugMessageCallback( MessageCallback, 0 );

    // glfwSetErrorCallback( MessageCallback);

    Shader base_shader = Shader("../assets/shaders/base_shader.vsh", "../assets/shaders/base_shader.fsh");


    // uncomment this call to draw in wireframe polygons.
    
    // glm::vec3 posMod = glm::vec3(1.0);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);


    glfwWindowHint(GLFW_SAMPLES, 4);
    glEnable(GL_MULTISAMPLE);  

    


    VoxelPalette palette = VoxelPalette();
    Voxel cube = Voxel("assets/cube.obj", false);
    Voxel grass = Voxel("assets/grass.obj", false);
    palette.registerType(cube);
    palette.registerType(grass);

    World world = World(16, &palette);
    
    for(int x = 0; x < WORLD_SIZE; x++) {
        for(int y = 0; y < WORLD_SIZE; y++) {
            for(int z = 0; z < WORLD_SIZE; z++) {
                world.generateAt(x, y, z);
            }
        }
    }
    
    // VoxelChunk voxels = VoxelChunk(CHUNK_SIZE, palette);
    // for(int x = 0; x < CHUNK_SIZE; x++) {
    //     for(int z = 0; z < CHUNK_SIZE; z++) {
    //         float a = sin(x/2)*cos(z/2);
    //         a *= 4;
    //         a += 4;
    //         a = abs(a);
    //         for(int y = 0; y < ceil(a); y++) {
    //             voxels.setVoxel(x,y,z,1);
    //         }
    //         voxels.setVoxel(x,a,z,2);
    //     }
    // }

    // for(int x = 1; x < CHUNK_SIZE-1; x++) {
    //     for(int z = 1; z < CHUNK_SIZE-1; z++) {
    //         for(int y = 1; y < CHUNK_SIZE-1; y++) {
    //             voxels.updateVisible(x,y,z);
    //         }
    //     }
    // }
    
    
    // render loop
    // -----------

    // framebuffer configuration
    // -------------------------
    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // create a color attachment texture
    unsigned int textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    std::cout << window_width << ", " << window_height << std::endl;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window_width, window_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    
    // create a depth stencil attachment texture

    unsigned int textureDepthStencilbuffer;
    glGenTextures(1, &textureDepthStencilbuffer);

    glBindTexture(GL_TEXTURE_2D, textureDepthStencilbuffer);

    
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, window_width, window_height, 0, 
        GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, textureDepthStencilbuffer, 0);


    // // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    // unsigned int rbo;
    // glGenRenderbuffers(1, &rbo);
    // glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    // glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, window_width, window_height); // use a single renderbuffer object for both a depth AND stencil buffer.
    // glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Screen quad config
    // ------------------

    // vertices for the quad
    float quad_vertices[] = {
        -1.0, 0.0, -1.0, /*tex coords*/ 0.0, 0.0,
        1.0, 0.0, -1.0, /*tex coords*/ 1.0, 0.0,
        -1.0, 0.0, 1.0, /*tex coords*/ 0.0, 1.0,
        1.0, 0.0, -1.0, /*tex coords*/ 1.0, 0.0,
        -1.0, 0.0, 1.0, /*tex coords*/ 0.0, 1.0,
        1.0, 0.0, 1.0, /*tex coords*/ 1.0, 1.0,
    };

    // vao/vbo for quad
    unsigned int qVAO;
    unsigned int qVBO;

    glGenVertexArrays(1, &qVAO);
    glGenBuffers(1, &qVBO);

    glBindVertexArray(qVAO);
    glBindBuffer(GL_ARRAY_BUFFER, qVBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), &quad_vertices[0], GL_STATIC_DRAW);
    
    // vertex positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);

    // texture coords
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));

    glBindVertexArray(0);
    

    Shader quad_shader = Shader("../assets/shaders/quad.vsh", "../assets/shaders/quad.fsh");
    Shader quad_shader2 = Shader("../assets/shaders/quad2.vsh", "../assets/shaders/quad2.fsh");

    while (!glfwWindowShouldClose(window)) {

        // glClearColor(1, 1, 1, 1);
        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // -----
        processInput(window);

        double cursorX, cursorY;
        glfwGetCursorPos(window, &cursorX, &cursorY);
        

        // std::cout << cursorX / window_width << ", " << cursorY / window_height << std::endl;

        glm::mat4 proj = glm::perspective(glm::radians(90.0f), (float)window_width/(float)window_height, 0.1f, 100.0f);
        
        glm::mat4 model = glm::mat4(1.0f);

        // model = glm::rotate(model, glm::radians(1.0f), glm::vec3(0,1,0));

        model = glm::scale(model, glm::vec3(scrolled));

        glm::mat4 view = glm::mat4(1.0);
        

        view = glm::rotate(view, pitch, glm::vec3(1, 0, 0));
        view = glm::rotate(view, yaw, cameraUp);
        
        
        
        view = glm::translate(view, cameraPos);

        // const float cameraSpeed = 0.05f;
        // if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        //     view = glm::translate(view, cameraSpeed * cameraFront);
        // if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        //     view = glm::translate(view, -cameraSpeed * cameraFront);
        // if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        //     view = glm::translate(view, glm::normalize(glm::cross(cameraFront, cameraUp)) * -cameraSpeed);
        // if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        //     view = glm::translate(view, glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed);
        
        

        // glEnable(GL_CLIP_DISTANCE0);

        // first pass
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
        glEnable(GL_DEPTH_TEST);

        glEnable(GL_CULL_FACE); 
        glCullFace(GL_BACK);
        
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        

        // quad_shader2.use();
        // glBindVertexArray(qVAO);
        // base_shader.use();
        
        // glDrawArrays(GL_TRIANGLES, 0, 3);  
        // base_shader.use();
        // std::cout << "main: base shader ptr: " << &base_shader << std::endl;

        base_shader.use();
        base_shader.setMat4("view", view);
        base_shader.setMat4("projection", proj);
        base_shader.setMat4("model", model);

        // voxels.render(base_shader, framebuffer);
        world.render(&base_shader, cameraPos);
        
        // second pass
        glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default

        
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT);
        
        
        // glUseProgram(0);
        glDisable(GL_CULL_FACE); 
        // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        

        quad_shader.use();  
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
        quad_shader.setInt("screenTexture", 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureDepthStencilbuffer);
        quad_shader.setInt("depthTexture", 1);


        glBindVertexArray(qVAO);
        glDisable(GL_DEPTH_TEST);
        
        glDrawArrays(GL_TRIANGLES, 0, 6);  

        glfwSwapBuffers(window);
        
        glfwPollEvents();
        // voxels.clear();
        // break;
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------

    
    glfwTerminate();
    return 0;
}
