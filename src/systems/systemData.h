#ifndef SYSTEM_DATA_H
#define SYSTEM_DATA_H

#include "entitySystem.h"
#include "transformSystem.h"
#include "graphicsSystem.h"
#include "physicsSystem.h"
#include "inputSystem.h"
#include "chunkSystem.h"

// from game.h:
struct GameData;
struct UpdateInfo;

struct SystemData {
	EntityData entityData;

	TransformData transformData;

	GraphicsData graphicsData;
	
	PhysicsData physicsData;

	InputData inputData;

	ChunkData chunkData;
};

void initSystemData(GameData& data);
void updateSystemData(GameData& data, UpdateInfo& update);

#endif
