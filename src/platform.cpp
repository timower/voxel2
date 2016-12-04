#include "platform.h"
#include "game.h"

#include <iostream>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <SOIL.h>

void stop() {
	glfwTerminate();
	exit(0);
}

static void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	// TODO: notify graphics system -> recreate projection matrix.
	glViewport(0, 0, width, height);
	std::cout << "frambuffer set" << std::endl;
}

void loadConfig(Config& config) {
	config.width = 1280;
	config.height = 720;
	config.vsync = 1;
	config.fov = 80.0f;
}

int main(int argc, char* argv[]) {
	// init window:
	Config config;
	loadConfig(config);

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(config.width, config.height, "TODO", nullptr, nullptr);
	if (!window) {
		std::cout << "Error creating window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (gl3wInit()) {
		std::cout << "error initing glew" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	glfwSwapInterval(config.vsync);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	// init game:
	size_t allocatedMemory = MEGABYTES(64);
	assert(allocatedMemory >= sizeof(GameData));
	//GameData* gameData = (GameData*)calloc(1, allocatedMemory);
	GameData* gameData = new (calloc(1, allocatedMemory))GameData;

	if (!gameData) {
		std::cout << "error allocating memory" << std::endl;
		glfwTerminate();
		return -1;
	}
	gameData->window = window;
	gameData->memorySize = allocatedMemory - sizeof(GameData);
	glfwSetWindowUserPointer(window, gameData);
	initGame(*gameData, config);

	// gameLoop:
	float lastTime = glfwGetTime();
	size_t frame = 0;

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		float curTime = glfwGetTime();
		UpdateInfo info = {
			curTime - lastTime,
			curTime,
			frame
		};
		lastTime = curTime;

		updateGame(*gameData, info);

		glfwSwapBuffers(window);
		frame++;
	}
	free(gameData); // NOTE: maybe remove, OS cleans memory?
	stop();
}
