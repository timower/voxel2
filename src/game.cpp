#include <glm/gtc/matrix_transform.hpp>

#include "platform.h"
#include "systems/systemData.h"
#include "entityFactory.h"
#include "message.h"

static Handle cube3;

// TODO: move to seperate file:
void loadScene(GameData& gameData) {
	SystemData& systemData = gameData.systemData;

	Handle camHandle = createEntity(systemData.entityData);

	addCameraComponent(systemData, camHandle);
	addTransformComponent(systemData, camHandle);
	addPhysComponent(systemData, camHandle);
	setControlEntity(gameData.systemData, camHandle);

	Transform camTrans;
	camTrans.position = glm::vec3(0.0f, 5.0f, -5.0f);
	camTrans.yaw = camTrans.roll = camTrans.pitch = 0;
	camTrans.scale = glm::vec3(1.0f);
	sendMessage(systemData, camHandle, SET_TRANSFORM, &camTrans);

	Handle cube = createCube(systemData);
	addPhysComponent(systemData, cube);
	Transform trans;
	trans.scale = glm::vec3(1.0f);
	trans.yaw = trans.roll = trans.pitch = 0;
	trans.position = glm::vec3(1.0f, 3.0f, 0.0f);
	sendMessage(systemData, cube, SET_TRANSFORM, &trans);


	Handle cube2 = createCube(systemData);
	trans.position = glm::vec3(-2.0f, 5.0f, 1.0f);
	sendMessage(systemData, cube2, SET_TRANSFORM, &trans);

	cube3 = createCube(systemData);
	trans.position = glm::vec3(-2.0f, 0.0f, 1.0f);
	sendMessage(systemData, cube3, SET_TRANSFORM, &trans);

	createChunk(systemData, glm::ivec3(0, -1, 0));
}

void initGame(GameData& data) {
	initSystemData(data);
	loadScene(data);
}

void updateGame(GameData& data, UpdateInfo& update) {
#ifndef NDEBUG
	static float lastTime = 0.0f;
	static size_t lastFrame = 0;
	static char titleBuffer[512];
	if (update.t - lastTime >= 1.0f) {
		size_t nframes = update.frame - lastFrame;
		//printf("%f ms/frame\n", 1000.0f / (float)nframes);
		sprintf(titleBuffer, "%f ms/frame", 1000.0f / (float)nframes);
		glfwSetWindowTitle(data.window, titleBuffer);
		lastFrame = update.frame;
		lastTime = update.t;
	}
#endif

	static float lastC = 0.0f;
	if (update.t - lastC >= 2.0f) {
		if (cube3.index) {
			sendMessage(data.systemData, cube3, DESTROY, nullptr);
			cube3.index = false;
		} else {
			cube3 = createCube(data.systemData);

			Transform trans;
			trans.scale = glm::vec3(1.0f);
			trans.yaw = trans.roll = trans.pitch = 0;
			trans.position = glm::vec3(-2.0f, 0.0f, 1.0f);

			sendMessage(data.systemData, cube3, SET_TRANSFORM, &trans);
		}
		lastC = update.t;
	}

	updateSystemData(data, update);
}
