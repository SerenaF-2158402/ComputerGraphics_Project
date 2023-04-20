#include "movement.h"

movement::movement() {

}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void movement::processInput(GLFWwindow* window, glm::vec3& playerPosition, glm::vec3& cameraPosition, glm::vec3& cameraFront, glm::vec3& cameraUp, bool* isJumping, double* jumpStartingTime) {
    // Close window when pressing escape
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Handle the extra: jumping
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS || *isJumping) {
        if (!*isJumping)
            *jumpStartingTime = glfwGetTime();
        *isJumping = true;

        // Move upwards
        if (glfwGetTime() - *jumpStartingTime < 0.5f) {
            playerPosition[1] += 0.1f;
        }
        // End jump movement
        else if (glfwGetTime() - *jumpStartingTime <= 1.0f) {
            playerPosition[1] -= 0.1f;
        }
        // Move downwards
        else {
            *isJumping = false;
        }
    }

    // Handle player input for movement -> move to new function because ugly

    const float playerSpeed = 0.2f;
    // Calculate the movement direction based on the camera front vector
    glm::vec3 movementDirection = glm::normalize(glm::vec3(cameraFront.x, 0.0f, cameraFront.z));

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        playerPosition += playerSpeed * movementDirection;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        playerPosition -= playerSpeed * movementDirection;

    // Move left and right relative to the camera
    glm::vec3 rightDirection = glm::normalize(glm::cross(movementDirection, cameraUp));
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        playerPosition -= playerSpeed * rightDirection;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        playerPosition += playerSpeed * rightDirection;

    const float flyingSpeed = playerSpeed / 2;
    // Move up and down
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        playerPosition[1] += flyingSpeed;
    if (glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS)
        playerPosition[1] -= flyingSpeed;
    // Set a maximum height and depth
    if (playerPosition[1] > 100)
        playerPosition[1] = 100;
    if (playerPosition[1] < -10)
        playerPosition[1] = -10;


    //printf("Player position: %f, %f, %f\n\n", playerPosition[0], playerPosition[1], playerPosition[2]);

    // Handle mouse movement, using Pythagorean theorem to calculate distance from center so it moves faster further from the middle
    // PROBLEM: Doesn't work with resizing the screen so needs to be fixed
    // calculate mouse offset and distance from center
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    // Get screen size incase it changed
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    float xOffset = mouseX - width / 2.0;
    float yOffset = mouseY - height / 2.0;
    // Calculate distance from middle
    float distance = sqrt(xOffset * xOffset + yOffset * yOffset);

    // Calculate max distance possible
    float maxDistance = sqrt((width / 2.0f) * (width / 2.0f) + (height / 2.0f) * (height / 2.0f));
    // Set mouse sensitivity based on distance from the center of the screen
    float mouseSensitivity = 0.005f + 0.0001f * (distance / maxDistance);

    // Set a maximum on the mouse sensitivity so the screen doesn't fly away when turning too far
    if (mouseSensitivity > 0.005)
        mouseSensitivity = 0.005f;

    // Update the offset
    xOffset *= mouseSensitivity;
    yOffset *= mouseSensitivity;

    // Check if the mouse moved enough to make movement happen
    if (mouseSensitivity > 0.0002) {
        cameraYaw += xOffset;
        cameraPitch -= yOffset;
    }

    // Constrain pitch angle so the screen doesn't flip when moving too far
    const float maxPitch = 89.0f;
    const float minPitch = -89.0f;
    if (cameraPitch > maxPitch) {
        cameraPitch = maxPitch;
    }
    if (cameraPitch < minPitch) {
        cameraPitch = minPitch;
    }

}


// Handles jumping
//void movement::processJumping(GLFWwindow* window, glm::vec3& playerPosition, glm::vec3& cameraPosition, double *jumpStartingTime) {
//    // Check howlong it has been since jump was pressed
//   double timePassed = glfwGetTime() - *jumpStartingTime;
//    printf("Time passed: %f\n", timePassed);
//}