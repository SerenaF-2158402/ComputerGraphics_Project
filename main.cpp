#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include "stb_image.h"
#include "shader.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, glm::vec3& playerPosition, glm::vec3& cameraPosition, glm::vec3& cameraFront, glm::vec3& cameraUp, bool* isJumping, double* jumpStartingTime);
//void processJumping(GLFWwindow* window, glm::vec3& playerPosition, glm::vec3& cameraPosition, double* jumpStartingTime);
void createWallCubes(const std::vector<std::vector<int>>& mazeArray, int mazeSize, float cubeSize);
void drawFloor(const std::vector<std::vector<int>>& maze, float floorSize);

// Settings to be changed to become dynamic
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


// Move this away from global
glm::vec3 playerPosition = glm::vec3(0.0f, 0.0f, 0.0f);
float playerYaw = -90.0f;
float playerPitch = 0.0f;

glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float cameraYaw = -90.0f;
float cameraPitch = 0.0f;
float cameraSpeed = 0.05f;


int main()
{


    std::vector<std::vector<int>> maze;
    std::ifstream inFile("maze.txt");
    if (!inFile.is_open()) {
        std::cerr << "Failed to open file " << "maze.txt" << "!" << std::endl;
    }

    std::string line;
    while (std::getline(inFile, line)) {
        std::vector<int> row;
        for (char c : line) {
            if (c == '#') {
                row.push_back(1); // muur
            }
            else if (c == ' ') {
                row.push_back(0); // geen muur
            }
            else {
                std::cerr << "Invalid character found in file " << "maze.txt" << "!" << std::endl;
            }
        }
        maze.push_back(row);
    }

    inFile.close();


    for (std::vector<int> row : maze) {
        for (int elem : row) {
            std::cout << elem;
        }
    }
    std::cout << maze.size() << " " << maze.at(0).size();


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
    Shader ourShader("texture.vs", "texture.fs");

    // Stel de grootte van de vloer en de kubussen in
    float floorSize = 50.0f;
    float cubeSize = 1.0f;

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    // world space positions of our cubes
    /*
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    */

    //create cubes
    std::vector<glm::vec3> cubePositions;
    for (int i = 0; i < maze.size(); i++) {
        for (int j = 0; j < maze.at(i).size(); j++) {
            if (maze.at(i).at(j) == 1) {
                // Calculate the position of the cube
                float x = (i + 1);
                float y = 0;

                cubePositions.push_back(glm::vec3(i, y, j));
            }
        }
        //float z = (j + 1);


    }

    // Print out the positions
    for (int i = 0; i < cubePositions.size(); i++) {
        std::cout << "Position " << i << ": (" << cubePositions[i].x << ", " << cubePositions[i].y << ", " << cubePositions[i].z << ")" << std::endl;
    }

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // load and create a texture 
    // -------------------------
    unsigned int texture1, texture2;
    // texture 1
    // ---------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char* data = stbi_load("container.jpg", & width, & height, & nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    // texture 2
    // ---------
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    
    // load image, create texture and generate mipmaps
    /*
    data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    */

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);

    // General settings which need to be defined before the program can run
    // Set jumping to false, is later used to make sure the player can't keep jumping before landing
    bool isJumping{ false };
    double jumpStartingTime = 0.0f;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window, playerPosition, cameraPosition, cameraFront, cameraUp, &isJumping, &jumpStartingTime);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // activate shader
        ourShader.use();

        // Update where the camera is looking
        glm::vec3 front;
        front.x = cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
        front.y = sin(glm::radians(cameraPitch));
        front.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
        cameraFront = glm::normalize(front);

        // Change actual angle the camera looks
        glm::mat4 view = glm::lookAt(playerPosition, playerPosition + cameraFront, cameraUp);

        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        
        // I don't know what this does but it works
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));


        // pass transformation matrices to the shader
        ourShader.setMat4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        ourShader.setMat4("view", view);

        float time = (float)glfwGetTime();
        // render boxes
        glBindVertexArray(VAO);
        // Draw each cube
        for (unsigned int i = 0; i < cubePositions.size(); i++)
        {
            // Calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            model = glm::scale(model, glm::vec3(1.0f, 2.0f, 1.0f));
            float angle = 20.0f * i;
            //model = glm::rotate(model, time * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader.setMat4("model", model);


            glDrawArrays(GL_TRIANGLES, 0, 36);
        }


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window, glm::vec3& playerPosition, glm::vec3& cameraPosition, glm::vec3& cameraFront, glm::vec3& cameraUp, bool* isJumping, double* jumpStartingTime) {
    // Close window when pressing escape
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Handle the extra: jumping
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS || *isJumping) {
        if (!*isJumping)
            *jumpStartingTime = glfwGetTime();
        *isJumping = true;

        // Move upwards
        if (glfwGetTime() - *jumpStartingTime < 1.0f) {
            playerPosition[1] += 0.05f;
        }
        // End jump movement
        else if (glfwGetTime() - *jumpStartingTime <= 2.0f) {
            playerPosition[1] -= 0.05f;
        }
        // Move downwards
        else {
            *isJumping = false;
        }
    }

    // Get time difference since last frame
    // Fix this so it actually works 
    float deltaTime = 0.1f;
    const float playerSpeed = 5.0f;
    // Handle player input for movement -> move to new function because ugly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        playerPosition += playerSpeed * deltaTime * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        playerPosition -= playerSpeed * deltaTime * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        playerPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * playerSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        playerPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * playerSpeed * deltaTime;
    
    // Move up and down
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        playerPosition[1] += 0.1f;
    if (glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS)
        playerPosition[1] -= 0.1f;

    if (playerPosition[1] > 100)
        playerPosition[1] = 100;
    if (playerPosition[1] < -10)
        playerPosition[1] = -10;


    //printf("Player position: %f, %f, %f\n\n", playerPosition[0], playerPosition[1], playerPosition[2]);

    // Handle mouse movement, using Pythagorean theorem to calculate distance from center so it moves faster further from the middle
    // PROBLEM: Doesn't work with resizing the screen so needs to be fixed
    // calculate mouse offset and distance from center
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    // Get screen size incase it changed
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    float xOffset = mouseX - width / 2.0f;
    float yOffset = mouseY - height / 2.0f;
    // Calculate distance from middle
    float distance = sqrt(xOffset * xOffset + yOffset * yOffset);

    // Calculate max distance possible
    float maxDistance = sqrt((width / 2.0f) * (width / 2.0f) + (height / 2.0f) * (height / 2.0f));
    // Set mouse sensitivity based on distance from the center of the screen
    float mouseSensitivity = 0.005f + 0.0001f * (distance / maxDistance);

    // Set a maximum on the mouse sensitivity so the screen doesn't fly away when turning too far
    if (mouseSensitivity > 0.005)
        mouseSensitivity = 0.005f;

    // Update the offset
    xOffset *= mouseSensitivity;
    yOffset *= mouseSensitivity;

    // Check if the mouse moved enough to make movement happen
    if (mouseSensitivity > 0.0002) {
        cameraYaw += xOffset;
        cameraPitch -= yOffset;
    }

    // Constrain pitch angle so the screen doesn't flip when moving too far
    const float maxPitch = 89.0f;
    const float minPitch = -89.0f;
    if (cameraPitch > maxPitch) {
        cameraPitch = maxPitch;
    }
    if (cameraPitch < minPitch) {
        cameraPitch = minPitch;
    }

}

