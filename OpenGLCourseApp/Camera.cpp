#include "Camera.h"

//Camera::Camera() {};

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed) 
		:position{startPosition}, front{glm::vec3(0.0f, 0.0f, -1.0f)}, up{0.0f, 0.0f, 0.0f}, right{0.0f, 0.0f, 0.0f}, 
		 worldUp{startUp}, yaw{startYaw}, pitch{startPitch}, moveSpeed{startMoveSpeed}, turnSpeed{startTurnSpeed}
{
		
		update();
}

void Camera::keyControl(bool* keys)
{
	// move forward
	if (keys[GLFW_KEY_W])					
	{
		position += front * moveSpeed;
	}

	// move backward
	if (keys[GLFW_KEY_S])
	{
		position -= front * moveSpeed;
	}

	// move left
	if (keys[GLFW_KEY_A])
	{
		position -= right * moveSpeed;
	}

	// move right
	if (keys[GLFW_KEY_D])
	{
		position += right * moveSpeed;
	}
}

glm::mat4 Camera::calculateViewMatrix()
{
	return glm::lookAt(position, position + front, up);				// position; camera position, position + front= target														
}

void Camera::update()
{ 
	// direction vector
	front.x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));	
	front.y = std::sin(glm::radians(pitch));								// check if roll can be used here if added
	front.z = std::sin(glm::radians(yaw)) * std::sin(glm::radians(pitch));
	front	= glm::normalize(front);										// normalize will divide by the length of vector giving a unit direction vector		

	right	= glm::normalize(glm::cross(front, worldUp));
	up		= glm::normalize(glm::cross(right, front));

}

Camera::~Camera() {}