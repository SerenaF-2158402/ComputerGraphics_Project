
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <glm/glm/gtc/matrix_transform.hpp>
#include "stb_image.h"
#include "shader.h"
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

class Maze {
private:


 
public:
    Maze(){
        
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

    //creer de muren
    void createWallCubes(const std::vector<std::vector<int>>& mazeArray, int mazeSize, float cubeSize){        // Loop over the maze array and create cubes for walls
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

    

     int mazemain() {

        
         std::vector<std::vector<int>> maze;
         std::ifstream inFile("maze.txt");
         if (!inFile.is_open()) {
             std::cerr << "Failed to open file " << "maze.txt" << "!" << std::endl;

         }

         std::vector<int> row;
         char c;

         while (inFile >> c) {
             if (c == '#') {
                 row.push_back(1); // muur
             }
             else if (c == ' ') {
                 row.push_back(0); // geen muur
             }
             else if (c == '\n') {
                 maze.push_back(row);
                 row.clear();
             }
             else {
                 std::cerr << "Invalid character found in file " << "maze.txt" << "!" << std::endl;
             }
         }

         inFile.close();

        for (std::vector<int> row : maze) {
            for (int elem : row) {
                std::cout << "elem: " << elem;
            }
        }
        std::cout << maze.size() << maze.at(0).size();


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
        unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
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

        // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
        // -------------------------------------------------------------------------------------------
        ourShader.use();
        ourShader.setInt("texture1", 0);
        ourShader.setInt("texture2", 1);


        // Voer de hoofdloop uit
        while (!glfwWindowShouldClose(window)) {
            processInput(window);

            // Wis de achtergrond
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture1);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture2);

            // activate shader
            ourShader.use();

            
            // create transformations
            glm::mat4 view = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            glm::mat4 projection = glm::mat4(1.0f);
            projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
            // pass transformation matrices to the shader
            ourShader.setMat4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
            ourShader.setMat4("view", view);
            float time = (float)glfwGetTime();
            // render boxes
            glBindVertexArray(VAO);

            //teken cube
            for (unsigned int i = 1; i < 10; i++)
            {
                // calculate the model matrix for each object and pass it to shader before drawing

                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, cubePositions[i]);
                float angle = 20.0f * i;
                model = glm::rotate(model, time * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
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

        // Sluit GLFW af
        glfwTerminate();


        return 0;

    }
};


//cube tekenen
//vloer tekenen
//shader toevoegen 