// Handles jumping
//void processJumping(GLFWwindow* window, glm::vec3& playerPosition, glm::vec3& cameraPosition, double *jumpStartingTime) {
//    // Check howlong it has been since jump was pressed
//   double timePassed = glfwGetTime() - *jumpStartingTime;
//    printf("Time passed: %f\n", timePassed);
//}

//creer de muren
void createWallCubes(const std::vector<std::vector<int>>& mazeArray, int mazeSize, float cubeSize) {        // Loop over the maze array and create cubes for walls
    for (int i = 0; i < mazeSize; i++) {
        for (int j = 0; j < mazeSize; j++) {
            if (mazeArray.at(i).at(j) == 1) {
                // Calculate the position of the cube
                float x = (i - (mazeSize / 2)) * cubeSize;
                float y = cubeSize / 2.0f;
                float z = (j - (mazeSize / 2)) * cubeSize;

                // Create the cube using OpenGL functions
                glPushMatrix();
                glTranslatef(x, y, z);
                //teken kubussen
                glPopMatrix();
            }
        }
    }
}

void drawFloor(const std::vector<std::vector<int>>& maze, float floorSize) {
    // Bepaal de grootte van het vlak en de positie op de grond
    float halfSize = floorSize / 2.0f;
    glm::vec3 floorPosition = glm::vec3(0.0f, -0.5f, 0.0f);

    // Maak een vector om de hoekpunten van het vlak op te slaan
    std::vector<glm::vec3> vertices(4);

    // Bereken de hoekpunten van het vlak
    vertices[0] = glm::vec3(-halfSize, 0.0f, -halfSize);
    vertices[1] = glm::vec3(-halfSize, 0.0f, halfSize);
    vertices[2] = glm::vec3(halfSize, 0.0f, -halfSize);
    vertices[3] = glm::vec3(halfSize, 0.0f, halfSize);

    // Maak een vector om de indices van de driehoeken op te slaan
    std::vector<unsigned int> indices = { 0, 1, 2, 2, 1, 3 };

    // Maak buffers voor de hoekpunten en de indices van de driehoeken
    GLuint vertexBuffer, indexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glGenBuffers(1, &indexBuffer);

    //vao

    // Zet de buffers in de juiste modus
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

    // Vul de buffers met de hoekpunten en de indices van de driehoeken
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Zet de vertexattributen op de juiste manier
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

    // Stel de positie van de mesh in
    glTranslatef(floorPosition.x, floorPosition.y, floorPosition.z);

    // Teken de mesh
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    // Maak de buffers weer los
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



    //helemaal op het einde van alles //voor terminate
    // Verwijder de buffers
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &indexBuffer);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}