#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

#include <cstdarg>

#include "entitySystem.h"
#include "../transform.h"

struct GameData;
struct SystemData;

struct InputData {
	double lastX, lastY;
	
	Handle controlEntity;
};

void initInputSystem(GameData& gameData, Handle controlEntity);

void updateInputSystem(GameData& gameData);

void sendInputMessage(SystemData& systemData, Handle receiver, uint32_t type, void* arg);
#endif
