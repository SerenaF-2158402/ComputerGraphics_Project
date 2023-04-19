#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <glm/glm/glm.hpp>
#include <GL/glut.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include "stb_image.h"
#include "shader.h"

#include <iostream>

class Maze {
    std::vector<std::vector<int>>& $maze;

 


    bool readMaze(const std::string& filename) {
        std::ifstream inFile(filename);
        if (!inFile.is_open()) {
            std::cerr << "Failed to open file " << filename << "!" << std::endl;
            return false;
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
                $maze.push_back(row);
                row.clear();
            }
            else {
                std::cerr << "Invalid character found in file " << filename << "!" << std::endl;
                return false;
            }
        }

        inFile.close();
        return true;
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

        // Verwijder de buffers
        glDeleteBuffers(1, &vertexBuffer);
        glDeleteBuffers(1, &indexBuffer);
    }

    //creer de muren
    void createWallCubes(std::vector<std::vector<int>>& mazeArray, int mazeSize, float cubeSize) {
        // Loop over the maze array and create cubes for walls
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
                    glutSolidCube(cubeSize);
                    glPopMatrix();
                }
            }
        }
    }

    void renderScene(float floorSize, float cubeSize) {
        //teken de vloer
        drawFloor($maze, floorSize);
         //teken de muren
        createWallCubes($maze, $maze.size(), cubeSize);

    }

  





     int main() {

        
        bool readFileStatus = readMaze("maze.txt");

        //test om te printen
        for (std::vector<int> row : $maze) {
            for (int i : row) {
                std::cout << i;
            }
        }

        // Initialiseer GLFW en maak een venster
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW!" << std::endl;
            return -1;
        }

        GLFWwindow* window = glfwCreateWindow(800, 600, "Maze", nullptr, nullptr);
        if (!window) {
            std::cerr << "Failed to create GLFW window!" << std::endl;
            glfwTerminate();
            return -1;
        }

        glfwMakeContextCurrent(window);

        // Initialiseer GLAD
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "Failed to initialize GLAD!" << std::endl;
            glfwTerminate();
            return -1;
        }

        // Initialiseer de shader
        Shader shader("vertexShader.glsl", "fragmentShader.glsl");

        // Stel de grootte van de vloer en de kubussen in
        float floorSize = 50.0f;
        float cubeSize = 1.0f;

        // Maak een instantie van de Maze-klasse en teken de scene
        renderScene(floorSize, cubeSize);

        // Zet de achtergrondkleur
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

        // Voer de hoofdloop uit
        while (!glfwWindowShouldClose(window)) {
            // Wis de achtergrond
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Gebruik de shader
            shader.use();

            // Render de scene
            renderScene(floorSize, cubeSize);

            // Swap buffers
            glfwSwapBuffers(window);

            // Poll for events
            glfwPollEvents();
        }

        // Sluit GLFW af
        glfwTerminate();

        return 0;



    }
};



/*
* void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set up the camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //gluLookAt(cameraPosX, cameraPosY, cameraPosZ, cameraLookAtX, cameraLookAtY, cameraLookAtZ, 0, 1, 0);
    gluLookAt(0, 0, 0, 0, 0, 0, 0, 1, 0);


    // Render the floor mesh
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, floorMeshVertices);
    glDrawArrays(GL_TRIANGLES, 0, floorMeshNumVertices);
    glDisableClientState(GL_VERTEX_ARRAY);

    // Render the wall cubes
    createWallCubes($maze, $maze.size(), 10.f);

    glutSwapBuffers();
}
*/

