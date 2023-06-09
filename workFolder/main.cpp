#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include "stb_image.h"
#include <irrKlang/include/irrKlang.h>
#include "shader.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "Camera.h"
#include "shader_m.h"
#include <iostream>
#include "mazeGenerator.h"
#include "movement.h"
#include "model.h"

using namespace irrklang;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
unsigned int loadTexture(const char* path);
void processInput(GLFWwindow* window, glm::vec3* cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp, std::vector<mazeGenerator::AABB> cubeAABBs);
glm::mat4 calculateViewMatrix(GLFWwindow* window, glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp, std::vector<mazeGenerator::AABB> cubeAABBs);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void newPosIsCollision(glm::vec3* cameraPos, glm::vec3 newLocation, std::vector<mazeGenerator::AABB> cubeAABBs);
unsigned int loadCubemap(std::vector<std::string> faces);
bool handleLeftMouseButtonPress(GLFWwindow* window);

unsigned int loadTexture(const char* path);

ISoundEngine* SoundEngine = createIrrKlangDevice();

// Move these all away from global
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 400, lastY = 300;
// Problem: mouse callback can't accept this as extra, so need to use the camera.h to fix it somehow
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 cameraPos = glm::vec3(0.0f, 10.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
int NUM_LAMPS = 20;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

bool firstMouse = true;
float yVelocity = 0.0f;
bool isJumping = false;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting



int main()
{
    mazeGenerator generator;

    float cameraYaw = -90.0f;
    float cameraPitch = 0.0f;
    float cameraSpeed = 0.05f;


    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);


    // build and compile our shader zprogram
    // ------------------------------------
    //Shader wallShader("1.colors.vs", "1.colors.fs");
    Shader floorShader("floorTexture.vs", "floorTexture.fs");
    Shader shader("6.1.cubemaps.vs", "6.1.cubemaps.fs");
    Shader skyboxShader("6.1.skybox.vs", "6.1.skybox.fs");
    //ligth
    Shader lightingShader("1.colors.vs", "1.colors.fs");
    Shader lightCubeShader("1.light_cube.vs", "1.light_cube.fs");

    // Stel de grootte van de vloer en de kubussen in
    float floorSize = 50.0f;
    float cubeSize = 1.0f;



    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    float skyboxVertices[] = {
        // positions          
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



    // first, configure the cube's VAO (and VBO)
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // skybox VAO
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


    std::vector<std::string> faces
    {
        "Spacebox5/right.png",
        "Spacebox5/left.png",
        "Spacebox5/top.png",
        "Spacebox5/bottom.png",
        "Spacebox5/front.png",
        "Spacebox5/back.png"
    };
    unsigned int cubemapTexture = loadCubemap(faces);
    unsigned int texture1 = loadTexture("black_floor.jpg");

    unsigned int texture2 = loadTexture("bloodvessel.jpg");
   

  

    lightingShader.use();
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 0);

    //lightingShader.setInt("texture2", 1);


    // Get the maze from the input file
    std::vector<std::vector<int>> maze = generator.getMazeFromFile();
    // Get the cubes from the maze generator
    std::vector<glm::vec3> cubePositions = generator.getCubeLocations();
    int numCubes = generator.getCubeLocations().size();


    // General settings which need to be defined before the program can run
    // Set jumping to false, is later used to make sure the player can't keep jumping before landing
    bool isJumping{ false };
    double jumpStartingTime = 0.0f;

    // Hide mouse from the screen
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // Set callback function for mouse handling
    glfwSetCursorPosCallback(window, mouse_callback);

    // Find boxes' AABB's
    std::vector<mazeGenerator::AABB> cubeAABBs = generator.getCubeAABB();

    // Only used for debug:
    int counter = 0;

    // Save last cameraPos so that if you try to go thru a wall you can be put back
    glm::vec3 lastCameraPos;

    Model ourModel("meshes/UFO/Low_poly_UFO.obj");
    Model cubeModel("meshes/Block_blender/mazecube.obj");
    Model floorModel("meshes/Block_blender/mazePlain.obj");

    Shader modelShader("1.model_loading.vs", "1.model_loading.fs");
    

    
    //instancing
    unsigned int amount = cubePositions.size();
    glm::mat4* modelMatrices;
    modelMatrices = new glm::mat4[amount];
    srand(static_cast<unsigned int>(glfwGetTime())); // initialize random seed

    for (unsigned int i = 0; i < amount; i++)
    {
        glm::mat4 modelCube = glm::mat4(1.0f);
        glm::vec3 cubePos = cubePositions[i];
        modelCube = glm::translate(modelCube, cubePos);
        modelCube = glm::scale(modelCube, glm::vec3(0.5f, 1.0f, 0.5f));
        modelMatrices[i] = modelCube;
    }

    // render loop
    // -----------
    //glm::vec3 lightPos(cubePositions[10].x, cubePositions[10].y + 1.5f, cubePositions[10].y);

    std::vector<glm::vec3> pointLightPositions;

    // positions of the point lights
    modelShader.use();

    bool isUfoFound = false;


    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window, &cameraPos, cameraFront, cameraUp, cubeAABBs);


        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

        // bind textures on corresponding texture units



        //processInput(window, &cameraPos, cameraFront, cameraUp);

        glm::mat4 view = calculateViewMatrix(window, cameraPos, cameraFront, cameraUp, cubeAABBs);


        float time = (float)glfwGetTime();


        // pass transformation matrices to the shader
        // activate shader
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);
        lightingShader.setFloat("material.shininess", 32.0f);


        // directional light
        lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        lightingShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

        for (int i = 0; i < 8; i++) {
            lightingShader.setVec3("pointLights[" + std::to_string(i) + "].position", glm::vec3(10.1f, 1.1f + i, 8.0f));
            lightingShader.setVec3("pointLights[" + std::to_string(i) + "].ambient", 0.05f, 0.05f, 0.05f);
            lightingShader.setVec3("pointLights[" + std::to_string(i) + "].diffuse", 0.8f, 0.8f, 0.8f);
            lightingShader.setVec3("pointLights[" + std::to_string(i) + "].specular", 1.0f, 1.0f, 1.0f);
            lightingShader.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0f);
            lightingShader.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09f);
            lightingShader.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.032f);
        }






        // spotLight
        lightingShader.setVec3("spotLight.position", camera.Position);
        lightingShader.setVec3("spotLight.direction", camera.Front);
        lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("spotLight.quadratic", 0.032f);


        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
        lightingShader.setMat4("view", view);
        lightingShader.setMat4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);
        // render boxes


        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture2);
        //lightCubeShader.use();
        lightingShader.use();

        // draw cubes with instancing
        for (unsigned int i = 0; i < amount; i++)
        {
            lightingShader.setMat4("model", modelMatrices[i]);
            cubeModel.Draw(lightingShader);
        }


        //draw floor
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        lightingShader.use();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.1f, 0.0f, 8.0f));
        lightingShader.setMat4("model", model);
        floorModel.Draw(lightingShader);

        //light cube under the ufo
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);

        glBindVertexArray(lightCubeVAO);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.1f, 12.0f, 8.0f));
        model = glm::scale(model, glm::vec3(1.5f)); // Make it a smaller cube
        lightCubeShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        if (!isUfoFound) {
            modelShader.use();
            modelShader.setMat4("projection", projection);
            modelShader.setMat4("view", view);
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(10.1f, 10.0f, 8.0f));
            model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
            modelShader.setMat4("model", model);
            ourModel.Draw(modelShader);
        }

        // draw skybox as last
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        skyboxShader.use();

        glm::mat4 skyboxView = glm::mat4(glm::mat3(view)); // remove translation from the view matrix
        skyboxShader.setMat4("view", skyboxView);
        skyboxShader.setMat4("projection", projection);
        glm::mat4 skyboxModel = glm::mat4(1.0f);
        skyboxModel = glm::scale(skyboxModel, glm::vec3(100.0f)); // adjust the scale of the skybox cube
        skyboxShader.setMat4("model", skyboxModel);
        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //glDepthMask(GL_TRUE);

        // Check if left mouse button is being pressed
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !isUfoFound)
        {
            // Call function to handle left mouse button press
            isUfoFound = handleLeftMouseButtonPress(window);
        }

        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default



        // glfw: swap buffers and l IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVBO);
    glDeleteVertexArrays(1, &lightCubeVAO);


    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

