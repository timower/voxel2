#include "platform.h"
#include "game.h"

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <dlfcn.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <SOIL.h>

void stop() {
	glfwTerminate();
	exit(0);
}

static void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	std::cout << "frambuffer set" << std::endl;
}

int main(int argc, char* argv[]) {
	// init window:
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "test", nullptr, nullptr);
	if (!window) {
		std::cout << "Error creating window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "error initing glew" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	glfwSwapInterval(0); // TODO: config
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	// init game:
	size_t allocatedMemory = MEGABYTES(64);
	assert(allocatedMemory >= sizeof(GameData));
	GameData* gameData = (GameData*)calloc(1, allocatedMemory);
	if (!gameData) {
		std::cout << "error allocating memory" << std::endl;
		glfwTerminate();
		return -1;
	}
	gameData->window = window;
	gameData->memorySize = allocatedMemory - sizeof(GameData);
	glfwSetWindowUserPointer(window, gameData);
	initGame(*gameData);

	// gameLoop:
	float lastTime = glfwGetTime();
	size_t frame = 0;

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		float curTime = glfwGetTime();
		UpdateInfo info = {
			.dt = curTime - lastTime,
			.t = curTime,
			.frame = frame
		};
		lastTime = curTime;

		updateGame(*gameData, info);

		glfwSwapBuffers(window);
		frame++;
	}
	free(gameData); // NOTE: maybe remove, OS cleans memory?
	stop();
}
