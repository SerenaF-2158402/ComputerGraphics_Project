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

//creer de muren
void mazeGenerator::createWallCubes(const std::vector<std::vector<int>>& mazeArray, int mazeSize, float cubeSize) {        // Loop over the maze array and create cubes for walls
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

void mazeGenerator::drawFloor(const std::vector<std::vector<int>>& maze, float floorSize) {
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