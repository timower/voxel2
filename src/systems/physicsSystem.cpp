#include "physicsSystem.h"
#include "../message.h"

#define GRAV_A 9.81f

Handle addPhysComponent(SystemData& systemData, Handle entityHndl) {
	Handle ret = systemData.physicsData.components.add();
	getPhysComponent(systemData.physicsData, ret).entity = entityHndl;
	addComponent(systemData.entityData, entityHndl, ret);
	return ret;
}

PhysicsComponent& getPhysComponent(PhysicsData& pData, Handle component) {
	return pData.components.get(component);
}

void initPhysicsSystem(PhysicsData& physicsData) {
	physicsData.components.init();
}

void updatePhysicsSystem(SystemData& systemData, float dt) {
	size_t nComp = systemData.physicsData.components.size;
	for (size_t i = 0; i < nComp; i++) {
		PhysicsComponent& comp = systemData.physicsData.components.data[i];

		glm::vec3 velocity = comp.velocity + comp.physVel;

		Transform transform;
		sendMessage(systemData, comp.entity, GET_TRANSFORM, &transform);

		glm::vec3 newPosition = transform.position + velocity * dt;
		comp.physVel.y -= GRAV_A * dt;
		// TODO: collision detection.
		if (newPosition.y <= 0) {
			newPosition.y = 0;
			comp.physVel.y = 0;
		}
		transform.position = newPosition;

		sendMessage(systemData, comp.entity, SET_TRANSFORM, &transform);
	}
}

void sendPhysicsMessage(SystemData& systemData, Handle receiver, uint32_t type, void* arg) {
	switch (type) {
		case SET_VELOCITY: {
			glm::vec3* velocity = static_cast<glm::vec3*>(arg);
			getPhysComponent(systemData.physicsData, receiver).velocity = *velocity;
		} break;
	}
}
