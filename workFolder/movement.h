#ifndef MOVEMENT_H
#define MOVEMENT_H

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


class movement
{
public:
	movement();
	void processInput(GLFWwindow* window, glm::vec3& playerPosition, glm::vec3& cameraFront, glm::vec3& cameraUp, bool* isJumping, double* jumpStartingTime, float* cameraYaw, float* cameraPitch);
	//void processJumping(GLFWwindow* window, glm::vec3& playerPosition, glm::vec3& cameraPosition, double* jumpStartingTime);
private:
};

#endif