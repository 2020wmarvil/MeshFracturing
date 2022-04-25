#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
public:
	Camera(glm::vec3 position, glm::vec3 forward, glm::vec3 up, float width, float height);

    void UpdateMousePosition(double xpos, double ypos);
    void UpdateScrollPosition(double xoffset, double yoffset);
    void ProcessWindowEvents(GLFWwindow* window, float dt);

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();

	glm::vec3 position, forward, up;
	float speed = 2.0f;
	bool firstMouse = true;
private:
	float pitch = 0.0f, yaw = -90.0f;
	float fov = 45.0f;
	float nearClip = 0.1f, farClip = 100.0f;
	float width, height;

	float lastX, lastY;
};

Camera::Camera(glm::vec3 position, glm::vec3 forward, glm::vec3 up, float width, float height) {
	this->position = position;
	this->forward = forward;
	this->up = up;
	this->width = width;
	this->height = height;
}

void Camera::UpdateMousePosition(double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        pitch = glm::degrees(asin(forward.y));
        yaw = glm::degrees(atan2(forward.x, forward.z));
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f) pitch = 89.0f;
    if(pitch < -89.0f) pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    forward = glm::normalize(direction);
}

void Camera::UpdateScrollPosition(double xoffset, double yoffset) {
    fov -= (float)yoffset;
    if (fov < 1.0f) fov = 1.0f;
    if (fov > 90.0f) fov = 90.0f; 
}

void Camera::ProcessWindowEvents(GLFWwindow *window, float dt) {
    const float camSpeed = speed * dt;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        position += forward * camSpeed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        position -= forward * camSpeed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        position -= glm::normalize(glm::cross(forward, up)) * camSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        position += glm::normalize(glm::cross(forward, up)) * camSpeed;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        position -= up * camSpeed;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        position += up * camSpeed;
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(position, position + forward, up);
}

glm::mat4 Camera::GetProjectionMatrix() {
    return glm::perspective(glm::radians(fov), width / height, nearClip, farClip);
}
#endif
