#include <glm/gtc/matrix_transform.hpp>

#include "platform.h"
#include "systems/systemData.h"
#include "entityFactory.h"
#include "message.h"


void initGame(GameData& gameData) {
	SystemData& systemData = gameData.systemData;

	Handle camHandle = createEntity(systemData.entityData);

	addCameraComponent(systemData, camHandle);
	addTransformComponent(systemData, camHandle);
	addPhysComponent(systemData, camHandle);
	initInputSystem(gameData, camHandle);
	//initInputSystem(gameData, camHandle);


	Transform camTrans;
	camTrans.position = glm::vec3(0.0f, 5.0f, -5.0f);
	camTrans.yaw = camTrans.roll = camTrans.pitch = 0;
	camTrans.scale = glm::vec3(1.0f);
	sendMessage(systemData, camHandle, SET_TRANSFORM, &camTrans);

	//addPhysComponent(systemData.physicsData, systemData.entityData, camHndl);


	initGraphicsSystem(systemData.graphicsData);

	Handle cube = createCube(systemData, 1.0f);
	addTransformComponent(systemData, cube);
	addPhysComponent(systemData, cube);
	Transform trans;
	trans.scale = glm::vec3(1.0f);
	trans.yaw = trans.roll = trans.pitch = 0;
	trans.position = glm::vec3(1.0f, 3.0f, 0.0f);
	sendMessage(systemData, cube, SET_TRANSFORM, &trans);


	Handle cube2 = createCube(systemData, 1.0f);
	addTransformComponent(systemData, cube2);
	trans.position = glm::vec3(-2.0f, 5.0f, 1.0f);
	sendMessage(systemData, cube2, SET_TRANSFORM, &trans);

	Handle cube3 = createCube(systemData, 1.0f);
	addTransformComponent(systemData, cube3);
	trans.position = glm::vec3(-2.0f, 0.0f, 1.0f);
	sendMessage(systemData, cube3, SET_TRANSFORM, &trans);
}

void updateGame(GameData& gameData, UpdateInfo& updateInfo) {
	SystemData& systemData = gameData.systemData;

#ifndef NDEBUG
	static float lastTime = 0.0f;
	static size_t lastFrame = 0;
	if (updateInfo.t - lastTime >= 1.0f) {
		size_t nframes = updateInfo.frame - lastFrame;
		printf("%f ms/frame\n", 1000.0f / (float)nframes);
		lastFrame = updateInfo.frame;
		lastTime = updateInfo.t;
	}
#endif

	updateInputSystem(gameData);

	updatePhysicsSystem(systemData, updateInfo.dt);

	updateTransformSystem(systemData);

	updateGraphicsSystem(systemData.graphicsData);
}
