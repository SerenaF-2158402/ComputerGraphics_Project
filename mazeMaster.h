#ifndef MAZEMASTER_H
#define MAZEMASTER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include "stb_image.h"
#include "shader.h"
#include "mazeGenerator.h"

// Settings to be changed to become dynamic
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

class mazeMaster
{
public:
	void runProgram();
	//mazeMaster();
private:

	//int makeWindow();
	mazeGenerator mazeGenerator;


	GLFWwindow* window;		// The window which has to be a singleton 

};

#endif 