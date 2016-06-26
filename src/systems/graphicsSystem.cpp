#include "graphicsSystem.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../platform.h"
#include "../message.h"

Handle addDrawComponent(SystemData& systemData, Handle entityHndl) {
	Handle ret = systemData.graphicsData.drawables.add();
	getGraphicsComponent(systemData.graphicsData, ret).entity = entityHndl;
	addComponent(systemData.entityData, entityHndl, ret);
	return ret;
}

GraphicsComponent& getGraphicsComponent(GraphicsData& graphicsData, Handle component) {
	return graphicsData.drawables.get(component);
}

Handle addCameraComponent(SystemData& systemData, Handle entityHndl) {
	Handle ret = {0, SystemTypes::CAMERA, 0};
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
	graphicsData.drawables.init();
}

void updateGraphicsSystem(GraphicsData& gData) {
	glClearColor(0.2, 0.5, 0.8, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 pv = gData.projectionMatrix * gData.viewMatrix;

	// TODO:
	// foreach (shader):
	// 	bindShader()
	// foreach (texture):
	//  bindTexture()
	// foreach (VAO):
	// 	bindVAO()
	// foreach (ModelMat):
	//   setUniform()
	//   draw()
	size_t nDrawables = gData.drawables.size;
	for (size_t i = 0; i < nDrawables; ++i) {
		const GraphicsComponent& drawable = gData.drawables.data[i];

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
			if (receiver.type == SystemTypes::CAMERA)
				systemData.graphicsData.viewMatrix = transformToView(*transform);
			else
				getGraphicsComponent(systemData.graphicsData, receiver).modelMat = transformToModel(*transform);
		} break;
		case GET_VBO: {
			GLuint* VBO = static_cast<GLuint*>(arg);
			*VBO = getGraphicsComponent(systemData.graphicsData, receiver).VBO;
		} break;
		case SET_NVERTICES: {
			size_t* nverts = static_cast<size_t*>(arg);
			getGraphicsComponent(systemData.graphicsData, receiver).nVertices = *nverts;
		} break;
		case DESTROY: {
			systemData.graphicsData.drawables.remove(receiver);
		} break;
	}
}
