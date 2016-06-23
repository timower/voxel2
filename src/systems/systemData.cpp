#include "systemData.h"

#include "../game.h"


void initSystemData(GameData& data) {
	initEntitySystem(data.systemData.entityData);

	initTransformSystem(data.systemData.transformData);

	initGraphicsSystem(data.systemData.graphicsData);

	initInputSystem(data);

	initPhysicsSystem(data.systemData.physicsData);
}

void updateSystemData(GameData& data, UpdateInfo& update) {
	updateInputSystem(data);

	updatePhysicsSystem(data.systemData, update.dt);

	updateTransformSystem(data.systemData);

	updateGraphicsSystem(data.systemData.graphicsData);
}
