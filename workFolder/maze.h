#ifndef MAZE_H
#define MAZE_H

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

// Settings to be changed to become dynamic
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


// Move this away from global
glm::vec3 playerPosition = glm::vec3(0.0f, 0.0f, 0.0f);
float playerYaw = -90.0f;
float playerPitch = 0.0f;

glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float cameraYaw = -90.0f;
float cameraPitch = 0.0f;
float cameraSpeed = 0.05f;


class maze {
public:
    maze();				// constructor
    mazeGenerator* newMaze;
    movement* move;
private:
    void runProgram();
    GLFWwindow* window;
    
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};

#endif