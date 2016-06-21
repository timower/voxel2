#ifndef SYSTEM_DATA_H
#define SYSTEM_DATA_H

#include "entitySystem.h"
#include "transformSystem.h"
#include "graphicsSystem.h"
#include "physicsSystem.h"
#include "inputSystem.h"

// from game.h:
struct GameData;
struct UpdateInfo;

enum SystemTypes {
	ENTITY = 0,
	TRANSFORM,
	GRAPHICS,
	PHYSICS,
	INPUT
};

struct SystemData {
	EntityData entityData;
	TransformData transformData;
	GraphicsData graphicsData;
	PhysicsData physicsData;
	InputData inputData;
};

void initSystemData(GameData& data);
void updateSystemData(GameData& data, UpdateInfo& update);

#endif
