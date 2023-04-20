#ifndef MAZEGENERATOR_H
#define MAZEGENERATOR_H

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
#include "mazeGenerator.h"
#include "movement.h"

class mazeGenerator {

public:
	mazeGenerator();

	void createWallCubes(const std::vector<std::vector<int>>& mazeArray, int mazeSize, float cubeSize);
	void drawFloor(const std::vector<std::vector<int>>& maze, float floorSize);

	std::vector<std::vector<int>> getMazeFromFile();
	std::vector<glm::vec3> getCubeLocations();
	std::vector<glm::vec3> findCubeLocations();
	std::vector<std::vector<int>> readFile();
private:
	
	
	void getDebugInfo();

	std::vector<glm::vec3> cubePositions;
	std::vector<std::vector<int>> maze;
};

#endif // !MAZEGENERATOR_H
