#include "systemData.h"

#include "../game.h"


void initSystemData(GameData& data, Config& config) {
	initEntitySystem(data.systemData.entityData);

	initTransformSystem(data.systemData.transformData);

	initGraphicsSystem(data.systemData.graphicsData, config);

	initInputSystem(data);

	initPhysicsSystem(data.systemData.physicsData);

	initChunkSystem(data.systemData.chunkData);
}

void updateSystemData(GameData& data, UpdateInfo& update) {
	updateInputSystem(data, update);

	updateChunkSystem(data.systemData, update);

	updatePhysicsSystem(data.systemData, update.dt);

	updateTransformSystem(data.systemData);

	updateGraphicsSystem(data.systemData.graphicsData);
}