glm::mat4 calculateViewMatrix(GLFWwindow* window, glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp, std::vector<mazeGenerator::AABB> cubeAABBs)
{
    processInput(window, &cameraPos, cameraFront, cameraUp, cubeAABBs);
    return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    yaw += xoffset;
    pitch += yoffset;
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
    camera.ProcessMouseMovement(xoffset, yoffset);



}


bool handleLeftMouseButtonPress(GLFWwindow* window) {
    double pixelX = SCR_WIDTH / 2;
    double pixelY = SCR_HEIGHT / 2; // Flip y-coordinate to OpenGL orientation

    // Read the color of the pixel that the mouse is currently over
    unsigned char color[4];
    glReadPixels(pixelX, pixelY, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);

    // Print the color value
    //printf("Mouse color: (%d, %d, %d)\n", color[0], color[1], color[2]);
    if (color[1] > 240 && color[0] < 240 && color[2] < 240) {
        printf("UFO FOUND\n");
        return true;
    }
    printf("No UFO!\n");
    return false;
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void processInput(GLFWwindow* window, glm::vec3* cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp, std::vector<mazeGenerator::AABB> cubeAABBs)
{
    // Close window when pressing escape
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    const float cameraSpeed = 2.5f * deltaTime;
    glm::vec3 cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));

    const float jumpSpeed = 7.0f; // m/s
    const float gravity = 9.81f; // m/s^2

    // When loading for the first time the deltatime is like 5 seconds depending on computer and it instantly teleports the player under the floor so we skip the first tick
    if (deltaTime < 1.0f) {
        // Apply gravity if player is in the air
        cameraPos->y += yVelocity * deltaTime;
        yVelocity -= gravity * deltaTime;
        if (cameraPos->y <= 0.5f) {
            cameraPos->y = 0.5f;
            yVelocity = 0.0f;
            isJumping = false;
        }
    }


    // Move player
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        newPosIsCollision(cameraPos, glm::vec3(cameraFront.x, 0.0f, cameraFront.z) * cameraSpeed, cubeAABBs);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        newPosIsCollision(cameraPos, -glm::vec3(cameraFront.x, 0.0f, cameraFront.z) * cameraSpeed, cubeAABBs);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        newPosIsCollision(cameraPos, -cameraRight * cameraSpeed, cubeAABBs);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        newPosIsCollision(cameraPos, cameraRight * cameraSpeed, cubeAABBs);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !isJumping && cameraPos->y <= 0.5f) {
        yVelocity = jumpSpeed;
        isJumping = true;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
        newPosIsCollision(cameraPos, glm::vec3(0.0f, gravity, 0.0f), cubeAABBs);
}

