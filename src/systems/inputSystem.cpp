#include "inputSystem.h"
#include "../game.h"
#include "../message.h"

#include <glm/gtc/constants.hpp>
#include <glm/gtx/euler_angles.hpp>

#define SPEED 1.5f
#define JUMP_SPEED 5.0f

void initInputSystem(GameData& gameData, Handle controlEntity) {
	SystemData& systemData = gameData.systemData;
	systemData.inputData.controlEntity = controlEntity;
	addComponent(systemData.entityData, controlEntity, {0, SystemTypes::INPUT, 0});

	double xpos, ypos;
	glfwGetCursorPos(gameData.window, &xpos, &ypos);
	systemData.inputData.lastX = xpos;
	systemData.inputData.lastY = ypos;
}

void updateInputSystem(GameData& gameData) {
	GLFWwindow* window = gameData.window;
	InputData& inputData = gameData.systemData.inputData;

	double xPos, yPos;
	glfwGetCursorPos(window, &xPos, &yPos);
	float dx = static_cast<float>(inputData.lastX - xPos);
	float dy = static_cast<float>(inputData.lastY - yPos);
	inputData.lastX = xPos;
	inputData.lastY = yPos;

	Transform controlTrans;
	sendMessage(gameData.systemData, inputData.controlEntity, GET_TRANSFORM, &controlTrans);

	controlTrans.yaw += dx * 0.005f; // TODO: config
	controlTrans.pitch -= dy * 0.005f;

	controlTrans.pitch = glm::clamp(controlTrans.pitch, -glm::radians(89.0f), glm::radians(89.0f));
	controlTrans.yaw = glm::mod(controlTrans.yaw, 2.0f * glm::pi<float>());


	bool up = glfwGetKey(window, GLFW_KEY_W);
	bool down = glfwGetKey(window, GLFW_KEY_S);
	bool left = glfwGetKey(window, GLFW_KEY_A);
	bool right = glfwGetKey(window, GLFW_KEY_D);
	//bool jump = glfwGetKey(window, GLFW_KEY_SPACE);

	float vertical = up * 1.0f + down * -1.0f; // local z
	float horizontal = left * 1.0f + right * -1.0f; // local x

	glm::vec3 velocity = (vertical * FRONT) + (horizontal * LEFT);
	velocity = transformRotation(controlTrans) * velocity;
	velocity.y = 0;
	if (glm::dot(velocity, velocity) != 0)
		velocity = glm::normalize(velocity) * SPEED;

	sendMessage(gameData.systemData, inputData.controlEntity, SET_TRANSFORM, &controlTrans); // TODO: remove, only needed for angles.
	sendMessage(gameData.systemData, inputData.controlEntity, SET_VELOCITY, &velocity);
}

void sendInputMessage(SystemData& systemData, Handle receiver, uint32_t type, void* arg) {
	// NOP?
}