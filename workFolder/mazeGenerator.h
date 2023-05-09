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
	struct AABB {
		glm::vec3 min;
		glm::vec3 max;
	};
	mazeGenerator();


	int loadCubemap(std::vector<std::string> faces);

	std::vector<std::vector<int>> getMazeFromFile() { return maze; };
	std::vector<glm::vec3> getCubeLocations() { return cubePositions; };
	std::vector<AABB> getCubeAABB() { return cubeAABB; };

private:




	void readFile();
	void findCubeLocations();

	void getDebugInfo();

	std::vector<AABB> cubeAABB;
	std::vector<glm::vec3> cubePositions;
	std::vector<std::vector<int>> maze;
};

#endif // !MAZEGENERATOR_H
