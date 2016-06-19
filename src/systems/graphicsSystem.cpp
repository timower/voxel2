#include "graphicsSystem.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../platform.h"
#include "../message.h"

Handle addDrawComponent(SystemData& systemData, Handle entityHndl) {
	assert(systemData.graphicsData.nDrawables < MAX_DRAWABLES);

	uint16_t idx = systemData.graphicsData.nDrawables++;
	systemData.graphicsData.drawables[idx] = {.entity = entityHndl};
	idx += 1;

	Handle ret = {idx, SystemTypes::GRAPHICS, 0};
	addComponent(systemData.entityData, entityHndl, ret);
	return ret;
}

GraphicsComponent& getGraphicsComponent(GraphicsData& graphicsData, Handle component) {
	assert(component.type == SystemTypes::GRAPHICS);
	auto index = component.index - 1;
	assert(index < graphicsData.nDrawables);
	return graphicsData.drawables[index];
}

Handle addCameraComponent(SystemData& systemData, Handle entityHndl) {
	Handle ret = {0, SystemTypes::GRAPHICS, 0};
	addComponent(systemData.entityData, entityHndl, ret);
	return ret;
}

void initGraphicsSystem(GraphicsData& graphicsData) {
	graphicsData.projectionMatrix = glm::perspective(
		glm::radians(80.0f),
		(float) WIDTH / (float)HEIGHT,
		0.01f,
		100.0f
	);
}

void updateGraphicsSystem(GraphicsData& gData) {
	glClearColor(0.2, 0.5, 0.8, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// TODO: fix camera & view
	glm::mat4 pv = gData.projectionMatrix * gData.viewMatrix;
	//glm::mat4 pv = gData.projectionMatrix * transformToView(gData.cameraTransform);

	size_t nDrawables = gData.nDrawables;
	for (size_t i = 0; i < nDrawables; ++i) {
		const GraphicsComponent& drawable = gData.drawables[i];

		glBindTexture(GL_TEXTURE_2D, drawable.TEX);

		glUseProgram(drawable.program); // TODO: sort & don't rebind program and stuff

		glm::mat4 pvm = pv * drawable.modelMat;
		GLint mvpLoc = glGetUniformLocation(drawable.program, "MVP");
		glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(pvm));

		glBindVertexArray(drawable.VAO);

		glDrawElements(GL_TRIANGLES, drawable.nVertices, GL_UNSIGNED_INT, (GLvoid*)0);
		glBindVertexArray(0);
	}
}

void sendGraphicsMessage(SystemData& systemData, Handle receiver, uint32_t type, void* arg) {
	switch (type) {
		case SET_GRAPH_TRANS: {
			Transform* transform = static_cast<Transform*>(arg);
			if (receiver.index == 0)
				systemData.graphicsData.viewMatrix = transformToView(*transform);
			else
				getGraphicsComponent(systemData.graphicsData, receiver).modelMat = transformToModel(*transform);
		} break;
	}
}
