#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

#include <cstdarg>

#include "entitySystem.h"
#include "../transform.h"

#define CLICK_TIME 0.2f

struct GameData;
struct SystemData;
struct UpdateInfo;

struct InputData {
	double lastX, lastY;

	float leftClickTimer, rightClickTimer;

	Handle controlEntity;
};


void initInputSystem(GameData& gameData);
void setControlEntity(SystemData& systemData,  Handle controlEntity);

void updateInputSystem(GameData& gameData, UpdateInfo& update);

void sendInputMessage(SystemData& systemData, Handle receiver, uint32_t type, void* arg);
#endif
