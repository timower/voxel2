#ifndef GAME_H
#define GAME_H

struct GameData;

#include "platform.h"
#include "systems/systemData.h"

struct GameData {
	SystemData systemData;
	GLFWwindow* window;
	size_t memorySize;
	uint8_t memory[0];
};

struct UpdateInfo {
	float dt;
	float t;
	size_t frame;
};

void initGame(GameData& gameData, Config& config);
void updateGame(GameData& gameData, UpdateInfo& updateInfo);

#endif
