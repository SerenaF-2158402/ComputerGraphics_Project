#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "maze.h"


class movement
{
public:
	movement();
	void processInput(GLFWwindow* window, glm::vec3& playerPosition, glm::vec3& cameraPosition, glm::vec3& cameraFront, glm::vec3& cameraUp, bool* isJumping, double* jumpStartingTime);
	//void processJumping(GLFWwindow* window, glm::vec3& playerPosition, glm::vec3& cameraPosition, double* jumpStartingTime);
private:
};

#endif