void newPosIsCollision(glm::vec3* cameraPos, glm::vec3 newLocation, std::vector<mazeGenerator::AABB> cubeAABBs) {
    // Check if collision by comparing a sphere around the cameraPos with every box's AABB
    bool collisionFound = false;
    glm::vec3 sphereCenter = *cameraPos + glm::vec3(0.1f, 0.1f, 0.1f); // Use cameraPos with offset
    float sphereRadius = 0.25f; // Adjust this value as needed
    for (int i = 0; i < cubeAABBs.size(); i++) {
        mazeGenerator::AABB aabb = cubeAABBs[i];
        glm::vec3 closestPointInAABB = glm::clamp(sphereCenter, aabb.min, aabb.max);
        float distance = glm::distance(closestPointInAABB, sphereCenter);
        glm::vec3 direction = glm::normalize(newLocation);
        glm::vec3 directionToAABB = glm::normalize(closestPointInAABB - sphereCenter);
        if (distance < sphereRadius && glm::dot(direction, directionToAABB) > 0) {
            collisionFound = true;
            break;
        }
    }

    if (!collisionFound) {
        *cameraPos += newLocation;
        if (cameraPos->y > 50.0f)
            cameraPos->y = 50.0f;
    }
    else {
        SoundEngine->play2D("Wrong.wav", false);
    }
}




unsigned int loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


    return textureID;
}
