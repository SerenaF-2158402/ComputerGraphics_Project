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
