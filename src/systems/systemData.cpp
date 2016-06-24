#include "systemData.h"

#include "../game.h"


void initSystemData(GameData& data) {
	initEntitySystem(data.systemData.entityData);

	initTransformSystem(data.systemData.transformData);

	initGraphicsSystem(data.systemData.graphicsData);

	initInputSystem(data);

	initPhysicsSystem(data.systemData.physicsData);

	initChunkSystem(data.systemData.chunkData);
}

void updateSystemData(GameData& data, UpdateInfo& update) {
	updateInputSystem(data);

	updateChunkSystem(data.systemData);

	updatePhysicsSystem(data.systemData, update.dt);

	updateTransformSystem(data.systemData);

	updateGraphicsSystem(data.systemData.graphicsData);
}
