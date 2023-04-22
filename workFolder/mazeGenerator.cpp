#include "mazeGenerator.h"

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
                float y = 0;

                cubePositions.push_back(glm::vec3(i, y, j));
                
                // Get AABB's of the cube
                AABB aabb;
                aabb.min = glm::vec3(i - 0.5f, y, j - 0.5f);
                aabb.max = glm::vec3(i + 0.5f, y + 2.0f, j + 0.5f);

                cubeAABB.push_back(aabb);
                 
            }
        }
        //float z = (j + 1);
    }

}

void mazeGenerator::drawFloor() {
    // Compute the dimensions of the floor based on the maze size
    float width = maze[0].size() * maze.size();
    float depth = maze.size() * maze.size();

   // Define the vertices for the floor
    float floorVertices[] = {
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, depth,
        width, 0.0f, depth,
        width, 0.0f, 0.0f
    };

    // Scale the floor vertices to match the maze size
    for (int i = 0; i < 12; i++) {
        floorVertices[i] *= maze.size();
    }

    // Set up the vertex array object (VAO)
    unsigned int floorVAO, floorVBO;
    glGenVertexArrays(1, &floorVAO);
    glGenBuffers(1, &floorVBO);

    glBindVertexArray(floorVAO);

    glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // Set the color of the floor to gray
    glColor3f(0.5f, 0.5f, 0.5f);

    // Draw the floor using glDrawArrays
    glDrawArrays(GL_QUADS, 0, 4);

    // Clean up the VAO and VBO
    glDisableVertexAttribArray(0);
    glDeleteBuffers(1, &floorVBO);
    glDeleteVertexArrays(1, &floorVAO);
}

 int mazeGenerator::loadCubemap(std::vector<std::string> faces)
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
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
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