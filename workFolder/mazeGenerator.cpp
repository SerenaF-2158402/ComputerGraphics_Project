#include "mazeGenerator.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

mazeGenerator::mazeGenerator() {
    readFile();
    findCubeLocations();
}

void mazeGenerator::getDebugInfo() {
    for (std::vector<int> row : maze) {
        for (int elem : row) {
            std::cout << elem;
        }
    }
    std::cout << maze.size() << " " << maze.at(0).size();

    // Print out the positions
    for (int i = 0; i < cubePositions.size(); i++) {
        std::cout << "Position " << i << ": (" << cubePositions[i].x << ", " << cubePositions[i].y << ", " << cubePositions[i].z << ")" << std::endl;
    }

}

void mazeGenerator::readFile() {
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
}

void mazeGenerator::findCubeLocations() {
    std::cout << "now find walls";

    // create cubes
    printf("\n\n\nSIZE::%zd", maze.size());
    for (int i = 0; i < maze.size(); i++) {
        for (int j = 0; j < maze.at(i).size(); j++) {
            if (maze.at(i).at(j) == 1) {
                // Calculate the position of the cube
                float x = (i + 1);
                float y = 1;

                cubePositions.push_back(glm::vec3(i, y, j));
                
                // Get AABB's of the cube
                AABB aabb;
                aabb.min = glm::vec3(i - 0.5f, y - 1.0f, j - 0.5f);
                aabb.max = glm::vec3(i + 0.5f, y + 1.0f, j + 0.5f);

                cubeAABB.push_back(aabb);
                 
            }
        }
        //float z = (j + 1);
    }

}
void mazeGenerator::drawFloor(Shader floorshader) {


    // Compute the dimensions of the floor based on the maze size
    float width = maze[0].size() - 0.20f;
    float depth = maze.size() + 10.0f ;

    // Define the vertices for the floor
    float floorVertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
    };
    // Scale the floor vertices to match the maze size
    for (int i = 0; i < 30; i += 5) {
        floorVertices[i] *= width;
        floorVertices[i + 1] *= depth;
    }

    /// configure floor VAO and VBO
    unsigned int floorVAO, floorVBO;
    glGenVertexArrays(1, &floorVAO);
    glGenBuffers(1, &floorVBO);

    glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);

    glBindVertexArray(floorVAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(30.0f, 0.0f, 15.0f));

    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::translate(model, glm::vec3(0.0f, 10.0f, 20.0f));

    floorshader.setMat4("model", model);


    // Draw the floor using glDrawArrays
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Clean up the VAO and VBO
    glDisableVertexAttribArray(0);
    glDeleteBuffers(1, &floorVBO);
    glDeleteVertexArrays(1, &floorVAO);

}
